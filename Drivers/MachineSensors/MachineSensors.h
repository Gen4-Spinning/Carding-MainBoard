/*
 * MachineSensors.h
 *
 *  Created on: May 11, 2023
 *      Author: harsha
 */

#ifndef MACHINESENSORS_H_
#define MACHINESENSORS_H_

#include "mcp23017.h"
#include "machineSettings.h"

#define UNKNOWN_SENSOR 0
#define CREEL_SLIVER_CUT_SENSOR 1
#define LAPPING_SENSOR 2
#define COILER_SENSOR 3
#define DUCT_SENSOR 4

#define DUCT_SENSOR_OPEN 1
#define DUCT_SENSOR_CLOSED 0

#define SENSOR_ENABLE 1
#define SENSOR_DISABLE 2

typedef struct {
	int8_t ductSensor;
	int8_t ductCurrentState;
	uint8_t ductTimerIncrementBool;
	uint8_t ductSensorTimer;
	uint8_t ductSensorOneShot;
	uint8_t ductSensorClosedTimer;

	uint8_t coilerSensor_activated;
	uint8_t coilerSensor;
	uint8_t latchedCoilerSensor;

} SensorTypeDef;

extern SensorTypeDef sensor;

uint8_t Sensor_whichTriggered(MCP23017_HandleTypeDef *mcp, MCP23017_PortB *whichSensor);
void Sensor_resetTriggeredStates(MCP23017_PortB *whichSensor);
int8_t Sensor_GetTriggerValue(MCP23017_HandleTypeDef *mcp, MCP23017_PortB *sensorVal,uint8_t sensor);
int8_t Sensor_ReadValueDirectly(MCP23017_HandleTypeDef *mcp, MCP23017_PortB *sensorVal,uint8_t sensor);

void SetCoilerSensorState(SensorTypeDef *s,uint8_t state);

void DuctSensorMonitor(SensorTypeDef *s,machineSettingsTypeDef *msp);

#endif /* MACHINESENSORS_H_ */
