/**
 *****************************************************************************
 * @file    tim_logan.c
 * @author  HeyBirdie
 * @date    July 13, 2019
 * @brief   This file contains timer functions for logic analyzer
 *****************************************************************************
 */

#include "tim.h"
#include "logic_analyzer.h"
#include "mcu_config.h"
#include "stm32f3xx_ll_tim.h"

#ifdef USE_LOG_ANLYS

/** @addtogroup Logic_Analyzer
 * @{
 */

/** @defgroup Logic_Analyzer_Timers Logic Analyzer Timers
 * @{
 */

/** @defgroup Logic_Analyzer_Timer_Variables Logic Analyzer Timer Variables
 * @{
 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim4;
DMA_HandleTypeDef hdma_tim1_up;
DMA_HandleTypeDef hdma_tim4_up;

/**
 * @}
 */

/** @defgroup Logic_Analyzer_TIM_Init_Functions LA TIMs Initialization Functions
 * @{
 */

/**
 * @brief  TIM1 Configuration.
 * @note   Configures the timer to periodically trigger DMA for transferring data
 *					from GPIOs to RAM (Logic analyzer data sampling).
 * @param  None
 * @retval None
 */
void MX_TIM1_LOG_ANLYS_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_SlaveConfigTypeDef sSlaveConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;  //0
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 14399;   //14399
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim1);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
	sSlaveConfig.InputTrigger = TIM_TS_ITR3;
	HAL_TIM_SlaveConfigSynchronization(&htim1, &sSlaveConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
}

/**
 * @brief  TIM4 Configuration. (F303RE: TIM4 = 72Mhz)
 * @note   Represents posttrigger time launched right after incoming event on required channel.
 * @param  None
 * @retval None
 */
void MX_TIM4_LOG_ANLYS_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	/* By default 1 Ksample buffer, 10 Ksamples per second, 50% trigger
		 => 50 ms pretrigger, 50 ms posttrigger - 20 Hz (PSC = 1200, ARR = 60K) */
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 1199;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 59999;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim4);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

	HAL_TIM_OnePulse_Init(&htim4, TIM_OPMODE_SINGLE);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);
}

void TIM1_LOG_ANLYS_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Peripheral clock enable */
	__HAL_RCC_TIM1_CLK_ENABLE();

	/*Configure GPIO pins : PB10 PB11 PB12 PB13 PB7 PB8 PB9 */
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT; //GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP; //GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn,9,0);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,9,0);

	/* TIM1 DMA Init */
	/* TIM1_UP Init */
	hdma_tim1_up.Instance = DMA1_Channel5;
	hdma_tim1_up.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim1_up.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim1_up.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim1_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim1_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim1_up.Init.Mode = DMA_CIRCULAR;
	hdma_tim1_up.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&hdma_tim1_up);
	/* Trigger DMA by TIMer to transfer data from GPIO IDR reg. to memory buffer. */
	__HAL_TIM_ENABLE_DMA(&htim1, TIM_DIER_UDE);
	__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim1_up);
}

void TIM4_LOG_ANLYS_MspInit(TIM_HandleTypeDef* htim_base)
{
	/* Peripheral clock enable */
	__HAL_RCC_TIM4_CLK_ENABLE();

	__HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);

	/* TIM4 interrupt Init */
	HAL_NVIC_SetPriority(TIM4_IRQn, 9, 0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_LOG_ANLYS_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM4_CLK_DISABLE();
	HAL_NVIC_DisableIRQ(TIM4_IRQn);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}

void TIM1_LOG_ANLYS_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	/* Peripheral clock disable */
	__HAL_RCC_TIM1_CLK_DISABLE();
	/* TIM1 DMA DeInit */
	HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_UPDATE]);
}

/**
 * @}
 */

/** @defgroup Logic_Analyzer_TIM_Callbacks LA TIMs Callback Functions.
 * @{
 */
/**
 * @brief  Posttrigger time elapsed callback.
 * @note		The time after the trigger occured elapsed and all required data is sampled.
 * @params htim:	TIM handler
 * @retval None
 */
void LOG_ANLYS_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) != RESET)
	//  {
	//    if(__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_UPDATE) != RESET)
	//    {
	__HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);

	/* Stop timer trigering the DMA for data transfer */
	//HAL_TIM_Base_Stop(&htim1);
	__HAL_TIM_DISABLE(&htim4);
	HAL_DMA_Abort(&hdma_tim1_up);

	GPIO_DisableIRQ();

	/* Data sending */
	if(logAnlys.trigOccur == TRIG_OCCURRED){
		logAnlysPeriodElapsedCallback();
	}
	//    }
	//  }
}
/**
 * @}
 */

/** @defgroup Logic_Analyzer_Timer_Functions Logic Analyzer Timer Functions.
 * @{
 */

/* ************************************************************************************** */
/* ------------------------------ Logic analyzer Functions ------------------------------ */
/* ************************************************************************************** */

/**
 * @brief  Stores the time when the trigger should have occured.
 * @note		Stores CNDTR reg. of DMA. This function is used only for AUTO trigger in case the trigger did not occur.
 *					It is the value of posttrigger time start and will change only if the actual trigger uccurs.
 * @params None
 * @retval None
 */
void LOG_ANLYS_TriggerEventOccured(void)
{
	/* Trigger interrupt after posttriger timer elapses (Update Event). */
	logAnlys.triggerPointer = hdma_tim1_up.Instance->CNDTR;
	logAnlys.trigOccur = TRIG_OCCURRED;
}

/**
 * @brief  Initialization Logic Analyzer.
 * @note		TIM1 & TIM4 init.
 * @params None
 * @retval None
 */
void TIM_LogAnlys_Init(void)
{
	__HAL_RCC_TIM4_FORCE_RESET();
	__HAL_RCC_TIM4_RELEASE_RESET();
	__HAL_RCC_TIM1_FORCE_RESET();
	__HAL_RCC_TIM1_RELEASE_RESET();

	htim4.State = HAL_TIM_STATE_RESET;
	htim1.State = HAL_TIM_STATE_RESET;

	MX_TIM1_LOG_ANLYS_Init();
	MX_TIM4_LOG_ANLYS_Init();
}

/**
 * @brief  Deinit Logic Analyzer.
 * @note		TIM1 & TIM4 peripherals reset.
 * @params None
 * @retval None
 */
void TIM_LogAnlys_Deinit(void)
{
	HAL_TIM_Base_DeInit(&htim4);
	HAL_TIM_Base_DeInit(&htim1);

	__HAL_RCC_TIM4_FORCE_RESET();
	__HAL_RCC_TIM4_RELEASE_RESET();
	__HAL_RCC_TIM1_FORCE_RESET();
	__HAL_RCC_TIM1_RELEASE_RESET();

	htim4.State = HAL_TIM_STATE_RESET;
	htim1.State = HAL_TIM_STATE_RESET;
}

/**
 * @brief  Starts Logic Analyzer sampling.
 * @note		Starts TIM1 for triggering DMA from GPIOs to RAM.
 * @params None
 * @retval None
 */
void TIM_LogAnlys_Start(void)
{
	/* Enable DMA transfers. */
	HAL_DMA_Start(&hdma_tim1_up, (uint32_t)&(GPIOB->IDR), (uint32_t)logAnlys.bufferMemory, logAnlys.samplesNumber + MAX_ADC_CHANNELS * SCOPE_BUFFER_MARGIN);
	/* Start TIM1 to trigger DMA for data transfering with user required frequency. */
	HAL_TIM_Base_Start(&htim1);
}

/**
 * @brief  Stops Logic Analyzer sampling.
 * @note		Stops TIM1 for triggering DMA from GPIOs to RAM. Aborts DMA and disables IRQ from the selected trigger.
 * @params None
 * @retval None
 */
void TIM_LogAnlys_Stop(void)
{
	/* Abort sampling so that CNDTR (DMA data length) can be changed. */
	TIM_SamplingStop();
	GPIO_DisableIRQ();

	HAL_TIM_Base_Stop(&htim4);
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	/* Slave TIM1 is stopped by TIM4 upon Update Event
	   and TIM4 is initialized in One Pulse Mode. */
	logAnlys.trigOccur = TRIG_NOT_OCCURRED;
}

/**
 * @brief  Posttrigger time reconfiguration.
 * @note		The time after the trigger is handled by TIM4.
 * @params posttrigInSec: post trigger in seconds
 * @retval None
 */
void TIM_LogAnlys_PostTrigger_Reconfig(double posttrigInSec)
{
	uint32_t periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM34);
	double freq = 1 / posttrigInSec;
	TIM_ReconfigPrecise(&htim4, periphClock, freq);
	HAL_TIM_Base_Stop(&htim4);
}

/**
 * @brief  Sampling frequency reconfiguration.
 * @note 	   Reconfigures timer TIM1 for triggering DMA to transfer data from GPIOs to RAM. ARR+PSC calculated by host.
 * @params arrPsc: ARR & PSC value
 * @retval None
 */
void TIM_LogAnlys_SamplingFreq_Reconfig(uint32_t smplFreq)
{
	uint32_t periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM1)*2;
	logAnlys.samplingFreq = TIM_ReconfigPrecise(&htim1, periphClock, smplFreq);
}

/**
 * @brief  Software start of posttrigger in Auto trigger mode.
 * @note		Starts TIM4 that represents posttrigger time.
 * @params None
 * @retval None
 */
void TIM_PostTrigger_SoftwareStart(void)
{
	/* Trigger interrupt after posttriger timer elapses (Update Event). */
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	__HAL_TIM_ENABLE(&htim4);
	//	HAL_TIM_Base_Start(&htim4);
}

/**
 * @brief  Disables trigger.
 * @note		Disables all IRQ channels of GPIOs to prevent from triggering.
 * @params None
 * @retval None
 */
void GPIO_DisableIRQ(void){
	__HAL_GPIO_EXTI_CLEAR_IT(0x3fc0);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief  Stops sampling.
 * @note		Stop TIM1 and abort DMA.
 * @params None
 * @retval None
 */
void TIM_SamplingStop(void){
	HAL_TIM_Base_Stop(&htim1);
	HAL_DMA_Abort(&hdma_tim1_up);
}

/* Called from logAnlys Task after pretrigger thread.sleep time elapsed. */
/**
 * @brief  Enables triggers.
 * @note		Enable triggers on selected channel after the pretrigger time data was already samples.
 * @params None
 * @retval None
 */
void GPIO_EnableTrigger(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	IRQn_Type ExtiLine = EXTI0_IRQn;

	//restore default settings
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

	GPIO_InitStructure.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
	EXTI->IMR &= ~(GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);  //when selecting different line the EXTI settings remain the same


	GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP; //GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);


	//init right pin to interrupt
	if(logAnlys.trigEdge == TRIG_EDGE_FALLING){
		GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	}else {
		GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	}

	switch(logAnlys.trigConfig){
	case TRIG_CHAN1:
		GPIO_InitStructure.Pin = GPIO_PIN_6;
		ExtiLine = EXTI9_5_IRQn;
		break;
	case TRIG_CHAN2:
		GPIO_InitStructure.Pin = GPIO_PIN_7;
		ExtiLine = EXTI9_5_IRQn;
		break;
	case TRIG_CHAN3:
		GPIO_InitStructure.Pin = GPIO_PIN_8;
		ExtiLine = EXTI9_5_IRQn;
		break;
	case TRIG_CHAN4:
		GPIO_InitStructure.Pin = GPIO_PIN_9;
		ExtiLine = EXTI9_5_IRQn;
		break;
	case TRIG_CHAN5:
		GPIO_InitStructure.Pin = GPIO_PIN_10;
		ExtiLine = EXTI15_10_IRQn;
		break;
	case TRIG_CHAN6:
		GPIO_InitStructure.Pin = GPIO_PIN_11;
		ExtiLine = EXTI15_10_IRQn;
		break;
	case TRIG_CHAN7:
		GPIO_InitStructure.Pin = GPIO_PIN_12;
		ExtiLine = EXTI15_10_IRQn;
		break;
	case TRIG_CHAN8:
		GPIO_InitStructure.Pin = GPIO_PIN_13;
		ExtiLine = EXTI15_10_IRQn;
		break;
	}

	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_NVIC_EnableIRQ(ExtiLine);
}

/**
 * @}
 */

/**
 * @}
 */

#endif //USE_LOG_ANLYS
