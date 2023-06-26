/*
 * EepromSettings.h
 *
 *  Created on: 06-Mar-2023
 *      Author: harsha
 */

#ifndef INC_EEPROMSETTINGS_H_
#define INC_EEPROMSETTINGS_H_

//Addresses
//Dont let a address go across address 32 and its multiples. Thats one page.
#define C_DELIVERY_M_MIN_ADDR 0X02 //float
#define C_DRAFT_ADDR 0X06 // float
#define C_CARDING_CYL_SPEED_ADDR 0x0A // int
#define C_BEATER_CYL_SPEED_ADDR 0x0C // int
#define C_CARDING_FEED_ADDR 0x10 // float
#define C_BEATER_FEED_ADDR 0x14 // float
#define C_TRUNK_DELAY_ADDR 0x18 // int
#define C_LENGTH_LIMIT_M_ADDR 0X1A //int
#define C_RAMP_TIMES_ADDR 0X1C //int
//DEFAULTS
#define DEFAULT_DELIVERY_M_MIN 8
#define DEFAULT_DRAFT 1
#define DEFAULT_CARDING_CYL_SPEED 1250
#define DEFAULT_BEATER_CYL_SPEED 900
#define DEFAULT_CARDING_FEED_SPEED 1.5
#define DEFAULT_BEATER_FEED_SPEED 1.5
#define DEFAULT_TRUNK_DELAY 3
#define DEFAULT_LENGTH_LIMIT 100
#define DEFAULT_RAMP_TIMES 5	// in sec


#endif /* INC_EEPROMSETTINGS_H_ */
