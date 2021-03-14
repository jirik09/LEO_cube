/**
  *****************************************************************************
  * @file    sync_pwm.h
  * @author  HeyBirdie
  * @date    Oct 15, 2017
  * @brief   This file contains definitions and prototypes of sync PWM generator 
						 functions.
  ***************************************************************************** 
*/ 

#ifdef USE_SYNC_PWM
#ifndef SYNC_PWM_H_
#define SYNC_PWM_H_

/* Includes */
#include <stdint.h>

/* Enums */
typedef enum{
	SYNC_PWM_CHANNEL1 = 1,
	SYNC_PWM_CHANNEL2 = 2,
	SYNC_PWM_CHANNEL3 = 3,
	SYNC_PWM_CHANNEL4 = 4
}syncPwmChannelTypeDef;

typedef enum{
	CH_DISABLE = 0,
	CH_ENABLE
}syncPwmChannelStateTypeDef;

/* Structs */
/* Timer set in Toggle mode (beside PWM mode 1/2, Asymmetrical mode, ...).
	 This configuration allows a transition from one logic state to another (Output Compare)
	 under the condition CNT == CCRx. Upon first transition the CCRx register is changed 
	 by a new data transfered by DMA. Two dimensional array is needed to define rising edge
	 and falling edge. */
typedef struct{		
	double realPwmFreqCh12;
	double realPwmFreqCh34;
	
	syncPwmChannelTypeDef channelToConfig;

	syncPwmChannelStateTypeDef chan1;
	syncPwmChannelStateTypeDef chan2;
	syncPwmChannelStateTypeDef chan3;
	syncPwmChannelStateTypeDef chan4;
	syncPwmChannelStateTypeDef stepMode;
}syncPwmTypeDef;

// Externs ===========================================================
extern syncPwmTypeDef syncPwm;

// Functions Prototypes ==============================================
void SyncPwmTask(void const *argument);

void syncPwmSendInit(void);
void syncPwmSendDeinit(void);
void syncPwmSendStart(void);
void syncPwmSendStop(void);

void syncPwmSetStepMode(void);	
void syncPwmResetStepMode(void);

void syncPwmInit(void);
void syncPwmDeinit(void);
void syncPwmStart(void);
void syncPwmStop(void);

void syncPwmChannelNumber(uint8_t chanNum);
void syncPwmSetChannelState(uint8_t channel, uint8_t state);
void syncPwmSetFreqCh12(double freq);
void syncPwmSetFreqCh34(double freq);

void syncPwmSetDefault(void);

#endif /* SYNC_PWM_H_ */

#endif //USE_SYNC_PWM

