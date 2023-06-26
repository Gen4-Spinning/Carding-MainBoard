/*
 * IdleState.c
 *
 *  Created on: 15-Apr-2023
 *      Author: harsha
 *
 *  Things to Do in Idle State for Flyer:
 *  1. Detect Button Press and do homing/ inching and start the full run cycle
 *  2. Save New settings from the app when we get them
 *  3. Write the Dbg codes to start motors. Dbg Stop is in the while loop.
 */

#include "stdio.h"
#include "MachineErrors.h"
#include "StateMachine.h"
#include "BT_Fns.h"
#include "CommonConstants.h"
#include "CAN_MotherBoard.h"
#include "MotorComms.h"
#include "Ack.h"
#include "userButtons.h"
#include "SysObserver.h"
#include "TowerLamp.h"
#include "mcp23017.h"
#include "MachineSensors.h"
#include "Log.h"

extern UART_HandleTypeDef huart1;

void RunState(void){

	/* when you enter this state, start the cylinder motors. When the cylinders have reached their top speed,
	 * start the other motors.
	 * When you press the yellow button, Pause.When u press the green, Resume.
	 * The red btn, will stop the run and go back to idle.
	 * the rotary switch disables or enables the coiler duct sensor
	 * if the coiler sensor is on, and it triggers go into pause state and turn off all the motors except the
	 * cylinders
	 * if the duct sensor triggers, turn off the beater feed motor, and wait till it releases before you turn it
	 * back on again.
	 * also need to check if the production limit has been reached and stop the motors if you have.
	 */
	uint8_t response = 0;
	uint8_t noOfMotors = 0;
	uint8_t BTpacketSize = 0;
	while(1){

		if (S.oneTime){
			//send the start commands
			uint8_t motors[] = {CARDING_CYLINDER,BEATER_CYLINDER};
			noOfMotors = 2;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,START);
			if (response!= 2){
				SO_enableCANObservers(&SO,motors,noOfMotors);
			}

			S.runMode = RUN_OPERATING;
			mcParams.allMotorsOn = 0;

			Log_setUpLogging(&L,motors,noOfMotors);
			Log_ResetBufferIndex(&L);
			if (S.LOG_enabled){
				L.bufferIdx += Log_addSettingsDataToBuffer(&msp,L.bufferIdx);
				L.logLayerChange = 1; //one time set it here so that when we start we get 0th layer rdngs.
				L.logRunStateChange = 1;
			}
			//when u start your in the run mode
			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_ON,AMBER_OFF);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);

			S.oneTime = 0;
		}

		if ((usrBtns.yellowBtn == BTN_PRESSED) && (S.runMode == RUN_OPERATING)){
			usrBtns.yellowBtn = BTN_IDLE;
			//Pause
			uint8_t motors[] = {CARDING_FEED,BEATER_FEED,CAGE,COILER};
			noOfMotors = 4;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,RAMPDOWN_STOP);
			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_OFF,AMBER_ON);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);

			//TODO - later make it such that once the CAN starts it never needs to stop.
			SO_disableAndResetCANObservers(&SO);

			S.runMode = RUN_PAUSED;
			S.BT_pauseReason = BT_PAUSE_USER_PRESSED;
			L.logRunStateChange = 1;
		}

		if ((usrBtns.greenBtn == BTN_PRESSED)  && (S.runMode == RUN_PAUSED)){
			usrBtns.greenBtn = BTN_IDLE;
			//RESUME
			uint8_t motors[] = {CARDING_FEED,BEATER_FEED,CAGE,COILER};
			noOfMotors = 4;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,RESUME);

			//TODO - later make it such that once the CAN starts it never needs to stop.
			// so we will not have this statement.
			if (response!= 2){
				SO_enableCANObservers(&SO,motors,noOfMotors);
			}
			S.runMode = RUN_OPERATING;

			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_ON,AMBER_OFF);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);
			L.logRunStateChange = 1;
		}

		if ((usrBtns.rotarySwitch == ROTARY_SWITCH_ON) && (sensor.coilerSensor_activated == 0)){
			SetCoilerSensorState(&sensor,SENSOR_ENABLE);
		}

		if ((usrBtns.rotarySwitch == ROTARY_SWITCH_OFF) && (sensor.coilerSensor_activated == 1)){
			SetCoilerSensorState(&sensor,SENSOR_DISABLE);
		}

		// stop btn
		if (usrBtns.redBtn == BTN_PRESSED){
			usrBtns.redBtn = BTN_IDLE;
			//STOP
			uint8_t motors[] = {CARDING_CYLINDER,BEATER_CYLINDER,CARDING_FEED,BEATER_FEED,CAGE,COILER};
			noOfMotors = 6;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,EMERGENCY_STOP);
			S.runMode = RUN_STOPPED ;
			S.BT_pauseReason = 0;

			SO_disableAndResetCANObservers(&SO);

			//beep once when we go to idle
			TowerLamp_SetState(&hmcp,&mcp_portB,BUZZER_ON,RED_OFF,GREEN_OFF,AMBER_OFF);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);
			HAL_Delay(1000); // to hear the beep

			ChangeState(&S,IDLE_STATE);
			SO_Reset_InitialLiftPosRecieved(&SO);
			break;
		}

		// TO CHANGE for the carding sensors
		/*if (sensor.latchedtrunkSensor){
			//Pause
			sensor.latchedtrunkSensor = 0;
			uint8_t motors[] = {CARDING_FEED,BEATER_FEED,CAGE,COILER};
			noOfMotors = 6;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,RAMPDOWN_STOP);
			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_OFF,AMBER_ON);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);

			//TODO - later make it such that once the CAN starts it never needs to stop.
			SO_disableAndResetCANObservers(&SO);

			S.runMode = RUN_PAUSED;
			S.BT_pauseReason = BT_PAUSE_CREEL_SLIVER_CUT;
			L.logRunStateChange = 1;
		}
		*/
		//--------ways to go into Error State--------

		//Error State
		if(ME.ErrorFlag == 1){
			ChangeState(&S,ERROR_STATE);
			break;
		}

		if (S.LOG_enabled){
			Log_DoOneCycle();
		}

		//--------sending BT info--------
		// 500ms timer.
		if ((S.BT_sendState == 1) && (S.BT_transmission_over == 1)){
			if (S.runMode == RUN_OPERATING ){
				BTpacketSize = BT_MC_generateStatusMsg(BT_RUN);
			}else{
				BTpacketSize = BT_MC_generateStatusMsg(BT_PAUSE);
			}
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			S.BT_transmission_over = 0;
			S.BT_sendState = 0;
		}

		//TODO:TO STOP WHEN LENGTH FINISHED
		if (mcParams.currentMtrsRun >= msp.lengthLimit){
			ChangeState(&S,FINISHED_STATE);
			break;
		}



	}//closes while

}

