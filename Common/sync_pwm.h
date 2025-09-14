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
	double realPwmFreqCh3;
	double realPwmFreqCh4;

	syncPwmDcPhase chanDcPhase[SYNC_PWM_CHAN_NUM];

	syncPwmStateTypeDef chan[SYNC_PWM_CHAN_NUM];
	syncPwmStateTypeDef chanInvert[SYNC_PWM_CHAN_NUM];
	syncPwmStateTypeDef stepMode;

	syncPwmStopRunState state;
	/* ------------------------------------------------------------------ */
	/* Legacy compatibility fields (older sync PWM implementation)        */
	/* These are accessed by tim_sync_pwm.c in some MCU variants (e.g.    */
	/* G474 project) which still use per-channel explicit members and    */
	/* DMA edge buffers. They are redundant with the newer array-based   */
	/* representation but kept to avoid touching the timer driver logic. */
	/* ------------------------------------------------------------------ */
	uint16_t dataEdgeChan1[2];
	uint16_t dataEdgeChan2[2];
	uint16_t dataEdgeChan3[2];
	uint16_t dataEdgeChan4[2];
	uint32_t timAutoReloadReg;   /* mirror of ARR */
	uint32_t timPrescReg;        /* mirror of PSC */
	uint8_t  channelToConfig;    /* channel index 1..4 for DMA config switch */
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

/* ---------------------------------------------------------------------- */
/* Legacy name compatibility macros (older code uses these identifiers).  */
/* ---------------------------------------------------------------------- */
#define chan1 chan[0]
#define chan2 chan[1]
#define chan3 chan[2]
#define chan4 chan[3]
#ifndef CHAN_ENABLE
#define CHAN_ENABLE   CH_ENABLE
#endif
#ifndef CHAN_DISABLE
#define CHAN_DISABLE  CH_DISABLE
#endif

/* ------------------------------------------------------------------ */
/* Legacy function naming compatibility (older underscore variants)   */
/* To prevent macro expansion from colliding with actual function     */
/* definitions inside implementation units, suppress these mappings   */
/* when SYNC_PWM_IMPLEMENTATION is defined before including header.   */
/* ------------------------------------------------------------------ */

/* Auto-detect legacy (F303) build: if STM32F303xE (or similar) defined, mark legacy API */
#if defined(STM32F303xE) || defined(STM32F303xC) || defined(STM32F303x8)
#ifndef SYNC_PWM_LEGACY_API
#define SYNC_PWM_LEGACY_API
#endif
#endif

/* Strategy:
 * F303 legacy implementation provides concrete functions:
 *   TIM_SYNC_PWM_SetFreqCh1, TIM_SYNC_PWM_SetFreqCh2,
 *   TIM_SYNC_PWM_SetChanDutyPhase, TIM_SYNC_PWM_SetChanInvert.
 * G474 (and future) provide generic channel-based versions instead:
 *   TIM_SYNC_PWM_SetFreqChannel, TIM_SYNC_PWM_SetChannelDutyPhase, TIM_SYNC_PWM_SetChannelInvert.
 * We only supply macro fallbacks mapping the legacy names to generic ones
 * when the legacy per-channel prototypes are NOT already present.
 */

/* If not legacy, declare the generic prototypes (implemented in MCU variant sources) */
#if !defined(SYNC_PWM_LEGACY_API)
double TIM_SYNC_PWM_SetFreqChannel(uint8_t channel, double freq);
void   TIM_SYNC_PWM_SetChannelDutyPhase(uint32_t channel, double dutyCycle, double phase);
void   TIM_SYNC_PWM_SetChannelInvert(uint8_t channel, uint8_t setInvert);

	/* If a build system (or a legacy header like tim.h) sets SYNC_PWM_LEGACY_API
		 we assume per-channel functions exist and skip remapping. */
	#ifndef TIM_SYNC_PWM_SetFreqCh1
		#define TIM_SYNC_PWM_SetFreqCh1(freq) TIM_SYNC_PWM_SetFreqChannel(0,(freq))
	#endif
	#ifndef TIM_SYNC_PWM_SetFreqCh2
		#define TIM_SYNC_PWM_SetFreqCh2(freq) TIM_SYNC_PWM_SetFreqChannel(1,(freq))
	#endif
	#ifndef TIM_SYNC_PWM_SetChanDutyPhase
		#define TIM_SYNC_PWM_SetChanDutyPhase(ch,duty,phase) TIM_SYNC_PWM_SetChannelDutyPhase((ch),(duty),(phase))
	#endif
	#ifndef TIM_SYNC_PWM_SetChanInvert
		#define TIM_SYNC_PWM_SetChanInvert(ch,setInv) TIM_SYNC_PWM_SetChannelInvert((ch),(setInv))
	#endif
#endif /* !SYNC_PWM_LEGACY_API */

#ifndef SYNC_PWM_CHANNEL1
#define SYNC_PWM_CHANNEL1 1
#define SYNC_PWM_CHANNEL2 2
#define SYNC_PWM_CHANNEL3 3
#define SYNC_PWM_CHANNEL4 4
#endif

/* Map old driver function name used in common layer to current one */
#ifndef TIM_SYNC_PWM_ChannelEnable
#define TIM_SYNC_PWM_ChannelEnable TIM_SYNC_PWM_ChannelState
#endif

#endif /* SYNC_PWM_H_ */

#endif //USE_SYNC_PWM

