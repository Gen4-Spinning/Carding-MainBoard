/*
 * EepromFns.c
 *
 *  Created on: 06-Mar-2023
 *      Author: harsha
 */

#include "Eeprom.h"
#include "machineSettings.h"
#include "machineEepromSettings.h"

void ReadMachineSettingsFromEeprom(machineSettingsTypeDef *m){
	m->delivery_mMin = EE_ReadFloat(C_DELIVERY_M_MIN_ADDR);
	m->draft  = EE_ReadFloat(C_DRAFT_ADDR);
	m->cylinderSpeed = EE_ReadInteger(C_CARDING_CYL_SPEED_ADDR);
	m->beaterSpeed = EE_ReadInteger(C_BEATER_CYL_SPEED_ADDR);
	m->cylinderFeed = EE_ReadFloat(C_CARDING_FEED_ADDR);
	m->beaterFeed  = EE_ReadFloat(C_BEATER_FEED_ADDR);
	m->trunkDelay = EE_ReadInteger(C_TRUNK_DELAY_ADDR);
	m->lengthLimit = EE_ReadInteger(C_LENGTH_LIMIT_M_ADDR);
	m->rampTimes = EE_ReadInteger(C_RAMP_TIMES_ADDR);
}


uint8_t WriteMachineSettingsIntoEeprom(machineSettingsTypeDef *m){
	uint8_t dataWritten = 0;
    dataWritten += EE_WriteFloat(m->delivery_mMin,C_DELIVERY_M_MIN_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteFloat(m->draft,C_DRAFT_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->cylinderSpeed,C_CARDING_CYL_SPEED_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->beaterSpeed,C_BEATER_CYL_SPEED_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteFloat(m->cylinderFeed,C_CARDING_FEED_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteFloat(m->beaterFeed,C_BEATER_FEED_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->trunkDelay,C_TRUNK_DELAY_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->lengthLimit,C_LENGTH_LIMIT_M_ADDR);
	HAL_Delay(2);
	dataWritten += EE_WriteInteger(m->rampTimes,C_RAMP_TIMES_ADDR);
	HAL_Delay(2);

    if (dataWritten == 9)
    	{return 0;}
    else{
    	return 1;}
}


uint8_t CheckMachineSettings(machineSettingsTypeDef* m){
	//typically when something goes wrong with the eeprom you get a value that is very high..
	//to allow for changes place to place without changing this code, we just set the thresholds to  2* maxRange.
	// dont expect in any place the nos to go higher than this..NEED TO PUT LOWER BOUNDS FOR EVERYTHING
	if ((m->delivery_mMin > 15 ) || (m->delivery_mMin < 6)){
		return 0;
	}
	if ((m->draft > 1.50f)||(m->draft < 0.5)){
		return 0;
	}
	if ((m->cylinderSpeed > 1500)||(m->cylinderSpeed < 1200)){
		return 0;
	}

	if ((m->cylinderFeed > 5) || (m->cylinderFeed < 0.1)){
		return 0;
	}
	if ((m->beaterSpeed > 1300) || (m->beaterSpeed < 800)){
		return 0;
	}

	if ((m->beaterFeed > 5) || (m->beaterSpeed < 0.1)){
		return 0;
	}

	if ((m->trunkDelay > 10) || (m->trunkDelay< 1)){
		return 0;
	}

	if ((m->lengthLimit > 300) || (m->lengthLimit < 30)){
		return 0;
	}
	if ((m->rampTimes > 10) || (m->rampTimes < 3)){
		return 0;
	}
	return 1;
}

void LoadDefaultMachineSettings(machineSettingsTypeDef* m){
	m->delivery_mMin = DEFAULT_DELIVERY_M_MIN;
	m->draft = DEFAULT_DRAFT;
	m->cylinderSpeed = DEFAULT_CARDING_CYL_SPEED;
	m->cylinderFeed = DEFAULT_CARDING_FEED_SPEED;
	m->beaterSpeed = DEFAULT_BEATER_CYL_SPEED;
	m->beaterFeed = DEFAULT_BEATER_FEED_SPEED;
	m->trunkDelay = DEFAULT_TRUNK_DELAY;
	m->lengthLimit = DEFAULT_LENGTH_LIMIT;
	m->rampTimes = DEFAULT_RAMP_TIMES;
}
