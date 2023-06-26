/*
 * MachineSensors.c
 *
 *  Created on: May 11, 2023
 *      Author: harsha
 */


#include "MachineSensors.h"

uint8_t Sensor_whichTriggered(MCP23017_HandleTypeDef *mcp, MCP23017_PortB *whichSensor){
	mcp23017_read(mcp, INTFB,mcp->intTrigger);// intFB gives which pin has interrupt
	whichSensor->raw = mcp->intTrigger[0];
	if (whichSensor->values.input1 == 1){
		return CREEL_SLIVER_CUT_SENSOR;
	}
	return UNKNOWN_SENSOR;
}


void Sensor_resetTriggeredStates(MCP23017_PortB *whichSensor){
	whichSensor->raw = 0 ;
}

int8_t Sensor_GetTriggerValue(MCP23017_HandleTypeDef *mcp, MCP23017_PortB *sensorVal,uint8_t sensor){
	mcp23017_read(mcp, INTCAPB,mcp->intTriggerCapturedValue); // captures GPIO value when interrupt comes.
	sensorVal->raw = mcp->intTriggerCapturedValue[0];
	if (sensor == CREEL_SLIVER_CUT_SENSOR){
		return sensorVal->values.input1;
	}
	return -1;
}

void SetCoilerSensorState(SensorTypeDef *s,uint8_t state){
	s->coilerSensor_activated = state;
}
