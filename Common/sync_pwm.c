/**
 *****************************************************************************
 * @file    sync_pwm.c
 * @author  HeyBirdie
 * @date    Oct 15, 2017
 * @brief   This file contains sync PWM generator functions.
 *****************************************************************************
 */

// Includes ===================================================================
#ifdef USE_SYNC_PWM

#include "cmsis_os.h"
#include "mcu_config.h"
#include "comms.h"
#include "sync_pwm.h"
#include "tim.h"
#include "messages.h"

// External variables definitions =============================================
xQueueHandle syncPwmMessageQueue;
xSemaphoreHandle syncPwmMutex;

syncPwmTypeDef syncPwm;

// Function definitions =======================================================
/**
 * @brief  Generator of synchronized PWM channels task function.
 * task is getting messages from other tasks and takes care about counter functions
 * @param  Task handler, parameters pointer
 * @retval None
 */
void SyncPwmTask(void const *argument)
{
	uint16_t message = 0xFFFF;
	syncPwmMessageQueue = xQueueCreate(5, sizeof(message)/sizeof(uint8_t));
	syncPwmMutex = xSemaphoreCreateRecursiveMutex();	

	if(syncPwmMessageQueue == 0){
		while(1); // Queue was not created and must not be used.
	}

	syncPwmSetDefault();

	while(1){

		xQueueReceive(syncPwmMessageQueue, &message, portMAX_DELAY);
		xSemaphoreTakeRecursive(syncPwmMutex, portMAX_DELAY);

		switch(message){
		case MSG_SYNCPWM_INIT:
			syncPwmInit();
			break;
		case MSG_SYNCPWM_DEINIT:
			syncPwmDeinit();
			break;
		case MSG_SYNCPWM_START:
			syncPwmStart();
			break;
		case MSG_SYNCPWM_STOP:
			syncPwmStop();
			break;
		default:
			break;
		}

		xSemaphoreGiveRecursive(syncPwmMutex);
	}
}

/* ************************************************************************************** */
/* -------------------- Sync PWM generator basic settings via queue --------------------- */
/* ************************************************************************************** */
void syncPwmSendInit(void){
	uint16_t passMsg = MSG_SYNCPWM_INIT;
	xQueueSendToBack(syncPwmMessageQueue, &passMsg, portMAX_DELAY);
}

void syncPwmSendDeinit(void){
	uint16_t passMsg = MSG_SYNCPWM_DEINIT;
	xQueueSendToBack(syncPwmMessageQueue, &passMsg, portMAX_DELAY);
}

void syncPwmSendStart(void){
	uint16_t passMsg = MSG_SYNCPWM_START;
	xQueueSendToBack(syncPwmMessageQueue, &passMsg, portMAX_DELAY);
}

void syncPwmSendStop(void){
	uint16_t passMsg = MSG_SYNCPWM_STOP;
	xQueueSendToBack(syncPwmMessageQueue, &passMsg, portMAX_DELAY);
}


/* ************************************************************************************** */
/* ------------------------- Sync PWM generator basic settings -------------------------- */
/* ************************************************************************************** */
void syncPwmInit(void){
	TIM_SYNC_PWM_Init();
}	

void syncPwmDeinit(void){
	TIM_SYNC_PWM_Deinit();
}	

void syncPwmStart(void){
	TIM_SYNC_PWM_Start();
}	

void syncPwmStop(void){
	TIM_SYNC_PWM_Stop();
}	

void syncPwmSetFreq(uint32_t channel, double freq){
	uint16_t passMsg;
	switch(channel){
	case 0:
		syncPwm.realPwmFreqCh1 = TIM_Reconfig_SyncPwm_Ch1(freq);
		passMsg = MSG_SYNCPWM_REAL_FREQ_CH1;
		break;
	case 1:
		syncPwm.realPwmFreqCh2 = TIM_Reconfig_SyncPwm_Ch2(freq);
		passMsg = MSG_SYNCPWM_REAL_FREQ_CH2;
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}

	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
}

void syncPwmSetDutyAndPhase(uint32_t channel, double dutyCycle, double phase){
	TIM_SYNC_PWM_SetChanDutyPhase(channel, dutyCycle, phase);
}

void syncPwmSetChannelState(uint8_t channel, uint8_t state){
	TIM_SYNC_PWM_ChannelState(channel, state);
}

void syncPwmSetChannelInvert(uint8_t channel, uint8_t setInvert){
	TIM_SYNC_PWM_SetChanInvert(channel, setInvert);
	syncPwm.chanInvert[channel] = setInvert;
}

void syncPwmSetStepMode(void){
	TIM_SYNC_PWM_Stop();
	TIM_SYNC_PWM_StepMode_Enable();
}

void syncPwmResetStepMode(void){
	TIM_SYNC_PWM_Stop();
	TIM_SYNC_PWM_StepMode_Disable();
}

void syncPwmOpmPeriodElapsedCallback(TIM_HandleTypeDef *htim){
	uint16_t passMsg = MSG_SYNCPWM_OPM_PERIOD_ELAPSED;
	portBASE_TYPE xHigherPriorityTaskWoken;
	xQueueSendToBackFromISR(messageQueue, &passMsg, &xHigherPriorityTaskWoken);
}

void syncPwmSetDefault(void){
	syncPwm.prevState = STOPPED;
	syncPwm.state = STOPPED;
	syncPwm.realPwmFreqCh1 = SYNC_PWM_DEF_FREQ;
	syncPwm.realPwmFreqCh2 = SYNC_PWM_DEF_FREQ;
	float phase = 0;
	for(int chan = 0; chan < SYNC_PWM_CHAN_NUM; chan++){
		syncPwm.chan[chan] = CH_ENABLE;
		syncPwm.chanInvert[chan] = CH_DISABLE;
		syncPwm.chanDcPhase[chan].phase = phase;
		syncPwm.chanDcPhase[chan].dc = SYNC_PWM_DEF_DC;
		phase += SYNC_PWM_PI_HALF;
	}
}

#endif //USE_SYNC_PWM

