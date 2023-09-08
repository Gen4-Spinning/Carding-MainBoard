/*
 * machineSettings.h
 *
 *  Created on: Apr 11, 2023
 *      Author: harsha
 */

#ifndef MACHINESETTINGS_H_
#define MACHINESETTINGS_H_

#include "stdio.h"
#include "FDCAN.h"
#include "Struct.h"

//all Structs with arrays have to follow this Form
#define CARDING_CYLINDER 0
#define BEATER_CYLINDER 1
#define CAGE 2
#define CARDING_FEED 3
#define BEATER_FEED 4
#define COILER 5

#define CYLINDER_GEAR_RATIO 1.17 // changed from 1.20 after measuring on machine
#define BEATER_GEAR_RATIO 1.17
#define BEATER_FEED_GB 180
#define CYLINDER_FEED_GB 180
#define TONGUE_GROOVE_CIRCUMFERENCE_MM 213.63
#define CAGE_GB 6.91
#define COILER_GROOVE_CIRCUMFERENCE_MM 194.779
#define COILER_GROOVE_TO_GB_RATIO 1.656
#define COILER_GB 6.91

#define CYLINDERS_RAMP_TIME_SEC 75

typedef struct machineSettings_Struct{
	float delivery_mMin;
	float draft;
	uint16_t cylinderSpeed;
	float cylinderFeed ;
	uint16_t beaterSpeed ;
	float beaterFeed ;
	uint16_t trunkDelay ;
	uint16_t lengthLimit;
    int rampTimes;	// only for non cylinder Motors
}machineSettingsTypeDef;

typedef struct machineParamaters_Struct{
    uint16_t cylinderRPM;
    uint16_t beaterRPM;
    uint16_t beaterFeedRPM;
    uint16_t cylinderFeedRPM;
    uint16_t cageRPM;
    uint16_t coilerRPM;

    float cageGB_RPM; // maybe we can measure and see if this is right
    float coilerGB_rpm;

    float allMotorsOn;

	float currentMtrsRun;
	float totalPower;

}machineParamsTypeDef;


extern machineSettingsTypeDef msp;
extern machineSettingsTypeDef ps;
extern machineSettingsTypeDef msp_BT;
extern machineParamsTypeDef mcParams;

// Eeprom MachineSettings
void ReadMachineSettingsFromEeprom(machineSettingsTypeDef *m);
uint8_t WriteMachineSettingsIntoEeprom(machineSettingsTypeDef *m);
uint8_t CheckMachineSettings(machineSettingsTypeDef* m);
void LoadDefaultMachineSettings(machineSettingsTypeDef* m);

//otherFns
void InitializeMachineSettings(machineSettingsTypeDef *ms);
void CalculateMachineParameters(machineSettingsTypeDef *ms,machineParamsTypeDef *mp);
void UpdateMachineParameters(machineSettingsTypeDef *ms,machineParamsTypeDef *m);

uint8_t getMotorCANAddress(uint8_t motor);
uint8_t GetMotorID_from_CANAddress(uint8_t canAddress);

uint8_t CheckCylindersRampUpOver(machineParamsTypeDef *mcp,RunTime_TypeDef *cylinder,RunTime_TypeDef *beater);

void InitializePiecingSettings(machineSettingsTypeDef *ps,machineSettingsTypeDef *ms);

#endif /* MACHINESETTINGS_H_ */
