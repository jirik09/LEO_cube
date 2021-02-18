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

volatile syncPwmTypeDef syncPwm;

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
void syncPwmChannelNumber(uint8_t chanNum)
{	
	syncPwm.channelToConfig = (syncPwmChannelTypeDef)chanNum;
}

/* Set two DMA transfers to transfer the required data to CCR1 register
	 upon DMA Output Compare event. */
void syncPwmChannelConfig(uint32_t ccr1st, uint16_t ccr2nd)
{	
	TIM_SYNC_PWM_DMA_ChanConfig(ccr1st, ccr2nd);
}

/* Frequency reconfiguring. */
double syncPwmSetFreq(double freq)
{
	syncPwm.realPwmFreq =  TIM_Reconfig_SyncPwm(freq);
	uint16_t passMsg = MSG_SYNCPWM_REAL_FREQ;
	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
	return syncPwm.realPwmFreq;
}

void syncPwmSetChannelState(uint8_t channel, uint8_t state)
{
	TIM_SYNC_PWM_ChannelState(channel, state);
}

void syncPwmSetStepMode(void)
{
	TIM_SYNC_PWM_StepMode_Enable();
}

void syncPwmResetStepMode(void)
{
	TIM_SYNC_PWM_StepMode_Disable();
}

void syncPwmSetDefault(void)
{
	/* Four channels to generate by default. */
	syncPwm.chan1 = CHAN_ENABLE;
	syncPwm.chan2 = CHAN_ENABLE;
	syncPwm.chan3 = CHAN_ENABLE;
	syncPwm.chan4 = CHAN_ENABLE;

	/* Default 4 channels equidistant 90� and 25% duty cycle settings. */
	syncPwm.dataEdgeChan1[0] = 3600;
	syncPwm.dataEdgeChan1[1] = 0;
	syncPwm.dataEdgeChan2[0] = 7200;
	syncPwm.dataEdgeChan2[1] = 3600;			
	syncPwm.dataEdgeChan3[0] = 10400;
	syncPwm.dataEdgeChan3[1] = 7200;			
	syncPwm.dataEdgeChan4[0] = 14000;
	syncPwm.dataEdgeChan4[1] = 10400;		
}

#endif //USE_SYNC_PWM

