/*
 * CAN_MotherBoardFns.c
 *
 *  Created on: Mar 7, 2023
 *      Author: harsha
 */
#include "main.h"
#include "stm32g4xx_hal.h"
#include "FDCAN.h"
#include "CAN_MotherBoard.h"
#include "CommonConstants.h"
#include "Struct.h"
#include "StateMachine.h"
#include "machineSettings.h"
#include "MachineErrors.h"
#include "SysObserver.h"
#include "Log.h"
#include "Ack.h"

/* Mother Board Funs*/

void FDCAN_sendCommand_ToMotor(uint8_t destination,uint8_t command){
	TxHeader.Identifier =(0x601<<16)|(destination<<8)|0x01;
	TxHeader.DataLength = FDCAN_DLC_BYTES_1;
	TxData[0] = command;
	while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1)==0){};
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
}

void FDCAN_sendSetUp_ToMotor(uint8_t destination,SetupMotor rd){
	TxHeader.Identifier =(0xA07<<16)|(destination<<8)|0x01;
	TxHeader.DataLength = FDCAN_DLC_BYTES_4;
	TxData[0]=rd.RUT;
	TxData[1]=rd.RDT;
	TxData[2]=rd.RPM>>8;
	TxData[3]=rd.RPM;
	while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1)==0){};
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
}

void FDCAN_SendDiagnostics_ToMotor(uint8_t destination,DiagnosticsTypeDef *d)
{
	TxHeader.Identifier =(0xE0A<<16)|(destination<<8)|0x01;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxData[0]=d->typeofTest;
	if (d->typeofTest == OPENLOOP){
		TxData[1]= d->targetDuty>>8;
		TxData[2]= d->targetDuty;
	}else{
		TxData[1]= d->targetRPM>>8 & 0xff;
		TxData[2]= d->targetRPM & 0xff;
	}
	TxData[3]=d->rampUpTime;
	TxData[4]=d->rampDownTime;
	TxData[5]=(d->runTime>>8);
	TxData[6]= d->runTime;
	TxData[7]= d->direction;
	while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1)==0){};
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
}


void FDCAN_sendChangeTarget_ToMotor(uint8_t destination, uint16_t newTarget, uint16_t transitionTime){
	TxHeader.Identifier =(0xA0D<<16)|(destination<<8)|0x01;
	TxHeader.DataLength = FDCAN_DLC_BYTES_4;
	TxData[0]=newTarget>>8;
	TxData[1]=newTarget;
	TxData[2]=transitionTime>>8;
	TxData[3]=transitionTime;
	while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1)==0){};
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
}

void FDCAN_Recieve_ACKFromMotors(uint8_t sourceAddress){
	ack.motorAcksRecvd |= (1 << (sourceAddress - 2));
	if (ack.motorAcksCheck == ack.motorAcksRecvd){
		ack.waitingForAckResult = 0;
		ack.ackResult = ACK_SUCCESS;
		HAL_TIM_Base_Stop_IT(&htim17);
	}
}

void FDCAN_parseForMotherBoard()
{	uint8_t motorID;
	functionID=((RxHeader.Identifier)&0xFF0000)>>16;
	source_address=(RxHeader.Identifier)&0xFF;
	switch (functionID) {
		case ERROR_FUNCTIONID:
			FDCAN_Recieve_ErrorsFromMotors(source_address);
			break;
		case ACKFRAME_FUNCTIONID:
			FDCAN_Recieve_ACKFromMotors(source_address);
			break;
		case RUNTIMEDATA_FUNCTIONID:
			motorID = GetMotorID_from_CANAddress(source_address);
			FDCAN_Recieve_RunDataFromMotors(functionID,motorID);
			SO_incrementCANCounter(&SO,motorID);
			break;
		case DIAGNOSTICSDONEFRAME_FUNCTIONID: // only stop the can Observer when u get the diag done frame.
			SO_disableAndResetCANObservers(&SO);
			break;
	}
}


void FDCAN_Recieve_RunDataFromMotors(uint8_t functionID,uint8_t motorID){

	/*TRPM = 2 Bytes
	 *PRPM = 2 Bytes
	 *APWMD = 2 Bytes
	 *RUNTIME DF:
	 *FETTEMP = 1 Byte
	 *MOTTEMP = 1 Byte
	 *CURRENT = 2 Bytes Raw Value
	 *VOLTAGE = 2 Bytes Raw Value
	 * ANALYSIS DF:
	 *  ProportionalTerm = 2 Bytes
	 *  IntegralTerm = 2 Bytes
	 *  FeedForward = 2 Bytes
	*/
	if (motorID <= 6){		//
		R[motorID].rdngNo ++;
		R[motorID].targetRPM = (RxData[0]<< 8 ) | RxData[1];
		R[motorID].presentRPM = (RxData[2]<< 8 ) | RxData[3];
		R[motorID].pwm = (RxData[4]<< 8 ) | RxData[5];

		if ((functionID == RUNTIMEDATA_FUNCTIONID)){
			R[motorID].mosfetTemp = RxData[6];
			R[motorID].motorTemp = RxData[7];
			R[motorID].currentRaw=(RxData[8]<< 8 ) | RxData[9];
			R[motorID].voltageRaw=(RxData[10]<< 8 ) | RxData[11];
			R[motorID].currentA = (float)(R[motorID].currentRaw) * MOTORBRD_CURRENT_GAIN ;
			R[motorID].voltageV = (float)(R[motorID].voltageRaw) * MOTORBRD_VOLTAGE_GAIN ;
			R[motorID].power=R[motorID].currentA*R[motorID].voltageV;
		}
		if (functionID == ANALYSISDATA_FUNCTIONID){
			R[motorID].proportionalTerm = (RxData[6]<< 8 ) | RxData[7];
			R[motorID].integralTerm = (RxData[8]<< 8 ) | RxData[9];
			R[motorID].feedForwardTerm = (RxData[10]<< 8 ) | RxData[11];
		}
	}

}

void FDCAN_Recieve_RunDataFromLiftMotors(uint8_t functionID,uint8_t motorID)
{
	R[motorID].rdngNo ++;
	R[motorID].targetPosition=(float)(((RxData[0]<<8)|RxData[1])/100.0);
	R[motorID].presentPosition=(float)(((RxData[2]<<8)|RxData[3])/100.0);
	R[motorID].presentRPM=(RxData[4]<<8)|RxData[5];
	R[motorID].pwm=(RxData[6]<<8)|RxData[7];
	if(functionID==LIFTRUNTIMEDATA_FUNCTIONID){
		R[motorID].mosfetTemp=RxData[8];
		R[motorID].motorTemp=RxData[9];
		R[motorID].currentA=((float)((RxData[10]<< 8 ) | RxData[11]))*MOTORBRD_CURRENT_GAIN;
		R[motorID].voltageV=((float)((RxData[12]<< 8 ) | RxData[13]))*MOTORBRD_VOLTAGE_GAIN;
		R[motorID].power=R[motorID].currentA*R[motorID].voltageV;
	}else if (functionID == ANALYSISDATA_FUNCTIONID){
		R[motorID].proportionalTerm = (RxData[6]<< 8 ) | RxData[7];
		R[motorID].integralTerm = (RxData[8]<< 8 ) | RxData[9];
		R[motorID].feedForwardTerm = (RxData[10]<< 8 ) | RxData[11];
	}

}



void FDCAN_Recieve_ErrorsFromMotors(uint8_t sourceAddress)
{
	uint8_t motorID = GetMotorID_from_CANAddress(sourceAddress);
	uint16_t temp = (RxData[0]<< 8 ) | RxData[1];
	R[motorID].motorError= temp;
	ME.ErrorFlag = 1;
	uint16_t motorErrReason = FindTopMotorError(&ME,temp);
	ME_addErrors(&ME,ERR_MOTOR_SOURCE,motorErrReason,motorID, temp);
}
