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

/* The received number determines what channel needs to be configured. */
void syncPwmChannelNumber(uint8_t chanNum){
	syncPwm.channelToConfig = (syncPwmChannelTypeDef)chanNum;
}

/* Frequency reconfig */
void syncPwmSetFreqCh12(double freq){
	syncPwm.realPwmFreqCh12 = TIM_Reconfig_SyncPwm_Ch12(freq);
	uint16_t passMsg = MSG_SYNCPWM_REAL_FREQ_CH12;
	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
}

void syncPwmSetFreqCh34(double freq){
	syncPwm.realPwmFreqCh34 = TIM_Reconfig_SyncPwm_Ch34(freq);
	uint16_t passMsg = MSG_SYNCPWM_REAL_FREQ_CH34;
	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
}

void syncPwmSetChannelState(uint8_t channel, uint8_t state){
	TIM_SYNC_PWM_ChannelState(channel, state);
}

void syncPwmSetStepMode(void){
	TIM_SYNC_PWM_StepMode_Enable();
}

void syncPwmResetStepMode(void){
	TIM_SYNC_PWM_StepMode_Disable();
}

void syncPwmSetDefault(void){
	syncPwm.chan1 = CH_ENABLE;
	syncPwm.chan2 = CH_ENABLE;
	syncPwm.chan3 = CH_ENABLE;
	syncPwm.chan4 = CH_ENABLE;
}

#endif //USE_SYNC_PWM

