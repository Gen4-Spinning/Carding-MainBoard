/*
 * BT_Machine.c
 *
 *  Created on: 25-Apr-2023
 *      Author: harsha
 */

#include "BT_Machine.h"


uint8_t BT_MC_generateSettingsMsg(machineSettingsTypeDef *m){
	  char TLV_Buffer[12];
	  uint8_t tlvSize = 0;
	  uint8_t eof_size  = 0;
	  uint8_t initLength = 0;

	  initLength = Init_TXBuf_Frame(SETTINGS_FROM_MC,SUBSTATE_NA,12);

	  generateTLV_F(TLV_Buffer,DELIVERY_M_MIN_BT,m->delivery_mMin);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_F(TLV_Buffer,DRAFT_BT,m->draft);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_I(TLV_Buffer,CARDING_CYL_SPEED_BT,m->cylinderSpeed);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_I(TLV_Buffer,BEATER_CYL_SPEED_BT,m->beaterSpeed);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_F(TLV_Buffer,CARDING_FEED_SPEED_BT,m->cylinderFeed);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_F(TLV_Buffer,BEATER_FEED_SPEED_BT,m->beaterFeed);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_I(TLV_Buffer,TRUNK_DELAY_BT,m->trunkDelay);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_I(TLV_Buffer,LENGTH_LIMIT_BT,m->lengthLimit);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_I(TLV_Buffer,RAMPTIMES_BT,m->rampTimes);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  eof_size = addEOF(initLength+tlvSize);
	  correctLengthInFrame(initLength,tlvSize,eof_size);

	  return initLength + tlvSize + eof_size;

}

//FLYER
uint8_t BT_MC_parse_Settings(machineSettingsTypeDef *mspBT){
	//Buffer Rec index 10 onwards is TLVs till 10 + TlvsLength
	TLVStruct_TypeDef T;
	uint8_t TLV_start = 10;
	uint8_t tlvSize = 0;
	uint8_t count = 0;
	uint8_t allSettingsRecieved = 0;

    for (int i=0;i<BT.attributeCnt;i++){
    	tlvSize = parseTLV(&T,TLV_start);
    	TLV_start += tlvSize;
    	switch (T.type){
    		case DELIVERY_M_MIN_BT:
    			mspBT->delivery_mMin = T.value_f;
    			count += 1;
    			break;
    		case DRAFT_BT:
    			mspBT->draft = T.value_f;
    			count += 1;
    			break;
    		case CARDING_CYL_SPEED_BT:
    			mspBT->cylinderSpeed = T.value_int;
    			count += 1;
    			break;
    		case BEATER_CYL_SPEED_BT:
    			mspBT->beaterSpeed = T.value_int;
    			count += 1;
    			break;
    		case CARDING_FEED_SPEED_BT:
    			mspBT->cylinderFeed = T.value_f;
    			count += 1;
    			break;
    		case BEATER_FEED_SPEED_BT:
    			mspBT->beaterFeed = T.value_f;
    			count += 1;
    			break;
       		case TRUNK_DELAY_BT:
				mspBT->trunkDelay = T.value_int;
				count += 1;
				break;
    		case LENGTH_LIMIT_BT:
    			mspBT->lengthLimit = T.value_int;
    			count += 1;
    			break;
    		case RAMPTIMES_BT:
    			mspBT->rampTimes = T.value_int;
    			count += 1;
    			break;
    	}
    }
    if (count == 9){
    	allSettingsRecieved = 1;
    }

    return allSettingsRecieved;
}

//FLYER
uint8_t BT_MC_Save_Settings(void){
	uint8_t fail;
	fail = WriteMachineSettingsIntoEeprom(&msp_BT);
	if (fail == 0){
		msp.delivery_mMin = msp_BT.delivery_mMin;
		msp.draft = msp_BT.draft;
		msp.cylinderSpeed = msp_BT.cylinderSpeed;
		msp.cylinderFeed = msp_BT.cylinderFeed;
		msp.beaterSpeed = msp_BT.beaterSpeed;
		msp.beaterFeed = msp_BT.beaterFeed;
		msp.trunkDelay = msp_BT.trunkDelay;
		msp.lengthLimit = msp_BT.lengthLimit;
		msp.rampTimes = msp_BT.rampTimes;
		//send success msg to APP
	}
	return !fail;
}

uint8_t BT_MC_Update_Settings(void){
	msp.delivery_mMin = msp_BT.delivery_mMin;
	msp.draft = msp_BT.draft;
	msp.cylinderSpeed = msp_BT.cylinderSpeed;
	msp.cylinderFeed = msp_BT.cylinderFeed;
	msp.beaterSpeed = msp_BT.beaterSpeed;
	msp.beaterFeed = msp_BT.beaterFeed;
	msp.trunkDelay = msp_BT.trunkDelay;
	msp.lengthLimit = msp_BT.lengthLimit;
	msp.rampTimes = msp_BT.rampTimes;
	//send success msg to APP
	return 1;
}

uint8_t GetMotorID_from_BTMotor_ID(uint8_t BT_motorID){
	if (BT_motorID == BT_CARDING_CYLINDER){
		return CARDING_CYLINDER;
	}else if (BT_motorID == BT_BEATER_CYLINDER){
		return BEATER_CYLINDER;
	}else if (BT_motorID == BT_CAGE){
		return CAGE;
	}else if (BT_motorID == BT_CARDING_FEED){
		return CARDING_FEED;
	}else if (BT_motorID == BT_BEATER_FEED){
		return BEATER_FEED;
	}else if (BT_motorID == BT_COILER){
		return COILER;
	}
	return 0;
}

uint8_t GetBTMotorID_from_Motor_ID(uint8_t motorID){
	if (motorID == CARDING_CYLINDER){
		return BT_CARDING_CYLINDER;
	}else if (motorID == BEATER_CYLINDER){
		return BT_BEATER_CYLINDER;
	}else if (motorID == CAGE){
		return BT_CAGE;
	}else if (motorID == CARDING_FEED){
		return BT_CARDING_FEED;
	}else if (motorID == BEATER_FEED){
		return BT_BEATER_FEED;
	}else if (motorID == COILER){
		return BT_COILER;
	}
	return 0;
}

uint8_t GetMotorId_from_CarousalID(uint8_t carousalID){
	if (carousalID == BT_CARDING_CYLINDER){
		return CARDING_CYLINDER;
	}else if (carousalID == BT_BEATER_CYLINDER){
		return BEATER_CYLINDER;
	}else if (carousalID == BT_CAGE){
		return CAGE;
	}else if (carousalID == BT_CARDING_FEED){
		return CARDING_FEED;
	}else if (carousalID == BT_BEATER_FEED){
		return BEATER_FEED;
	}else if (carousalID == BT_COILER){
		return COILER;
	}
	return 99;
}

