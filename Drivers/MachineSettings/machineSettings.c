/*
 * machineSettings.c
 *
 *  Created on: Apr 11, 2023
 *      Author: harsha
 */

#include "machineSettings.h"


void InitializeMachineSettings(machineSettingsTypeDef *ms){
	ms->delivery_mMin = 6;
	ms->draft = 1.0;
	ms->cylinderSpeed = 1500;
	ms->cylinderFeed = 3.5;
	ms->beaterSpeed = 950;
	ms->beaterFeed = 2.5;
	ms->trunkDelay = 3;
	ms->lengthLimit = 1500;
	ms->rampTimes = 3;
}

void InitializePiecingSettings(machineSettingsTypeDef *ps,machineSettingsTypeDef *ms){
	ps->delivery_mMin = 6;
	ps->draft = ms->draft;
	ps->cylinderSpeed = ms->cylinderSpeed;
	ps->cylinderFeed = 1.5;
	ps->beaterSpeed = ms->beaterSpeed ;
	ps->beaterFeed = 3.5;
	ps->trunkDelay = ms->trunkDelay;
	ps->lengthLimit = ms->lengthLimit;
	ps->rampTimes = ms->rampTimes;
}

void CalculateMachineParameters(machineSettingsTypeDef *ms,machineParamsTypeDef *m){
	m->cylinderRPM = ms->cylinderSpeed/CYLINDER_GEAR_RATIO;
	m->beaterRPM = ms->beaterSpeed/BEATER_GEAR_RATIO;
	m->cylinderFeedRPM = ms->cylinderFeed * CYLINDER_FEED_GB;
	m->beaterFeedRPM = ms->beaterFeed * BEATER_FEED_GB;
	m->cageGB_RPM = (ms->delivery_mMin*1000)/TONGUE_GROOVE_CIRCUMFERENCE_MM;
	m->cageRPM = m->cageGB_RPM * CAGE_GB;
	float req_coiler_tongue_surfaceSpeed_mm = (ms->delivery_mMin*1000) * ms->draft;
	float req_coiler_tongueRPM = req_coiler_tongue_surfaceSpeed_mm/COILER_GROOVE_CIRCUMFERENCE_MM;
	m->coilerGB_rpm = req_coiler_tongueRPM/COILER_GROOVE_TO_GB_RATIO;
	m->coilerRPM = m->coilerGB_rpm * COILER_GB;
}

void UpdateMachineParameters(machineSettingsTypeDef *ms,machineParamsTypeDef *m){
	m->cylinderFeedRPM = ms->cylinderFeed * CYLINDER_FEED_GB;
	m->beaterFeedRPM = ms->beaterFeed * BEATER_FEED_GB;
	m->cageGB_RPM = (ms->delivery_mMin*1000)/TONGUE_GROOVE_CIRCUMFERENCE_MM;
	m->cageRPM = m->cageGB_RPM * CAGE_GB;
	float req_coiler_tongue_surfaceSpeed_mm = (ms->delivery_mMin*1000) * ms->draft;
	float req_coiler_tongueRPM = req_coiler_tongue_surfaceSpeed_mm/COILER_GROOVE_CIRCUMFERENCE_MM;
	m->coilerGB_rpm = req_coiler_tongueRPM/COILER_GROOVE_TO_GB_RATIO;
	m->coilerRPM = m->coilerGB_rpm * COILER_GB;
}


uint8_t getMotorCANAddress(uint8_t motor){
	if (motor == CARDING_CYLINDER){
		return 2;
	}else if (motor == BEATER_CYLINDER){
		return 3;
	}else if (motor == CAGE){
		return 4;
	}else if (motor == CARDING_FEED){
		return 5;
	}else if (motor == BEATER_FEED){
		return 6;
	}else if (motor == COILER){
		return 7;
	}else{
		return 0;
	}
}

uint8_t GetMotorID_from_CANAddress(uint8_t canAddress){
	if (canAddress == CARDING_CYLINDER_ADDRESS){
		return CARDING_CYLINDER;
	}else if (canAddress == BEATER_CYLINDER_ADDRESS){
		return BEATER_CYLINDER;
	}else if (canAddress == CAGE_ADDRESS){
		return CAGE;
	}else if (canAddress == CARDING_FEED_ADDRESS){
		return CARDING_FEED;
	}else if (canAddress == BEATER_FEED_ADDRESS){
		return BEATER_FEED;
	}else if (canAddress == COILER_ADDRESS){
		return COILER;
	}
	return 6; // return
}


uint8_t CheckCylindersRampUpOver(machineParamsTypeDef *mcp,RunTime_TypeDef *cylinder,RunTime_TypeDef *beater){
	if (cylinder->presentRPM >= (mcp->cylinderRPM - 20)){
		if (beater->presentRPM >= (mcp->beaterRPM - 20)){
			return 1;
		}
	}
	return 0;
}

