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
#include "BT_Machine.h"

extern UART_HandleTypeDef huart1;

uint8_t ductStateDirect = 0;
uint8_t ductStateDirectCount = 0;
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
	long currentTime;
	uint8_t rampOver = 0;
	while(1){

		if (S.oneTime){
			//send the start commands
			uint8_t motors[] = {CARDING_CYLINDER,BEATER_CYLINDER,COILER};
			noOfMotors = 3;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,START);
			if (response!= 2){
				SO_enableCANObservers(&SO,motors,noOfMotors);
			}

			S.runMode = RUN_RAMPUP;
			mcParams.allMotorsOn = 0;
			rampOver = 0;

			uint8_t motors1[] = {CARDING_CYLINDER,BEATER_CYLINDER,CARDING_FEED,BEATER_FEED,CAGE,COILER};
			Log_setUpLogging(&L,motors1,6);
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

		if (S.runMode == RUN_RAMPUP){
			rampOver = CheckCylindersRampUpOver(&mcParams,&R[CARDING_CYLINDER],&R[BEATER_CYLINDER]);
			if (rampOver == 1){
				S.runMode = RUN_ALL;
				mcParams.allMotorsOn = 1;
				//before we start the other motors we need to stop the can checking
				SO_disableAndResetCANObservers(&SO);

				uint8_t motors[] = {CARDING_FEED,BEATER_FEED,CAGE};
				noOfMotors = 3;
				response = SendCommands_To_MultipleMotors(motors,noOfMotors,START);

				//TODO - later make it such that once the CAN starts it never needs to stop.
				// so we will not have this statement.
				if (response!= 2){
					uint8_t motors1[] = {CARDING_CYLINDER,BEATER_CYLINDER,CARDING_FEED,BEATER_FEED,CAGE,COILER};
					noOfMotors = 6;
					SO_enableCANObservers(&SO,motors1,noOfMotors);
				}
				rampOver = 0;
			}
		}

		if ((usrBtns.yellowBtn == BTN_PRESSED) && (S.runMode == RUN_ALL)){
			usrBtns.yellowBtn = BTN_IDLE;
			//Pause
			uint8_t motors[] = {CARDING_FEED,BEATER_FEED,CAGE,COILER};
			noOfMotors = 4;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,RAMPDOWN_STOP);
			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_OFF,AMBER_ON);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);

			//TODO - later make it such that once the CAN starts it never needs to stop.
			// if we disable here, we stop checking the cylinder and beater also.
			// TODO: fix this.
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
			S.runMode = RUN_ALL;

			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_ON,AMBER_OFF);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);
			L.logRunStateChange = 1;
		}

		if (S.oneSecTimer != currentTime){
			mcParams.totalPower = R[0].power + R[1].power + R[2].power + R[3].power + R[4].power + R[5].power;
			currentTime = S.oneSecTimer;
		}


		if (usrBtns.rotarySwitch == ROTARY_SWITCH_ON){

		}else{

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

		//if settings modified through app for carding
		if (S.settingsModified == 1){
			UpdateMachineParameters(&msp,&mcParams);
			uint8_t motors[] = {CARDING_FEED,BEATER_FEED,CAGE,COILER};
			uint16_t targets[] = {mcParams.cylinderFeedRPM,mcParams.beaterFeedRPM,
												mcParams.cageRPM,mcParams.coilerRPM};
			noOfMotors = 4;
			SendChangeTargetToMultipleMotors(motors,noOfMotors,targets);
			S.settingsModified = 0;
		}

		//read once a sec
		sensor.ductSensor = Sensor_ReadValueDirectly(&hmcp,&mcp_portB_sensorVal,DUCT_SENSOR);
		if (sensor.ductCurrentState == DUCT_SENSOR_OPEN){
			if (sensor.ductSensor  == DUCT_SENSOR_CLOSED){
				if (sensor.ductTimerIncrementBool == 0){
					sensor.ductSensorTimer = 0;
					sensor.ductTimerIncrementBool = 1;
				}else{
					if (sensor.ductSensorTimer >= msp.trunkDelay){
						uint8_t motors[] = {BEATER_FEED};
						noOfMotors = 1;
						response = SendCommands_To_MultipleMotors(motors,noOfMotors,RAMPDOWN_STOP);
						if (response == 1){
							sensor.ductCurrentState = DUCT_SENSOR_CLOSED;
							sensor.ductTimerIncrementBool = 0;
							SO_disableCanObserver(&SO,BEATER_FEED);
						}
					}
				}
			}else{ //might have triggered but not for trunk delay time, so restart the count
				sensor.ductTimerIncrementBool = 0;
				sensor.ductCurrentState = DUCT_SENSOR_OPEN;
			}
		}else{ // if (sensor.ductCurrentState == DUCT_SENSOR_CLOSED){
			if (sensor.ductSensor  == DUCT_SENSOR_OPEN){
				if (sensor.ductTimerIncrementBool == 0){
					sensor.ductSensorTimer = 0;
					sensor.ductTimerIncrementBool = 1;
				}else{
					if (sensor.ductSensorTimer >= msp.trunkDelay){

						uint8_t motors[] = {BEATER_FEED};
						noOfMotors = 1;
						response = SendCommands_To_MultipleMotors(motors,noOfMotors,RESUME);

						if (response == 1){
							sensor.ductCurrentState = DUCT_SENSOR_OPEN;
							sensor.ductTimerIncrementBool = 0;

							//enable all motors again
							uint8_t motors1[] = {CARDING_CYLINDER,BEATER_CYLINDER,CARDING_FEED,BEATER_FEED,CAGE,COILER};
							noOfMotors = 6;
							SO_enableCANObservers(&SO,motors1,noOfMotors);

						}
					}
				}
			}else{ //might have triggered but not for trunk delay time, so restart the count
				sensor.ductTimerIncrementBool = 0;
				sensor.ductCurrentState = DUCT_SENSOR_CLOSED;
			}
		}

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
			if (S.runMode != RUN_PAUSED){
				BTpacketSize = BT_MC_generateStatusMsg(BT_RUN);
			}else{
				BTpacketSize = BT_MC_generateStatusMsg(BT_PAUSE);
			}
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			S.BT_transmission_over = 0;
			S.BT_sendState = 0;
		}

		/*---- go into settings when need be-----*/
		if (S.switchState == TO_SETTINGS){
			if (BT.subState == UPDATE_CARDING_SETTINGS){ // BUG FIX. NEED TO SEE WHATS HAPPENING WITH
				if (S.runMode ==  RUN_ALL){
					uint8_t motors[] = {BEATER_FEED};
					noOfMotors = 1;
					response = SendCommands_To_MultipleMotors(motors,noOfMotors,RESUME);
				}
			}else{
				ChangeState(&S,SETTINGS_STATE);
			}
			S.switchState = 0;
			break;
		}

		//without stopping blink and blare to tell the indicator to change the CAN, and then go back to
		//your normal run mode
		if (mcParams.currentMtrsRun >= msp.lengthLimit){
			if (S.runMode!= RUN_OVER){
				S.runMode = RUN_OVER;
				S.oneSecTimer = 0;
				TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_ON,RED_ON,GREEN_ON,AMBER_ON);
				TowerLamp_ApplyState(&hmcp,&mcp_portB);
			}else{
				if (S.oneSecTimer == 10){
					S.runMode = RUN_ALL;
					mcParams.currentMtrsRun = 0;
					TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_ON,AMBER_OFF);
					TowerLamp_ApplyState(&hmcp,&mcp_portB);
				}
			}
		}

	}//closes while

}

