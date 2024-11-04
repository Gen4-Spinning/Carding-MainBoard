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
	if (whichSensor->values.input0 == 1){ // input 0 is duct sensor
		return DUCT_SENSOR;
	}
	return UNKNOWN_SENSOR;
}

void Sensor_resetTriggeredStates(MCP23017_PortB *whichSensor){
	whichSensor->raw = 0 ;
}

int8_t Sensor_GetTriggerValue(MCP23017_HandleTypeDef *mcp, MCP23017_PortB *sensorVal,uint8_t sensor){
	mcp23017_read(mcp, INTCAPB,mcp->intTriggerCapturedValue); // captures GPIO value when interrupt comes.
	sensorVal->raw = mcp->intTriggerCapturedValue[0];
	if (sensor == DUCT_SENSOR){
		return sensorVal->values.input0;
	}
	return -1;
}

int8_t Sensor_ReadValueDirectly(MCP23017_HandleTypeDef *mcp, MCP23017_PortB *sensorVal,uint8_t sensor){
	mcp23017_read(mcp, MCP_GPIOB,mcp->intTriggerCapturedValue); // captures GPIO value when interrupt comes.
	sensorVal->raw = mcp->intTriggerCapturedValue[0];
	if (sensor == DUCT_SENSOR){
		return sensorVal->values.input0;
	}
	return -1;
}
void SetCoilerSensorState(SensorTypeDef *s,uint8_t state){
	s->coilerSensor_activated = state;
}

void DuctSensorMonitor(SensorTypeDef *s,machineSettingsTypeDef *msp){
	if (s->ductSensor != s->ductCurrentState){
			s->ductTimerIncrementBool = 1;
			if (s->ductSensorTimer >= msp->trunkDelay){
				s->ductCurrentState = s->ductSensor;
				s->ductSensorTimer = 0;
			}
		}else{
			s->ductSensorTimer = 0;
			s->ductTimerIncrementBool = 0;
		}
}

// check 2 seconds after a change in state.
uint8_t DuctSensor_CompareDuctStateWithBeaterFeedState(SensorTypeDef *s,RunTime_TypeDef *btrFeedData){
	if (s->ductCurrentState == DUCT_SENSOR_OPEN){
		if (btrFeedData->pwm <= 100){
			// means the btr FEED is not running.
			return 0;
		}else{
			return 1;
		}
	}
	if (s->ductCurrentState == DUCT_SENSOR_CLOSED){
		if (btrFeedData->pwm >= 100){
			// means the btr FEED is running.
			return 0;
		}else{
			return 1;
		}
	}

	return 1;
}

