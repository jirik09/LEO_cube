/**
 *****************************************************************************
 * @file    logic_analyzer.c
 * @author  HeyBirdie
 * @date    Nov 4, 2017
 * @brief   This file contains logic analyzer functions.
 *****************************************************************************
 */

// Includes ===================================================================
#ifdef USE_LOG_ANLYS
#include "cmsis_os.h"
#include "mcu_config.h"
#include "comms.h"
#include "logic_analyzer.h"
#include "tim.h"
#include "scope.h"
#include "messages.h"
#include "queue.h"

/** @defgroup Logic_Analyzer Logic Analyzer
 * @{
 */

/** @defgroup Logic_Analyzer_Private_Variables LA Private Variables
 * @{
 */
xQueueHandle logAnlysMessageQueue;
xSemaphoreHandle logAnlysMutex;

volatile logAnlysTypeDef logAnlys;
//portTickType xLastWakeTime;
/**
 * @}
 */

/** @defgroup Logic_Analyzer_Function_Definitions LA Function Definitions
 * @{
 */

/**
 * @brief  Logic analyzer task function.
 * 				Task is getting messages from other tasks, sends messages
 *					to communication (comms) and takes care of logic analyzer functions.
 * @param  Task handler, parameters pointer
 * @retval None
 */
void LogAnlysTask(void const *argument)
{	
	uint16_t message = 0xFFFF;
	logAnlysMessageQueue = xQueueCreate(5, sizeof(message/sizeof(uint8_t)));
	logAnlysMutex = xSemaphoreCreateRecursiveMutex();	

	logAnlysSetDefault();

	/* Get tick count for vTaskDelayUntil() function */
	//	xLastWakeTime = xTaskGetTickCount();

	while(1){

		xQueueReceive(logAnlysMessageQueue, &message, portMAX_DELAY);
		xSemaphoreTakeRecursive(logAnlysMutex, portMAX_DELAY);

		switch(message){
		uint16_t passMsg;
		case MSG_LOGAN_INIT:
			//			logAnlys.state = LOGA_IDLE;
			logAnlysInit();
			break;
		case MSG_LOGAN_DEINIT:
			logAnlysDeinit();
			//			logAnlys.state = LOGA_IDLE;
			break;
		case MSG_LOGAN_START:
			logAnlysStart();
			break;
		case MSG_LOGAN_STOP:
			logAnlysStop();
			break;
		case MSG_LOGAN_SAMPLING_END:
			passMsg = MSG_LOGAN_SEND_DATA;
			xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
			break;
		default:
			break;
		}			

		xSemaphoreGiveRecursive(logAnlysMutex);
	}
}

/* ************************************************************************************** */
/* ---------------------- Logic analyzer basic settings via queue ----------------------- */
/* ************************************************************************************** */
/**
 * @brief  Logic analyzer initialization request.
 *					Sends the request to the queue of Logic analyzer task.
 *					(Command sent from host and parsed in cmd_parser.c where this function is called from.)
 * @param  None
 * @retval None
 */
void logAnlysSendInit(void){
	uint16_t passMsg = MSG_LOGAN_INIT;
	xQueueSendToBack(logAnlysMessageQueue, &passMsg , portMAX_DELAY);
}

/**
 * @brief  Logic analyzer deinitialization request.
 *					Sends the request to the queue of Logic analyzer task.
 *					(Command sent from host and parsed in cmd_parser.c where this function is called from.)
 * @param  None
 * @retval None
 */
void logAnlysSendDeinit(void){
	uint16_t passMsg = MSG_LOGAN_DEINIT;
	xQueueSendToBack(logAnlysMessageQueue, &passMsg, portMAX_DELAY);
}

/**
 * @brief  Logic analyzer start sampling request.
 *					Sends the request to the queue of Logic analyzer task.
 *					(Command sent from host and parsed in cmd_parser.c where this function is called from.)
 * @param  None
 * @retval None
 */
void logAnlysSendStart(void){
	uint16_t passMsg = MSG_LOGAN_START;
	xQueueSendToBack(logAnlysMessageQueue, &passMsg, portMAX_DELAY);
}

/**
 * @brief  Logic analyzer stop sampling request.
 *					Sends the request to the queue of Logic analyzer task.
 *					(Command sent from host and parsed in cmd_parser.c where this function is called from.)
 * @param  None
 * @retval None
 */
void logAnlysSendStop(void){
	uint16_t passMsg = MSG_LOGAN_STOP;
	xQueueSendToBack(logAnlysMessageQueue, &passMsg, portMAX_DELAY);
}

/* ************************************************************************************** */
/* ------------------------ Logic analyzer Interrupts/Callbacks ------------------------- */
/* ************************************************************************************** */
/* TIM4 overflow (Update Event after posttrigger) 
	 occured after trigger event that started TIM4. */
/**
 * @brief  Period elapsed callback of posttrigger time which represents sampling time after the trigger occurs.
 *					Sends the info to the queue of Logic analyzer task.
 *					Called from LOG_ANLYS_PeriodElapsedCallback() (ISR) in tim.c.
 * @param  None
 * @retval None
 */
void logAnlysPeriodElapsedCallback(void){
	portBASE_TYPE xHigherPriorityTaskWoken;
	uint16_t passMsg = MSG_LOGAN_SAMPLING_END;
	xQueueSendToBackFromISR(logAnlysMessageQueue, &passMsg, &xHigherPriorityTaskWoken);
}

/* ************************************************************************************** */
/* --------------------------- Logic analyzer basic settings ---------------------------- */
/* ************************************************************************************** */
/**
 * @brief  Initialization function of Logic analyzer.
 *					Called from Logic analyzer task.
 * @param  None
 * @retval None
 */
void logAnlysInit(void){
	/* Log. analyzer uses TIM4 as well as Universal counter. Therefore, there
		 has to be some clue for msp_init function to decide which functionality
		 to initialize - LOGA_ENABLED */
	logAnlys.enable = LOGA_ENABLED;
	TIM_LogAnlys_Init();
}	

/**
 * @brief  Deinitialization function of Logic analyzer.
 *					Called from Logic analyzer task.
 * @param  None
 * @retval None
 */
void logAnlysDeinit(void){
	TIM_LogAnlys_Deinit();
	logAnlys.enable = LOGA_DISABLED;
}	

/**
 * @brief  Start Logic analyzer function.
 *					Called from Logic analyzer task.
 * @param  None
 * @retval None
 */
void logAnlysStart(void){
	/* Start sampling */		
	TIM_LogAnlys_Start();		
	logAnlys.state = LOGA_SAMPLING;			

	/* Wait the pretrigger time */
	vTaskDelay(logAnlys.preTriggerTime/portTICK_RATE_MS);	
	__HAL_GPIO_EXTI_CLEAR_IT(0x3fc0);  // mask for sampled loganlys PB pins

	if(logAnlys.triggerMode == LOGA_MODE_AUTO){
		/* In AUTO trigger mode the posttriger is started without event trigger. After posttrigger 
			 time elapses the data is sent to PC even if the trigger did not occur. */
		LOG_ANLYS_TriggerEventOccured();		
		TIM_PostTrigger_SoftwareStart();	
	}

	/* Enable trigger after pretrigger time elapses */		
	GPIO_EnableTrigger();	
}	

/**
 * @brief  Stop Logic analyzer function.
 *					Called from Logic analyzer task.
 * @param  None
 * @retval None
 */
void logAnlysStop(void){
	TIM_LogAnlys_Stop();
	logAnlys.state = LOGA_WAIT_FOR_RESTART;
}

/* Configure TIM1 to trigger DMA with required frequency. */
/**
 * @brief  Sets sampling frequency of Logic analyzer.
 *					This function sets timer frequency for triggering DMA in order to trasfer data from GPIOs to RAM.
 *					Called directly from parser (cmd_parser.c).
 * @param  arrPsc: 16-bit ARR and 16-bit PSC register values of timer in one 32-bit
 * @retval None
 */
void logAnlysSetSamplingFreq(uint32_t samplingFrequency){
	TIM_LogAnlys_SamplingFreq_Reconfig(samplingFrequency);
}

/**
 * @brief  Sets data length (samples number) of Logic analyzer.
 *					Called directly from parser (cmd_parser.c).
 * @param  samplesNum: 16-bit value
 * @retval None
 */
void logAnlysSetSamplesNum(uint16_t samplesNum){
	xSemaphoreTakeRecursive(logAnlysMutex, portMAX_DELAY);
	logAnlys.samplesNumber = samplesNum;
	xSemaphoreGiveRecursive(logAnlysMutex);
}

/**
 * @brief  Sets pretrigger time of Logic analyzer.
 *					This function configures pretrigger time which represents the time before the trigger on selected channel occurs.
 *					Called directly from parser (cmd_parser.c).
 * @param  timeInMilliseconds: time in milliseconds
 * @retval None
 */
void logAnlysSetPretrigger(uint32_t pretrigMillisec){
	xSemaphoreTakeRecursive(logAnlysMutex, portMAX_DELAY);
	/* logAnlys task to sleep for defined time in milliseconds */
	logAnlys.preTriggerTime = pretrigMillisec;
	xSemaphoreGiveRecursive(logAnlysMutex);
}

/* Configure TIM4 to stop TIM1 */
/**
 * @brief  Sets posttrigger time of Logic analyzer.
 *					This function configures and starts one timer which after timeout stops the sampling timer in order to stop transferring the data.
 *					Called directly from parser (cmd_parser.c).
 * @param  posttrigInSec: time of posttrigger in seconds
 * @retval None
 */
void logAnlysSetPosttrigger(double posttrigInSec){
	TIM_LogAnlys_PostTrigger_Reconfig(posttrigInSec);
}

/**
 * @brief  Configures the trigger edge sensitivity to rising.
 *					Called directly from parser (cmd_parser.c).
 * @param  None
 * @retval None
 */
void logAnlysSetTriggerRising(void){
	logAnlys.trigEdge = TRIG_EDGE_RISING;
	//	GPIO_EnableTrigger();
}

/**
 * @brief  Configures the trigger edge sensitivity to falling.
 *					Called directly from parser (cmd_parser.c).
 * @param  None
 * @retval None
 */
void logAnlysSetTriggerFalling(void){
	logAnlys.trigEdge = TRIG_EDGE_FALLING;
	//	GPIO_EnableTrigger();
}

/**
 * @brief  Disables the interrupt request from trigger (GPIO).
 * @param  None
 * @retval None
 * @state  NOT USED
 */
void logAnlysDisablePostTrigIRQ(void){
	GPIO_DisableIRQ();
}

/**
 * @brief  Sets trigger channel.
 *					Called directly from parser (cmd_parser.c)
 * @param  chan: channel number between 1 and 8
 * @retval None
 */
void logAnlysSetTriggerChannel(uint32_t chan){
	switch(chan){
	case 1:
		logAnlys.trigConfig = TRIG_CHAN1;
		break;
	case 2:
		logAnlys.trigConfig = TRIG_CHAN2;
		break;
	case 3:
		logAnlys.trigConfig = TRIG_CHAN3;
		break;
	case 4:
		logAnlys.trigConfig = TRIG_CHAN4;
		break;
	case 5:
		logAnlys.trigConfig = TRIG_CHAN5;
		break;
	case 6:
		logAnlys.trigConfig = TRIG_CHAN6;
		break;
	case 7:
		logAnlys.trigConfig = TRIG_CHAN7;
		break;
	case 8:
		logAnlys.trigConfig = TRIG_CHAN8;
		break;
	}
	//		GPIO_EnableTrigger();
}

/**
 * @brief  Sets default Logic analyzer structure values such as:
 *					pretrigger time, samples number, trigger edge, trigger mode, pointer to samples buffer
 *					Called at the beginning of Logic analyzer task.
 * @param  None
 * @retval None
 */
void logAnlysSetDefault(void){
	/* By default: dataLength = 1 Ksamples, samplingFreq = 10 Ksmpls / s, trigger = 50 %
	 Therefore, 100 ms * 50 % = 50 ms. It applies that postTrigger is set with period 
	 50 ms as well as in One Pulse mode. */
	logAnlys.preTriggerTime = 50;
	logAnlys.samplesNumber = 1000;
	logAnlys.trigConfig = TRIG_CHAN1;
	logAnlys.trigEdge = TRIG_EDGE_RISING;
	logAnlys.triggerMode = LOGA_MODE_AUTO;
	logAnlys.trigOccur = TRIG_NOT_OCCURRED;
	logAnlys.bufferMemory = (uint16_t *)scopeBuffer;
}

/**
 * @}
 */
#endif //USE_LOG_ANLYS
/**
 * @}
 */

