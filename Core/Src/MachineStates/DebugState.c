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
#include "StateMachine.h"
#include "CommonConstants.h"
#include "CAN_MotherBoard.h"
#include "MotorComms.h"
#include "Ack.h"
#include "userButtons.h"
#include "FDCAN.h"
#include "SMPS.h"

#include "MachineErrors.h"
#include "BT_Fns.h"
#include "BT_Machine.h"
uint8_t doActivity = 0 ;

uint8_t dbg_Start = 0;
uint8_t dbg_stop = 0;
uint8_t dbg_pause = 0;
uint8_t dbg_resume = 0;

uint8_t response = 0;
uint8_t stopSMPSNow = 0;

uint8_t motors[] = {CARDING_CYLINDER};
uint8_t noOfMotors = 1;

uint8_t sendBTCmd,btReturn,btCmd = 0;

uint8_t BT_errDbg = 0;
uint8_t BT_pauseDbg = 0;
uint8_t BT_runDbg = 0;
uint8_t BTpacketSize = 0;
uint16_t errSource1 = 0 ;

uint8_t SMPS_on,SMPS_off;

extern UART_HandleTypeDef huart1;
void DebugState(void){

	while(1){


		if (SMPS_on){
			SMPS_TurnOn();
			SMPS_on = 0;
		}
		if (SMPS_off){
			SMPS_TurnOff();
			SMPS_off = 0;
		}

		if(dbg_Start){
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,START);
			dbg_Start = 0;
		}

		if (dbg_pause){
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,RAMPDOWN_STOP);
			dbg_pause =0;
		}

		if (dbg_resume){
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,RESUME);
			dbg_resume = 0;
		}

		if (dbg_stop){
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,EMERGENCY_STOP);
			dbg_stop = 0;
		}

		//BT command mode commands
		if (sendBTCmd){
			btReturn = BTConsole_WriteCMD(btCmd);
			sendBTCmd = 0;
		}

		if (BT_errDbg){
			if (ME.errType1 == ERR_MOTOR_SOURCE){
				errSource1 = GetBTMotorID_from_Motor_ID(ME.errSource1);
			}else{
				errSource1 = ME.errSource1;
			}
			SetBTErrors(&ME,ME.errReason1,errSource1,ME.errCode1);
			BTpacketSize = BT_MC_generateStatusMsg(BT_STOP);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			BT_errDbg = 0;
		}

		if (BT_pauseDbg){
			BTpacketSize = BT_MC_generateStatusMsg(BT_PAUSE);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			BT_pauseDbg = 0;
		}

		if ((S.BT_sendState == 1) && (S.BT_transmission_over == 1)){
			BT_runDbg = 1;
		}
		if (BT_runDbg){
			BTpacketSize = BT_MC_generateStatusMsg(BT_RUN);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			BT_runDbg = 0;
			S.BT_transmission_over = 0;
			S.BT_sendState = 0;
		}

		if (S.switchState == TO_SETTINGS){
			ChangeState(&S,SETTINGS_STATE);
			S.switchState = 0;
			break;
		}


	}//closes while

}

