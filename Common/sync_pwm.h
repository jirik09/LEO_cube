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
#define SYNC_PWM_PI_HALF	90
#define SYNC_PWM_DEF_FREQ	1000
#define SYNC_PWM_DEF_DC 	25

typedef enum{
	STOPPED = 0,
	RUNNING = 1
}syncPwmStopRunState;

typedef enum{
	CH_DISABLE = 0,
	CH_ENABLE = 1
}syncPwmStateTypeDef;

typedef struct{
	float dc;
	float phase;
}syncPwmDcPhase;

/* Structs */
typedef struct{		
	double realPwmFreqCh1;
	double realPwmFreqCh2;

	syncPwmDcPhase chanDcPhase[SYNC_PWM_CHAN_NUM];

	syncPwmStateTypeDef chan[SYNC_PWM_CHAN_NUM];
	syncPwmStateTypeDef chanInvert[SYNC_PWM_CHAN_NUM];
	syncPwmStateTypeDef stepMode;

	syncPwmStopRunState state;
	syncPwmStopRunState prevState;
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
void syncPwmSetFreq(uint32_t channel, double freq);
void syncPwmSetDutyAndPhase(uint32_t channel, double dutyCycle, double phase);

void syncPwmOpmPeriodElapsedCallback(TIM_HandleTypeDef *htim);

void syncPwmSetDefault(void);

#endif /* SYNC_PWM_H_ */

#endif //USE_SYNC_PWM

