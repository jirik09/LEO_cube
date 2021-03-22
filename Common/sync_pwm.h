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

#define SYNC_PWM_CHAN_NUM	4

typedef enum{
	CH_DISABLE = 0,
	CH_ENABLE = 1
}syncPwmStateTypeDef;

/* Structs */
/* Timer set in Toggle mode (beside PWM mode 1/2, Asymmetrical mode, ...).
	 This configuration allows a transition from one logic state to another (Output Compare)
	 under the condition CNT == CCRx. Upon first transition the CCRx register is changed 
	 by a new data transfered by DMA. Two dimensional array is needed to define rising edge
	 and falling edge. */
typedef struct{		
	double realPwmFreqCh1;
	double realPwmFreqCh2;

	syncPwmStateTypeDef chan[SYNC_PWM_CHAN_NUM];
	syncPwmStateTypeDef chanInvert[SYNC_PWM_CHAN_NUM];
	syncPwmStateTypeDef stepMode;
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

void syncPwmSetChannelState(uint8_t channel, uint8_t state);
void syncPwmSetChannelInvert(uint8_t channel, uint8_t setInvert);
void syncPwmSetFreqCh1(double freq);
void syncPwmSetFreqCh2(double freq);
void syncPwmSetDutyAndPhase(uint32_t channel, double dutyCycle, uint32_t phase);

void syncPwmSetDefault(void);

#endif /* SYNC_PWM_H_ */

#endif //USE_SYNC_PWM

