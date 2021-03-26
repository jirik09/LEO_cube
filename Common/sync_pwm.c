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
	syncPwm.state = RUNNING;
}	

void syncPwmStop(void){
	TIM_SYNC_PWM_Stop();
	syncPwm.state = STOPPED;
}	

void syncPwmSetFreq(uint32_t channel, double freq){
//	syncPwm.prevState = syncPwm.state;
//	if (syncPwm.state == RUNNING)
//		syncPwmStop();

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

//	if(syncPwm.prevState == RUNNING)
//		syncPwmStart();

	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
}

void syncPwmSetDutyAndPhase(uint32_t channel, double dutyCycle, double phase){
//	syncPwm.prevState = syncPwm.state;
//	if (syncPwm.state == RUNNING)
//		syncPwmStop();

	TIM_SYNC_PWM_SetChanDutyPhase(channel, dutyCycle, phase);

//	if(syncPwm.prevState == RUNNING)
//		syncPwmStart();
}

void syncPwmSetChannelState(uint8_t channel, uint8_t state){
//	syncPwm.prevState = syncPwm.state;
//	if (syncPwm.state == RUNNING)
//		syncPwmStop();

	TIM_SYNC_PWM_ChannelState(channel, state);

//	if(syncPwm.prevState == RUNNING)
//		syncPwmStart();
}

void syncPwmSetChannelInvert(uint8_t channel, uint8_t setInvert){
	TIM_SYNC_PWM_SetChanInvert(channel, setInvert);
	syncPwm.chanInvert[channel] = setInvert;
}

void syncPwmSetStepMode(void){
	if (syncPwm.state == RUNNING)
		TIM_SYNC_PWM_Stop();

	TIM_SYNC_PWM_StepMode_Enable();
	syncPwm.stepMode = CH_ENABLE;
	syncPwm.state = STOPPED;
}

void syncPwmResetStepMode(void){
	if (syncPwm.state == RUNNING)
		TIM_SYNC_PWM_Stop();

	TIM_SYNC_PWM_StepMode_Disable();
	syncPwm.stepMode = CH_DISABLE;
	syncPwm.state = STOPPED;
}

void syncPwmOpmPeriodElapsedCallback(TIM_HandleTypeDef *htim){
	uint16_t passMsg = MSG_SYNCPWM_OPM_PERIOD_ELAPSED;
	portBASE_TYPE xHigherPriorityTaskWoken;
	xQueueSendToBackFromISR(messageQueue, &passMsg, &xHigherPriorityTaskWoken);
}

void syncPwmSetDefault(void){
	syncPwm.prevState = STOPPED;
	syncPwm.state = STOPPED;
	syncPwm.realPwmFreqCh1 = 1;
	syncPwm.realPwmFreqCh2 = 1;
	for(int i = 0; i < SYNC_PWM_CHAN_NUM; i++){
		syncPwm.chan[i] = CH_ENABLE;
		syncPwm.chanInvert[i] = CH_DISABLE;
	}
}

#endif //USE_SYNC_PWM

