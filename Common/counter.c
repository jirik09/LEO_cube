/**
 *****************************************************************************
 * @file    counter.c
 * @author  HeyBirdie
 * @date    June 3, 2017
 * @brief   This file contains counter functions
 *****************************************************************************
 */

// Includes ===================================================================
#ifdef USE_COUNTER

#include "cmsis_os.h"
#include "mcu_config.h"
#include "comms.h"
#include "counter.h"
#include "tim.h"
#include "stdlib.h"
#include "portmacro.h"
#include "messages.h"

/** @defgroup Counter Counter
 * @{
 */

/** @defgroup Counter_Private_Variables Counter Private Variables
 * @{
 */
xQueueHandle counterMessageQueue;
xSemaphoreHandle counterMutex; //counterBinSemaphore
portTickType xStartTime;

volatile counterTypeDef counter;
/**
 * @}
 */

/** @defgroup Counter_Function_Definitions Counter Function Definitions
 * @{
 */
/**
 * @brief  Counter task function.
 * 				Task is getting messages from other tasks and takes care about counter functions.
 * @param  Task handler, parameters pointer
 * @retval None
 */
void CounterTask(void const *argument) {
	uint16_t message = 0xFFFF;
	counterMessageQueue = xQueueCreate(30, sizeof(message) / sizeof(uint8_t));
	counterMutex = xSemaphoreCreateRecursiveMutex();

	if (counterMessageQueue == 0) {
		while (1)
			; // Queue was not created and must not be used.
	}

	counterInitETR();

	while (1) {

		xQueueReceive(counterMessageQueue, &message, portMAX_DELAY);
		xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);

		switch (message) {
		case MSG_CNT_SET_DIRECT_MODE:
			counterInitETR();
			break;
		case MSG_CNT_SET_RECIPROCAL_MODE:
			counterInitIC();
			break;
		case MSG_CNT_SET_REFERENCE_MODE:
			counterInitREF();
			break;
		case MSG_CNT_SET_INTERVAL_MODE:
			counterInitTI();
			break;
		case MSG_CNT_SET_QUANTITY_FREQUENCY:
			counterSetQuantityFreq();
			break;
		case MSG_CNT_SET_QUANTITY_PERIOD:
			counterSetQuantityPer();
			break;
		case MSG_CNT_SET_IC1_QUANTITY_FREQUENCY:
			counterSetIc1QuantityFreq();
			break;
		case MSG_CNT_SET_IC1_QUANTITY_PERIOD:
			counterSetIc1QuantityPer();
			break;
		case MSG_CNT_SET_IC2_QUANTITY_FREQUENCY:
			counterSetIc2QuantityFreq();
			break;
		case MSG_CNT_SET_IC2_QUANTITY_PERIOD:
			counterSetIc2QuantityPer();
			break;
		case MSG_CNT_START:
			counterStart();
			break;
		case MSG_CNT_STOP:
			counterStop();
			break;
		case MSG_CNT_DEINIT:
			counter_deinit();
			break;
		case MSG_CNT_SET_GATE_TIME:
			counterGateConfig(counter.counterEtr.gateTime);
			break;
		default:
			break;
		}

		xSemaphoreGiveRecursive(counterMutex);
	}
}

/* ************************************************************************************** */
/* -------------------------------- Counter basic settings ------------------------------ */
/* ************************************************************************************** */
/**
 * @brief  Counter mode initialization function.
 *					Sends the request to the queue of Counter task.
 *					Called from parser (cmd_parser.c).
 * @param  mode:	ETR - direct method, IC - reciprocal, TI - time interval, REF - measuring with reference
 * @retval None
 */
void counterSetMode(uint8_t mode) {
	counterSendStop();
	uint16_t passMsg;
	switch (mode) {
	case ETR:
		passMsg = MSG_CNT_SET_DIRECT_MODE;
		xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		break;
	case IC:
		passMsg = MSG_CNT_SET_RECIPROCAL_MODE;
		xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		break;
	case REF:
		passMsg = MSG_CNT_SET_REFERENCE_MODE;
		xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		break;
	case TI:
		passMsg = MSG_CNT_SET_INTERVAL_MODE;
		xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		break;
	}

	if (counter.paused == NO) {
		if (mode != TI)
			counterSendStart();
	}
}

uint8_t counterSetQuantity(uint8_t quant) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_ETR) {
		uint16_t passMsg;
		if (quant == QUANTITY_FREQUENCY) {
			passMsg = MSG_CNT_SET_QUANTITY_FREQUENCY;
			xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		} else if (quant == QUANTITY_PERIOD) {
			passMsg = MSG_CNT_SET_QUANTITY_PERIOD;
			xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		}
		result = 0;
	}
	return result;
}

uint8_t counterSetIc1Quantity(uint8_t quant) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_IC) {
		uint16_t passMsg;
		if (quant == QUANTITY_FREQUENCY) {
			passMsg = MSG_CNT_SET_IC1_QUANTITY_FREQUENCY;
			xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		} else if (quant == QUANTITY_PERIOD) {
			passMsg = MSG_CNT_SET_IC1_QUANTITY_PERIOD;
			xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		}
		result = 0;
	}
	return result;
}

uint8_t counterSetIc2Quantity(uint8_t quant) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_IC) {
		uint16_t passMsg;
		if (quant == QUANTITY_FREQUENCY) {
			passMsg = MSG_CNT_SET_IC2_QUANTITY_FREQUENCY;
			xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		} else if (quant == QUANTITY_PERIOD) {
			passMsg = MSG_CNT_SET_IC2_QUANTITY_PERIOD;
			xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		}
		result = 0;
	}
	return result;
}

/**
 * @brief  Counter start request.
 *					Sends the request to the queue of Counter task.
 *					Called from parser (cmd_parser.c).
 * @param  None
 * @retval None
 */
void counterSendStart(void) {
	counter.sampleCntChange = SAMPLE_COUNT_CHANGED;
	uint16_t passMsg = MSG_CNT_START;
	xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
}

/**
 * @brief  Counter stop request.
 *					Sends the request to the queue of Counter task.
 *					Called from parser (cmd_parser.c).
 * @param  None
 * @retval None
 */
void counterSendStop(void) {
	uint16_t passMsg = MSG_CNT_STOP;
	xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
}

void counterPause(void) {
	counter.paused = YES;
	counterSendStop();
}

void counterUnpause(void) {
	counter.paused = NO;
	counterSendStart();
}

/**
 * @brief  Counter deinitialization request.
 *					Sends the request to the queue of Counter task.
 *					Called from parser (cmd_parser.c).
 * @param  None
 * @retval None
 */
void counterDeinit(void) {
	uint16_t passMsg = MSG_CNT_DEINIT;
	xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
}

/* ************************************************************************************** */
/* ---------------------------- Counter INIT DEINIT functions --------------------------- */
/* ************************************************************************************** */
/**
 * @brief  Counter direct (External TRigger) mode initialization function.
 *					Called from Counter task.
 * @param  None
 * @retval None
 */
void counterInitETR(void) {
	counter_deinit();
	counter.state = COUNTER_ETR;
	TIM_counter_etr_init();
}

/**
 * @brief  Counter reciprocal (Input Capture) mode initialization function.
 *					Called from Counter task.
 * @param  None
 * @retval None
 */
void counterInitIC(void) {
	counter_deinit();
	counter.state = COUNTER_IC;
	TIM_counter_ic_init();
}

/**
 * @brief  Counter reference measurement mode initialization function.
 *					Allows measuring frequency with precise external clock source.
 *					Called from Counter task.
 * @param  None
 * @retval None
 */
void counterInitREF(void) {
	counter_deinit();
	counter.state = COUNTER_REF;
	TIM_counter_ref_init();
}

/**
 * @brief  Counter Time Interval mode initialization function.
 *					Allows measuring time between two events (rising/falling edge) on single or two channels.
 *					Called from Counter task.
 * @param  None
 * @retval None
 */
void counterInitTI(void) {
	counter_deinit();
	counter.state = COUNTER_TI;
	TIM_counter_ti_init();
}

/**
 * @brief  Counter deinitialization function.
 * @param  None
 * @retval None
 */
void counter_deinit(void) {
	switch (counter.state) {
	case COUNTER_ETR:
		TIM_etr_deinit();
		break;
	case COUNTER_IC:
		if (counter.icDutyCycle != DUTY_CYCLE_DISABLED) {
			counterIcDutyCycleDisable();
		}
		TIM_ic_deinit();
		break;
	case COUNTER_REF:
		TIM_ref_deinit();
		break;
	case COUNTER_TI:
		TIM_ti_deinit();
		break;
	case COUNTER_IDLE:
		/* no hacer nada */
		break;
	}
}

/* ************************************************************************************** */
/* ---------------------------- Counter START STOP functions ---------------------------- */
/* ************************************************************************************** */
/**
 * @brief  Counter start function.
 *					Called from Counter task.
 * @param  None
 * @retval None
 */
void counterStart(void) {
	switch (counter.state) {
	case COUNTER_ETR:
		TIM_ETR_Start();
		break;
	case COUNTER_IC:
		TIM_IC_Start();
		break;
	case COUNTER_REF:
		TIM_ETR_Start();
		break;
	case COUNTER_TI:
		TIM_TI_Start();
		break;
	case COUNTER_IDLE:
		/* no hacer nada */
		break;
	}
}

/**
 * @brief  Counter stop function.
 *					Called from Counter task.
 * @param  None
 * @retval None
 */
void counterStop(void) {
	switch (counter.state) {
	case COUNTER_ETR:
		TIM_ETR_Stop();
		break;
	case COUNTER_IC:
		TIM_IC_Stop();
		break;
	case COUNTER_REF:
		TIM_ETR_Stop();
		break;
	case COUNTER_TI:
		TIM_TI_Stop();
		break;
	case COUNTER_IDLE:
		/* no hacer nada */
		break;
	}
}

void counterSetQuantityFreq(void) {
	xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);
	counter.counterEtr.quantity = QUANTITY_FREQUENCY;
	xSemaphoreGiveRecursive(counterMutex);
}

void counterSetQuantityPer(void) {
	xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);
	counter.counterEtr.quantity = QUANTITY_PERIOD;
	xSemaphoreGiveRecursive(counterMutex);
}

void counterSetIc1QuantityFreq(void) {
	xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);
	counter.counterIc.quantityChan1 = QUANTITY_FREQUENCY;
	xSemaphoreGiveRecursive(counterMutex);
}

void counterSetIc1QuantityPer(void) {
	xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);
	counter.counterIc.quantityChan1 = QUANTITY_PERIOD;
	xSemaphoreGiveRecursive(counterMutex);
}

void counterSetIc2QuantityFreq(void) {
	xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);
	counter.counterIc.quantityChan2 = QUANTITY_FREQUENCY;
	xSemaphoreGiveRecursive(counterMutex);
}

void counterSetIc2QuantityPer(void) {
	xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);
	counter.counterIc.quantityChan2 = QUANTITY_PERIOD;
	xSemaphoreGiveRecursive(counterMutex);
}

/**
 * @brief  Setter for time gating of direct counting (ETR).
 * @param  gateTime: 100, 500, 1000, 5000, 10000 in milliseconds
 * @retval None
 */
uint8_t counterSetEtrGate(uint16_t gateTime) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_ETR) {
		counter.counterEtr.gateTime = gateTime;
		uint16_t passMsg = MSG_CNT_SET_GATE_TIME;
		xQueueSendToBack(counterMessageQueue, &passMsg, portMAX_DELAY);
		result = 0;
	}
	return result;
}

/**
 * @brief  Function setting Sample count of Reference counter
 * @param  sampleCount: 0x02 - 0xF4240000
 * @retval None
 */
uint8_t counterSetRefSampleCount(uint32_t sampleCount) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_REF) {
		xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);
		counter.sampleCntChange = SAMPLE_COUNT_CHANGED;
		counter.counterEtr.refBuffer = sampleCount;
		xSemaphoreGiveRecursive(counterMutex);
		TIM_REF_Reconfig_cnt(sampleCount);
		result = 0;
	}
	return result;
}

/**
 * @brief  Setter for counter IC buffer size (number of edges counted) on channel 1.
 * @param  buffer: range between 2 - xxx (max. value depends on free memory availability)
 * @retval None
 */
uint8_t counterSetIc1SampleCount(uint16_t buffer) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_IC) {
		xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);
		counter.counterIc.ic1BufferSize = buffer + 1; // PC app sends number of samples but IC needs the number of edges, therefore "buffer + 1";
		counter.sampleCntChange = SAMPLE_COUNT_CHANGED;
		DMA_Restart(&hdma_tim2_ch1);
		xSemaphoreGiveRecursive(counterMutex);
		result = 0;
	}
	return result;
}

/**
 * @brief  Setter for counter IC buffer size (number of edges counted) on channel 2.
 * @param  buffer: range between 2 - xxx (max. value depends on free memory availability)
 * @retval None
 */
uint8_t counterSetIc2SampleCount(uint16_t buffer) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_IC) {
		xSemaphoreTakeRecursive(counterMutex, portMAX_DELAY);
		counter.counterIc.ic2BufferSize = buffer + 1;
		counter.sampleCntChange = SAMPLE_COUNT_CHANGED;
		DMA_Restart(&hdma_tim2_ch2_ch4);
		xSemaphoreGiveRecursive(counterMutex);
		result = 0;
	}
	return result;
}

/**
 * @brief  Setter for counter IC signal prescaler on channel 1.
 * @param  presc: 1, 2, 4, 8
 * @retval None
 */
uint8_t counterSetIc1Prescaler(uint16_t presc) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_IC) {
		counterSendStop();
		counter.sampleCntChange = SAMPLE_COUNT_CHANGED;
		TIM_IC1_PSC_Config(presc);
		DMA_Restart(&hdma_tim2_ch1);
		if (counter.paused == NO) {
			counterSendStart();
		}
		result = 0;
	}
	return result;
}

/**
 * @brief  Setter for counter IC signal prescaler on channel 2.
 * @param  presc: 1, 2, 4, 8
 * @retval None
 */
uint8_t counterSetIc2Prescaler(uint16_t presc) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_IC) {
		counterSendStop();
		counter.sampleCntChange = SAMPLE_COUNT_CHANGED;
		TIM_IC2_PSC_Config(presc);
		DMA_Restart(&hdma_tim2_ch2_ch4);
		if (counter.paused == NO) {
			counterSendStart();
		}
		result = 0;
	}
	return result;
}

/**
 * @brief  DUTY CYCLE enable/disable
 * @param  None
 * @retval None
 */
uint8_t counterIc1DutyCycleEnable(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_IC) {
		counterSetIc1SampleCount(1);
		counter.icDutyCycle = DUTY_CYCLE_CH1_ENABLED;
		TIM_IC_DutyCycle_Init();
		TIM_IC_DutyCycle_Start();
		DMA_Restart(&hdma_tim2_ch1);
		result = 0;
	}
	return result;
}

uint8_t counterIc2DutyCycleEnable(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_IC) {
		counterSetIc2SampleCount(1);
		counter.icDutyCycle = DUTY_CYCLE_CH2_ENABLED;
		TIM_IC_DutyCycle_Init();
		TIM_IC_DutyCycle_Start();
		result = 0;
	}
	return result;
}

uint8_t counterIcDutyCycleDisable(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_IC) {
		TIM_IC_DutyCycle_Stop();
		TIM_IC_DutyCycle_Deinit();
		counter.icDutyCycle = DUTY_CYCLE_DISABLED;
		result = 0;
	}
	return result;
}

/**
 * @brief  Configures the trigger edge sensitivity to rising/falling.
 *					For IC and TI modes on channel 1.
 * @param  None
 * @retval None
 */
uint8_t counterSetIcTi1_RisingFalling(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		TIM_IC1_RisingFalling();
		DMA_Restart(&hdma_tim2_ch1);
		result = 0;
	}
	return result;
}

/**
 * @brief  Configures the trigger edge sensitivity to rising.
 *					For IC and TI modes on channel 1.
 * @param  None
 * @retval None
 */
uint8_t counterSetIcTi1_Rising(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		counter.eventChan1 = EVENT_RISING;
		TIM_IC1_RisingOnly();
		result = 0;
	}
	return result;
}

/**
 * @brief  Configures the trigger edge sensitivity to falling.
 *					For IC and TI modes on channel 1.
 * @param  None
 * @retval None
 */
uint8_t counterSetIcTi1_Falling(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		counter.eventChan1 = EVENT_FALLING;
		TIM_IC1_FallingOnly();
		result = 0;
	}
	return result;
}

/**
 * @brief  Configures the trigger edge sensitivity to rising/falling.
 *					For IC and TI modes on channel 2.
 * @param  None
 * @retval None
 */
uint8_t counterSetIcTi2_RisingFalling(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		TIM_IC2_RisingFalling();
		DMA_Restart(&hdma_tim2_ch2_ch4);
		result = 0;
	}
	return result;
}

/**
 * @brief  Configures the trigger edge sensitivity to rising.
 *					For IC and TI modes on channel 2.
 * @param  None
 * @retval None
 */
uint8_t counterSetIcTi2_Rising(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		counter.eventChan2 = EVENT_RISING;
		TIM_IC2_RisingOnly();
		result = 0;
	}
	return result;
}

/**
 * @brief  Configures the trigger edge sensitivity to falling.
 *					For IC and TI modes on channel 2.
 * @param  None
 * @retval None
 */
uint8_t counterSetIcTi2_Falling(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		counter.eventChan2 = EVENT_FALLING;
		TIM_IC2_FallingOnly();
		result = 0;
	}
	return result;
}

/**
 * @brief  Configures sequence of events AB to be measured. E.g. setting event A on channel 1 and event B on channel 2 and sequence AB then we can measure time between AB.
 *					For TI mode only.
 * @param  None
 * @retval None
 */
uint8_t counterSetTiSequence_AB(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		TIM_TI_Sequence_AB();
		result = 0;
	}
	return result;
}

/**
 * @brief  Configures sequence of events BA to be measured. E.g. setting event A on channel 2 and event B on channel 1 or vice versa and sequence BA, we can measure time between BA.
 *					For TI mode only.
 * @param  None
 * @retval None
 */
uint8_t counterSetTiSequence_BA(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		TIM_TI_Sequence_BA();
		result = 0;
	}
	return result;
}

/**
 * @brief  Function configuring event sequence dependence - refer to tim.c TIM_TI_Start() for more information.
 * @param  None
 * @retval None
 */
uint8_t counterSetTiMode_Independent(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		counter.tiMode = TI_MODE_EVENT_SEQUENCE_INDEP;
		result = 0;
	}
	return result;
}

uint8_t counterSetTiMode_Dependent(void) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		counter.tiMode = TI_MODE_FAST_EVENT_SEQUENCE_DEP;
		result = 0;
	}
	return result;
}

/**
 * @brief  Setter for counter TI measurement timeout.
 * @param  timeout: 1 - 3600 seconds
 * @retval None
 */
uint8_t counterSetTiTimeout(uint32_t timeout) {
	uint8_t result = CNT_FEATURE_MODE_MISMATCH;
	if (counter.state == COUNTER_TI) {
		counter.counterIc.tiTimeout = timeout * 1000;
		xStartTime = xTaskGetTickCount();
		result = 0;
	}
	return result;

}

/* ************************************************************************************** */
/* ----------------------------- Counter callback functions ----------------------------- */
/* ************************************************************************************** */
/**
 * @brief  This function is executed in case of DMA transfer complete event of direct (ETR) or reference (REF) mode.
 * 		The DMA transfer complete event is triggered after TIM4 gate time elapses.
 * @param  Pointer to DMA handle structure.
 * @retval None
 * @state  VERY USED
 */
void COUNTER_ETR_DMA_CpltCallback(DMA_HandleTypeDef *dmah) {
	portBASE_TYPE xHigherPriorityTaskWoken;
	uint16_t passMsg = MSG_CNT_SEND_DATA;

	/***** Counter ETR handle *****/
	if (counter.state == COUNTER_ETR) {

		counter.counterEtr.etrp = TIM_ETPS_GetPrescaler();
		float gateFreq = ((double) counter.tim4PrphClk
				/ (double) ((counter.counterEtr.arr + 1)
						* (counter.counterEtr.psc + 1))); /* TIM4 gating frequency */
		counter.counterEtr.freq = ((double) counter.counterEtr.buffer * gateFreq
				* counter.counterEtr.etrp); /* Sampled frequency */
		counter.qError = counterEtrCalculateQuantError(gateFreq);
		counter.tbError = counterEtrCalculateTimeBaseError();
		/* Configure the ETR input prescaler */
		TIM_ETRP_Config(counter.counterEtr.freq);

		if (counter.counterEtr.quantity == QUANTITY_PERIOD) {
			counter.counterEtr.freq = 1 / counter.counterEtr.freq;
		}

		if (counter.sampleCntChange != SAMPLE_COUNT_CHANGED) {
			xQueueSendToBackFromISR(messageQueue, &passMsg,
					&xHigherPriorityTaskWoken);

		} else {
			counter.sampleCntChange = SAMPLE_COUNT_NOT_CHANGED;
		}

		/***** Counter REF handle *****/
	} else if (counter.state == COUNTER_REF) {

		if ((counter.sampleCntChange != SAMPLE_COUNT_CHANGED)
				&& (xTaskGetTickCount() - xStartTime) < 100) {
			xQueueSendToBackFromISR(messageQueue, &passMsg,
					&xHigherPriorityTaskWoken);
			TIM_REF_SecondInputDisable();
			counter.refWarning = COUNTER_WARNING_FIRED;

		} else if (counter.sampleCntChange != SAMPLE_COUNT_CHANGED
				&& counter.counterEtr.buffer != 0) {
			counter.counterEtr.freq = counter.counterEtr.refBuffer
					/ (double) counter.counterEtr.buffer;
			xQueueSendToBackFromISR(messageQueue, &passMsg,
					&xHigherPriorityTaskWoken);
			counter.refWarning = COUNTER_REF_SEND_DATA;

		} else {
			counter.sampleCntChange = SAMPLE_COUNT_NOT_CHANGED;
		}
	}
}

/**
 * @brief  This function is executed in case of TIM4 period elapse event. This function accompanies
 IC (freq. meas. + Duty Cycle) and TI (time interval between 2 events meas.) modes.
 * @param  Pointer to TIM handle structure.
 * @retval None
 * @state  VERY USED
 */
void counterPeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (counter.icDutyCycle == DUTY_CYCLE_DISABLED) {
		if (counter.state == COUNTER_IC) {
			/* Input Capture Low Frequency coutner */
			counterIcProcess();
		} else {
			/* Time Interval mode of measurement */
			counterTiProcess();
		}
	} else {
		/* Duty cycle measurement in Low Frequency counter */
		counterIcDutyCycleProcess();
	}
}

/**
 * @brief  Function colaborating with counterPeriodElapsedCallback to handle every 100 ms captured events if the required samples were transfered.
 Frequency of IC1 or IC2 channel is computed and sent to PC app. This approach replaces DMA data transfercomplete interrupts	of both
 channels - the higher frequencies measured the CPU more heavy loaded, therefore replaced.
 BIN "semaphore" implemented due to communication issue when data send to PC. (too slow)
 * @param  None
 * @retval None
 * @state  VERY USED
 */
int testChann1 = 0, testChann2 = 0;
void counterIcProcess(void) {
	portBASE_TYPE xHigherPriorityTaskWoken;
	uint16_t passMsg = MSG_CNT_SEND_DATA;

	if (counter.bin != BIN0) {
		counter.bin = BIN0;

		if (DMA_TransferComplete(&hdma_tim2_ch1)) {
			testChann1++;
			counter.counterIc.ic1psc = TIM_IC1PSC_GetPrescaler();
			uint32_t capture1 =
					counter.counterIc.ic1buffer[counter.counterIc.ic1BufferSize
							- 1] - counter.counterIc.ic1buffer[0];
			counter.counterIc.ic1freq = (double) (counter.tim2PrphClk
					* (counter.counterIc.psc + 1) * counter.counterIc.ic1psc)
					* ((double) (counter.counterIc.ic1BufferSize - 1)
							/ (double) capture1);

			counter.qError = counterIcCalculateQuantError(1);
			counter.tbError = counterIcCalculateTimeBaseError(1);
			if (counter.counterIc.quantityChan1 == QUANTITY_PERIOD) {
				counter.counterIc.ic1freq = 1 / counter.counterIc.ic1freq;
			}

			counter.icChannel1 = COUNTER_IRQ_IC;
			xQueueSendToBackFromISR(messageQueue, &passMsg,
					&xHigherPriorityTaskWoken);

			DMA_Restart(&hdma_tim2_ch1);
		}
	} else if (counter.bin != BIN1) {
		counter.bin = BIN1;

		if (DMA_TransferComplete(&hdma_tim2_ch2_ch4)) {
			testChann2++;
			counter.counterIc.ic2psc = TIM_IC2PSC_GetPrescaler();
			uint32_t capture2 =
					counter.counterIc.ic2buffer[counter.counterIc.ic2BufferSize
							- 1] - counter.counterIc.ic2buffer[0];
			counter.counterIc.ic2freq = (double) (counter.tim2PrphClk
					* (counter.counterIc.psc + 1) * counter.counterIc.ic2psc)
					* ((double) (counter.counterIc.ic2BufferSize - 1)
							/ (double) capture2);

			counter.qError2 = counterIcCalculateQuantError(2);
			counter.tbError2 = counterIcCalculateTimeBaseError(2);
			if (counter.counterIc.quantityChan2 == QUANTITY_PERIOD) {
				counter.counterIc.ic2freq = 1 / counter.counterIc.ic2freq;
			}

			counter.icChannel2 = COUNTER_IRQ_IC;
			xQueueSendToBackFromISR(messageQueue, &passMsg,
					&xHigherPriorityTaskWoken);

			DMA_Restart(&hdma_tim2_ch2_ch4);
		}
	}
}

/**
 * @brief  Function colaborating with counterPeriodElapsedCallback to handle
 one input (TI1 or TI2) that is feeding two IC registers to calculate pulse width
 and duty cycle. BIN implemented due to UART speed issue when sending data to PC.
 * @param  None
 * @retval None
 * @state  VERY USED
 */
void counterIcDutyCycleProcess(void) {
	portBASE_TYPE xHigherPriorityTaskWoken;
	uint16_t passMsg = MSG_CNT_SEND_DATA;

	if (counter.icDutyCycle == DUTY_CYCLE_CH1_ENABLED) {
		if (DMA_TransferComplete(&hdma_tim2_ch1)) {
			/* Calculate duty cycle (ic1freq) and pulse width(ic2freq). Frequency struct variables temporarily used. */
			counter.counterIc.duty = (counter.counterIc.ic2buffer[0]
					/ (double) counter.counterIc.ic1buffer[0]) * 100;
			counter.counterIc.pulseWidth = counter.counterIc.ic2buffer[0]
					/ (double) counter.tim2PrphClk;

			TIM_IC_DutyCycleDmaRestart();

			/* DMA transfers some unspecified number immediately after
			 Duty Cycle start - getting rid of it. */
			if (counter.bin == BIN0) {
				counter.bin = BIN1;
			} else {
				xQueueSendToBackFromISR(messageQueue, &passMsg,
						&xHigherPriorityTaskWoken);
			}
		}
	} else if (counter.icDutyCycle == DUTY_CYCLE_CH2_ENABLED) {
		if (DMA_TransferComplete(&hdma_tim2_ch2_ch4)) {
			counter.counterIc.duty = (counter.counterIc.ic1buffer[0]
					/ (double) counter.counterIc.ic2buffer[0]) * 100;
			counter.counterIc.pulseWidth = counter.counterIc.ic1buffer[0]
					/ (double) counter.tim2PrphClk;

			TIM_IC_DutyCycleDmaRestart();

			if (counter.bin == BIN0) {
				counter.bin = BIN1;
			} else {
				xQueueSendToBackFromISR(messageQueue, &passMsg,
						&xHigherPriorityTaskWoken);
			}
		}
	}
}

/**
 * @brief  Function colaborating with counterPeriodElapsedCallback to handle
 2 independent input events on 2 separate channels - Time Interval meas.
 mode. CounterIc.ic1freq is used to send time delay between two events.
 * @param  None
 * @retval None
 * @state  VERY USED
 */
void counterTiProcess(void) {
	portBASE_TYPE xHigherPriorityTaskWoken;
	uint16_t passMsg = MSG_CNT_SEND_DATA;

	/* Check timeout. */
	if ((xTaskGetTickCountFromISR() - xStartTime)
			<= counter.counterIc.tiTimeout) {
		/* Check the event sequence - AB or BA */
		if (counter.abba == BIN0) {
			/* Check DMA transfer channel 1 occured */
			if (DMA_TransferComplete(&hdma_tim2_ch2_ch4)) {
				counter.counterIc.ic1freq = counter.counterIc.ic2buffer[0]
						/ (double) counter.tim2PrphClk;
				TIM_TI_Stop();
				counter.tiState = SEND_TI_DATA;
				xQueueSendToBackFromISR(messageQueue, &passMsg,
						&xHigherPriorityTaskWoken);
			}
			/* If timeout occurred stop TI counter and send alert to PC application. */
		} else {
			/* Check DMA transfer channel 2 occured */
			if (DMA_TransferComplete(&hdma_tim2_ch1)) {
				counter.counterIc.ic1freq = counter.counterIc.ic1buffer[0]
						/ (double) counter.tim2PrphClk;
				TIM_TI_Stop();
				counter.tiState = SEND_TI_DATA;
				xQueueSendToBackFromISR(messageQueue, &passMsg,
						&xHigherPriorityTaskWoken);
			}
		}
		counter.qError = counterIcCalculateQuantError(1);
		counter.tbError = counterIcCalculateTimeBaseError(1);
	} else {
		TIM_TI_Stop();
		counter.tiState = TIMEOUT;
		xQueueSendToBackFromISR(messageQueue, &passMsg,
				&xHigherPriorityTaskWoken);
	}
}

/* ************************************************************************************** */
/* ----------------------------- Counter ERROR computations ----------------------------- */
/* ************************************************************************************** */
double counterEtrCalculateQuantError(float gateFreq) {
	double qError = counter.counterEtr.etrp * gateFreq;
	if (counter.counterEtr.quantity == QUANTITY_PERIOD) {
		qError = (1 / (counter.counterEtr.freq - qError)
				- 1 / counter.counterEtr.freq);
	}
	return qError;
}

double counterEtrCalculateTimeBaseError(void) {
	double tbError = counter.counterEtr.freq * NUCLEO_CRYSTAL_ERROR;
	if (counter.counterEtr.quantity == QUANTITY_PERIOD) {
		tbError = (1 / (counter.counterEtr.freq - tbError)
				- 1 / counter.counterEtr.freq);
	}
	return tbError;
}

double counterIcCalculateQuantError(int icChannel) {
	double freq, buffSize;
	counterQuantity channelQuantity;
	uint8_t presc;

	if (icChannel == 1) {
		freq = counter.counterIc.ic1freq;
		buffSize = counter.counterIc.ic1BufferSize;
		channelQuantity = counter.counterIc.quantityChan1;
		presc = counter.counterIc.ic1psc;
	} else {
		freq = counter.counterIc.ic2freq;
		buffSize = counter.counterIc.ic2BufferSize;
		channelQuantity = counter.counterIc.quantityChan2;
		presc = counter.counterIc.ic2psc;
	}

	double qError = freq / (counter.tim4PrphClk - freq) * freq
			/ (buffSize * presc);
	if (channelQuantity == QUANTITY_PERIOD) {
		qError = 1 / (freq - qError) - 1 / freq;
	}
	return qError;
}

double counterIcCalculateTimeBaseError(int icChannel) {
	double freq;
	counterQuantity channelQuantity;

	if (icChannel == 1) {
		channelQuantity = counter.counterIc.quantityChan1;
		freq = counter.counterIc.ic1freq;
	} else {
		channelQuantity = counter.counterIc.quantityChan2;
		freq = counter.counterIc.ic2freq;
	}

	double tbError = freq * NUCLEO_CRYSTAL_ERROR;
	if (channelQuantity == QUANTITY_PERIOD) {
		tbError = 1 / (freq - tbError) - 1 / freq;
	}
	return tbError;
}

/* ************************************************************************************** */
/* ----------------------------- Counter specific functions ----------------------------- */
/* ************************************************************************************** */
/**
 * @brief  This function configures ARR and PSC registers of 16b-it timer if running on 72 MHz.
 * @param  gateTime: gate time in milliseconds
 * @retval none
 */
void counterGateConfig(uint16_t gateTime) {
	counterStop();
	switch (gateTime) {
	case 100: /* min.	gate time 00.10 second */
		TIM_ARR_PSC_Config(0.1);
		break;
	case 500: /* ----	gate time 00.50 second */
		TIM_ARR_PSC_Config(0.5);
		break;
	case 1000: /* ----	gate time 01.00 second */
		TIM_ARR_PSC_Config(1);
		break;
	case 5000: /* ----	gate time 05.00 second */
		TIM_ARR_PSC_Config(5);
		break;
	case 10000: /* max. gate time 10.00 second */
		TIM_ARR_PSC_Config(10);
		break;
	default:
		break;
	}
	if (counter.paused == NO)
		counterStart();
}

/**
 * @brief  Counter set Default values
 * @param  None
 * @retval None
 * @state 	USED
 */
void counterEtrRefSetDefault(void) {
	if (counter.state == COUNTER_ETR) {
		counter.counterEtr.psc = TIM4_PSC;
		counter.counterEtr.arr = TIM4_ARR;
		counter.counterEtr.gateTime = 100; /* 100 ms */
	} else {
		counter.counterEtr.psc = 59999;
		counter.counterEtr.arr = 59999;
		counter.counterEtr.refBuffer = (counter.counterEtr.psc + 1)
				* (counter.counterEtr.arr + 1);
	}
	counter.counterEtr.etrp = 1;
	counter.counterEtr.buffer = 0;
	counter.sampleCntChange = SAMPLE_COUNT_CHANGED;
	counter.counterEtr.quantity = QUANTITY_FREQUENCY;
}

void counterIcTiSetDefault(void) {
	if (counter.state == COUNTER_IC) {
		counter.counterIc.ic1BufferSize = 2; /* the lowest value of icxBufferSize is 2! - 1 sample for IC frequency measuring */
		counter.counterIc.ic2BufferSize = 2;
		counter.icChannel1 = COUNTER_IRQ_IC_PASS;
		counter.icChannel2 = COUNTER_IRQ_IC_PASS;
	} else {
		counter.counterIc.ic1BufferSize = 1; /* only 1 sample for one event that occurs on one single channel */
		counter.counterIc.ic2BufferSize = 1;
		counter.counterIc.tiTimeout = 5000;
		counter.eventChan1 = EVENT_RISING;
		counter.eventChan2 = EVENT_RISING;
		counter.tiMode = TI_MODE_EVENT_SEQUENCE_INDEP;
	}
	counter.counterIc.ic1psc = 1;
	counter.counterIc.ic2psc = 1;
	counter.counterIc.quantityChan1 = QUANTITY_FREQUENCY;
	counter.counterIc.quantityChan2 = QUANTITY_FREQUENCY;
	TIM_IC1_PSC_Config(1);
	TIM_IC2_PSC_Config(1);
	counter.counterIc.psc = 0;
	counter.counterIc.arr = 0xFFFFFFFF;
}

/**
 * @}
 */

/**
 * @}
 */

#endif //USE_COUNTER

