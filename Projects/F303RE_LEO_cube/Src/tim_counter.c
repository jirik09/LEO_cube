/**
 *****************************************************************************
 * @file    tim_counter.c
 * @author  HeyBirdie
 * @date    July 13, 2019
 * @brief   This file contains timer functions for counter
 *****************************************************************************
 */


#include "tim.h"
#include "FreeRTOS.h"
#include "counter.h"
#include "mcu_config.h"
#include "task.h"
#include "semphr.h"
#include "stm32f3xx_ll_tim.h"

#ifdef USE_COUNTER

/** @addtogroup Counter
 * @{
 */

/** @defgroup Counter_Timers Counter Timers
 * @{
 */

/** @defgroup Counter_Timer_Variables Counter Variables
 * @{
 */

extern portTickType xStartTime;
extern xSemaphoreHandle counterMutex;
extern xQueueHandle counterMessageQueue;
uint32_t timCcerRegCc1eVal = 0x01;
uint32_t timCcerRegCc2eVal = 0x10;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
DMA_HandleTypeDef hdma_tim2_up;
DMA_HandleTypeDef hdma_tim2_ch1;
DMA_HandleTypeDef hdma_tim2_ch2_ch4;

/**
 * @}
 */

/** @defgroup Counter_Timer_Init_Functions Counter Timer Initialization Functions
 * @{
 */

/**
 * @brief  TIM4 Configuration.
 * @note   Direct (ETR): used for timing of input signal gating. Then applying T=t/n resp. f=n/t.
 * @note   Reference (REF): used for counting an external clock. Gate opened according to the frequency of the external clk source.
 * @note   Reciprocal (IC): used for periodical check whether all the data was already transfered.
 * @note   Time Interval (TI): used for periodical check whether all the data was already transfered.
 * @param  None
 * @retval None
 */
void MX_TIM4_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim4.Instance = TIM4;
	if(counter.state == COUNTER_REF){
		/* REF mode - 3.6B samples (60000 * 60000) */
		htim4.Init.Prescaler = 59999;
		htim4.Init.Period = 59999;
	}else if(counter.state == COUNTER_ETR){
		/* ETR mode - 100 ms gate time by default */
		htim4.Init.Prescaler = TIM4_PSC;			// by default 7199 for ETR mode
		htim4.Init.Period = TIM4_ARR;					// by default 999 for ETR mode
	}else if((counter.state == COUNTER_IC) || counter.state == COUNTER_TI){
		/* IC mode - 100 ms interrupt event to send data */
		htim4.Init.Prescaler = TIM4_PSC;
		htim4.Init.Period = TIM4_ARR;
	}
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim4);

	if(counter.state == COUNTER_REF){
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
		sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
		sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
		sClockSourceConfig.ClockFilter = 0;
	}else{
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	}
	HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	if((counter.state == COUNTER_IC) || (counter.state == COUNTER_TI)){
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	}else{
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	}
	HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);
}

/* TIM2 mode ETR init function */
/**
 * @brief  TIM2 Configuration for Direct and Reference methods.
 * @note   Direct (ETR): Directly counts edges. The gate opened according to TIM4.
 * @note   Reference (REF): Counting an external clock source. The time of counting is given by the frequency of an external clk source on TIM4 and its ARR*PSC (gating).
 * @param  None
 * @retval None
 */
void MX_TIM2_ETRorREF_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_SlaveConfigTypeDef sSlaveConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0xFFFFFFFF;		// full 32 bit 4 294 967 295
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim2);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
	sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
	sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
	sClockSourceConfig.ClockFilter = 0;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_COMBINED_RESETTRIGGER;
	sSlaveConfig.InputTrigger = TIM_TS_ITR3;
	sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
	sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
	HAL_TIM_SlaveConfigSynchronization(&htim2, &sSlaveConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_UPDATE);

	htim2.Instance->CCMR1 &= ~TIM_CCMR1_CC2S;
	htim2.Instance->CCMR1 |= TIM_CCMR1_CC1S;     /* Capture/Compare 1 Selection - CC1 channel is configured as input, IC1 is mapped on TRC */
}

/* TIM2 mode IC init function */
/**
 * @brief  TIM2 Configuration for Reciprocal and Time Interval methods.
 * @note   Reciprocal (IC): TIM2 is counting the internal clk and its value is stored into RAM whenever an event occurs on the channel (incoming edge).
 * @note   Time Interval (TI): TIM2 is reset + triggered by an incoming event on the 1st channel. The next incoming event on 2nd channel stores the value of TIM2 to RAM.
 * @param  None
 * @retval None
 */
void MX_TIM2_ICorTI_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_IC_InitTypeDef sConfigIC;

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0xFFFFFFFF;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim2);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

	HAL_TIM_IC_Init(&htim2);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1);
	HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2);

	/* Capture/Compare 1 Selection - CC1 channel is configured as input, IC1 is mapped on TI1, IC2 is mapped on TI2	*/
	htim2.Instance->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
	/* CC1 channel configured as input: This bit determines if a capture of the counter value can
	 *  actually be done into the input capture/compare register 1 (TIMx_CCR1) or not.  */
	htim2.Instance->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);

	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1); /* Capture/Compare 1 DMA request */
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC2); /* Capture/Compare 1 DMA request */
}

/**
 * @}
 */

/** @defgroup Counter_Timer_MSPInit_Functions Counter Timer MSP (De)Init Functions
 * @{
 */
void TIM2_CNT_ETRorREF_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_TIM2_CLK_ENABLE();

	/**TIM2 GPIO Configuration
	PA0     ------> TIM2_ETR
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Peripheral DMA init*/

	hdma_tim2_up.Instance = DMA1_Channel2;
	hdma_tim2_up.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim2_up.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim2_up.Init.MemInc = DMA_MINC_DISABLE;
	hdma_tim2_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_tim2_up.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_tim2_up.Init.Mode = DMA_CIRCULAR;
	hdma_tim2_up.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&hdma_tim2_up);

	__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim2_up);
	HAL_DMA_RegisterCallback(&hdma_tim2_up, HAL_DMA_XFER_CPLT_CB_ID, COUNTER_ETR_DMA_CpltCallback);

	/* DMA1_Channel2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 9, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

	counterEtrRefSetDefault();
}

void TIM2_CNT_ICorTI_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_TIM2_CLK_ENABLE();

	/**TIM2 GPIO Configuration
	PA0     ------> TIM2_CH1
	PA1     ------> TIM2_CH2
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Peripheral DMA init*/

	hdma_tim2_ch2_ch4.Instance = DMA1_Channel7;
	hdma_tim2_ch2_ch4.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim2_ch2_ch4.Init.PeriphInc = DMA_PINC_DISABLE;
	if(counter.state==COUNTER_IC){
		hdma_tim2_ch2_ch4.Init.MemInc = DMA_MINC_ENABLE;
	}else{
		hdma_tim2_ch2_ch4.Init.MemInc = DMA_MINC_DISABLE;
	}
	hdma_tim2_ch2_ch4.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_tim2_ch2_ch4.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_tim2_ch2_ch4.Init.Mode = DMA_NORMAL;
	hdma_tim2_ch2_ch4.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&hdma_tim2_ch2_ch4);

	/* Several peripheral DMA handle pointers point to the same DMA handle.
	 Be aware that there is only one channel to perform all the requested DMAs. */
	__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC2],hdma_tim2_ch2_ch4);
	//			__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC4],hdma_tim2_ch2_ch4);

	hdma_tim2_ch1.Instance = DMA1_Channel5;
	hdma_tim2_ch1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim2_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
	if(counter.state==COUNTER_IC){
		hdma_tim2_ch1.Init.MemInc = DMA_MINC_ENABLE;
	}else{
		hdma_tim2_ch1.Init.MemInc = DMA_MINC_DISABLE;
	}
	hdma_tim2_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_tim2_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_tim2_ch1.Init.Mode = DMA_NORMAL;
	hdma_tim2_ch1.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&hdma_tim2_ch1);

	__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC1],hdma_tim2_ch1);

	counterIcTiSetDefault();
}

void TIM4_CNT_REForICorTI_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_TIM4_CLK_ENABLE();

	if(counter.state==COUNTER_REF){

		/**TIM4 GPIO Configuration
		PA8     ------> TIM4_ETR_REF (as reference)
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_TIM4;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	}else if(counter.state==COUNTER_IC||counter.state==COUNTER_TI){

		HAL_NVIC_SetPriority(TIM4_IRQn, 9, 0);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
	}
}

void TIM2_CNT_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM2_CLK_DISABLE();

	if(counter.state==COUNTER_ETR||counter.state==COUNTER_REF){

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);		/* TIM2 GPIO Configuration PA0 -> TIM2_ETR */
		HAL_NVIC_DisableIRQ(DMA1_Channel2_IRQn);
		HAL_DMA_UnRegisterCallback(&hdma_tim2_up, HAL_DMA_XFER_CPLT_CB_ID);
		HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_UPDATE]);

	}else if(counter.state==COUNTER_IC||counter.state == COUNTER_TI){

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);
		HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC2]);
		HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC1]);
	}

	__HAL_RCC_TIM2_FORCE_RESET();
	__HAL_RCC_TIM2_RELEASE_RESET();
}

void TIM4_CNT_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM4_CLK_DISABLE();

	if(counter.state==COUNTER_REF){
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);

	} else if(counter.state==COUNTER_IC||counter.state==COUNTER_TI){
		HAL_NVIC_DisableIRQ(TIM4_IRQn);

	}	else if(counter.state==COUNTER_ETR){
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
	}

	__HAL_RCC_TIM4_FORCE_RESET();
	__HAL_RCC_TIM4_RELEASE_RESET();
}

/**
 * @}
 */

/** @defgroup Counter_Timer_Callbacks Counter Timer Callback Functions.
 * @{
 */

/**
 * @brief  Period of gate time elapsed callback.
 * @note	  TIM4 ISR called whenever gate time elapses.
 * @params htim:	TIM handler
 * @retval None
 */
void COUNTER_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_UPDATE) !=RESET)
		{
			__HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
			counterPeriodElapsedCallback(htim);
		}
	}
}

/**
 * @}
 */

/** @defgroup Counter_Timer_Functions Counter Timer Functions.
 * @{
 */

/* ************************************************************************************** */
/* ---------------------------- Counter timer INIT functions ---------------------------- */
/* ************************************************************************************** */
/**
 * @brief  Initialize Counter Direct method (ETR input).
 * @note		TIM4 gate time; TIM2 counting an external signal.
 * @params None
 * @retval None
 */
void TIM_counter_etr_init(void){
	/* Initialized as a first mode */
	__HAL_RCC_TIM2_FORCE_RESET();
	__HAL_RCC_TIM2_RELEASE_RESET();
	__HAL_RCC_TIM4_FORCE_RESET();
	__HAL_RCC_TIM4_RELEASE_RESET();

	TIM_doubleClockVal();
	MX_TIM4_Init();
	MX_TIM2_ETRorREF_Init();
	counter.tim4PrphClk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM34);
}

/**
 * @brief  Initialize Counter Reference method.
 * @note		TIM4 counts predefined number of ticks of an external signal (gate opened).
 * @note 	TIM2 counting an external signal.
 * @params None
 * @retval None
 */
void TIM_counter_ref_init(void){
	TIM_doubleClockVal();
	MX_TIM4_Init();
	MX_TIM2_ETRorREF_Init();
}

/**
 * @brief  Initialize Counter Reciprocal method (Input Capture).
 * @note		TIM4 channel periodically triggers ISR to check whether the sampling is done.
 * @note 	TIM2 counting and storing time (count) of an incomming edge to the memory.
 * @params None
 * @retval None
 */
void TIM_counter_ic_init(void){
	TIM_doubleClockVal();
	MX_TIM4_Init();
	MX_TIM2_ICorTI_Init();
}

/**
 * @brief  Initialize Counter Time Interval method.
 * @note		TIM4 channel periodically triggers ISR to check whether the sampling is done.
 * @note 	TIM2 triggered by 1st channel event. The 2nd channel stores the time of incoming 2nd event.
 * @params None
 * @retval None
 */
void TIM_counter_ti_init(void){
	TIM_doubleClockVal();
	MX_TIM4_Init();
	MX_TIM2_ICorTI_Init();
	TIM_TI_Init();
}

/* HAL_RCCEx_GetPeriphCLKFreq function does not count with PLL clock source for TIM2 */
/**
 * @brief  Overclocks / doubles TIM2 peripheral frequency for Counter.
 * @note		HAL function for overclocking TIM2 does not work.
 * @params None
 * @retval None
 */
void TIM_doubleClockVal(void){
	if (RCC->CFGR3&RCC_TIM2CLK_PLLCLK){
		counter.tim2PrphClk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM2) * 2;
	}	else {
		counter.tim2PrphClk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM2);
	}
}

/* ************************************************************************************** */
/* --------------------------- Counter timer DEINIT functions --------------------------- */
/* ************************************************************************************** */
/**
 * @brief  Deinits Counter Direct method.
 * @params None
 * @retval None
 */
void TIM_etr_deinit(void){
	HAL_TIM_Base_DeInit(&htim2);
	HAL_TIM_Base_DeInit(&htim4);
}

/**
 * @brief  Deinits Counter Reference method.
 * @params None
 * @retval None
 */
void TIM_ref_deinit(void){
	HAL_TIM_Base_DeInit(&htim2);
	HAL_TIM_Base_DeInit(&htim4);
}

/**
 * @brief  Deinits Counter Reciprocal method.
 * @params None
 * @retval None
 */
void TIM_ic_deinit(void){
	HAL_TIM_Base_DeInit(&htim2);
	HAL_TIM_Base_DeInit(&htim4);
}

/**
 * @brief  Deinits Counter Time Interval method.
 * @params None
 * @retval None
 */
void TIM_ti_deinit(void){
	HAL_TIM_Base_DeInit(&htim2);
	HAL_TIM_Base_DeInit(&htim4);
	TIM_TI_Deinit();
}

/* ************************************************************************************** */
/* ---------------------- Counter timer mode START STOP functions ----------------------- */
/* ************************************************************************************** */
/**
 * @brief  Starts Counter Direct method.
 * @params None
 * @retval None
 */
void TIM_ETR_Start(void)
{
	TIM_ClearFlagsIT(&htim2);
	TIM_ClearFlagsIT(&htim4);
	DMA_TransferComplete(&hdma_tim2_up);

	/* DMA requests enable */
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);
	TIM_CCxChannelCmd(htim2.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

	HAL_DMA_Start_IT(&hdma_tim2_up, (uint32_t)&htim2.Instance->CCR1, (uint32_t)&counter.counterEtr.buffer, 1);
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start(&htim4);

	LL_TIM_GenerateEvent_UPDATE(htim4.Instance);

	counter.sampleCntChange = SAMPLE_COUNT_CHANGED;
}

/**
 * @brief  Stops Counter Direct method.
 * @params None
 * @retval None
 */
void TIM_ETR_Stop(void)
{
	HAL_DMA_Abort_IT(&hdma_tim2_up);
	__HAL_TIM_DISABLE_DMA(&htim2, TIM_DMA_CC1);

	HAL_TIM_Base_Stop(&htim2);
	HAL_TIM_Base_Stop(&htim4);
}

/**
 * @brief  Starts Counter Reciprocal method.
 * @params None
 * @retval None
 */
void TIM_IC_Start(void)
{
	/* DMA requests enable */
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC2);

	HAL_DMA_Start(&hdma_tim2_ch1, (uint32_t)&(htim2.Instance->CCR1), (uint32_t)counter.counterIc.ic1buffer, counter.counterIc.ic1BufferSize);
	HAL_DMA_Start(&hdma_tim2_ch2_ch4, (uint32_t)&(htim2.Instance->CCR2), (uint32_t)counter.counterIc.ic2buffer, counter.counterIc.ic2BufferSize);

	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start_IT(&htim4);

	/* Enable capturing on both channels */
	htim2.Instance->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);
}

/**
 * @brief  Stops Counter Reciprocal method.
 * @params None
 * @retval None
 */
void TIM_IC_Stop(void)
{
	/* Disable capturing */
	htim2.Instance->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E);

	/* Abort DMA transfers */
	HAL_DMA_Abort(&hdma_tim2_ch1);
	HAL_DMA_Abort(&hdma_tim2_ch2_ch4);
	//	HAL_TIM_Base_Stop_DMA(&htim2);

	/* DMA requests disable */
	__HAL_TIM_DISABLE_DMA(&htim2, TIM_DMA_CC1);
	__HAL_TIM_DISABLE_DMA(&htim2, TIM_DMA_CC2);

	/* Stop timers */
	HAL_TIM_Base_Stop_IT(&htim4);
	HAL_TIM_Base_Stop(&htim2);
}

/**
 * @brief  Starts Counter Time Interval method.
 * @params None
 * @retval None
 */
void TIM_TI_Start(void)
{
	/* Get systick value to handle timeout */
	xStartTime = xTaskGetTickCount();

	/* There are two modes:

		1)  FAST DEPENDENT MODE - Can measure time between two quick consecutive events. Maximal
				resolution is given by time base of TIM2 (for NUCLEO-STM32F303RE it's 1 / 144 MHz).
				One channel is configured to reset timer's CNT on required event (rising or falling edge).
				An event that occurs on second channel triggers DMA transfer to save CNT value.

				This approach encounters one major problem - if an edge on the second channel comes first
				the DMA transfer is triggered before the CNT is reset (and retriggered - combined slave mode).
				That means a wrong value is saved and the computed time is not valid. Therefore, user has
				to ensure that events come in the required sequence (the edge incoming on the channel that
				is cofigured to reset CNT has to transit first). This problem is handled by INDEPENDENT MODE.


		2)	INDEPENDENT MODE - Is able to give correct answers without ensuring of required events sequence.
				The event that is expected to come first (one that resets CNT) may come after the one that trigger
				DMA. The reason is, in this mode, DMA transfer of second channel is not enabled until first event
				(that should reset CNT) occures. When the first event resets CNT a DMA transfer configuring CCER
				register of TIM2 is triggered to enable capturing on the second channel. When this DMA transfer is
				processed the second event can be captured.

				It's nice, but if two quick consecutive edges come and the DMA transfer enabling capturing is not
				processed yet, the edge is not captured.
	 */

	/* Set DMA CNDTR buffer count */
	if(counter.abba == BIN1){
		/* Set DMA to transfer time of event on channel 1 after TIM CNT is reset by an event on channel 2 */
		HAL_DMA_Start(&hdma_tim2_ch1, (uint32_t)&(htim2.Instance->CCR1), (uint32_t)counter.counterIc.ic1buffer, 1);

		if(counter.tiMode==TI_MODE_EVENT_SEQUENCE_INDEP){
			if(counter.eventChan1==EVENT_FALLING){
				timCcerRegCc1eVal |= (uint32_t)TIM_CCER_CC1P;
			}else{
				timCcerRegCc1eVal &= (uint32_t)~TIM_CCER_CC1P;
			}
			TIM_TI_ReconfigActiveEdges();
			/* Set DMA to enable capturing of the channel 1 after an event comes on channel 2. This Method
			prevents capturing an event if no event came first on the desired channel. */
			HAL_DMA_Start(&hdma_tim2_ch2_ch4, (uint32_t)&timCcerRegCc1eVal, (uint32_t)&(htim2.Instance->CCER), 1);  //TIM2_CCER_ADDR
			/* Disable Capturing on channel 1 to be enabled later after an event on channel 2 comes. */
			htim2.Instance->CCER &= ~TIM_CCER_CC1E;
			/* Enable Capturing on channel 2. */
			htim2.Instance->CCER |= TIM_CCER_CC2E;
		}

	}else{
		/* Set DMA to transfer time of event on channel 2 after TIM CNT is reset by an event on channel 1 */
		HAL_DMA_Start(&hdma_tim2_ch2_ch4, (uint32_t)&(htim2.Instance->CCR2), (uint32_t)counter.counterIc.ic2buffer, 1);

		if(counter.tiMode==TI_MODE_EVENT_SEQUENCE_INDEP){
			if(counter.eventChan2==EVENT_FALLING){
				timCcerRegCc2eVal |= (uint32_t)TIM_CCER_CC2P;
			}else{
				timCcerRegCc2eVal &= (uint32_t)~TIM_CCER_CC2P;
			}
			TIM_TI_ReconfigActiveEdges();
			/* Set DMA to enable capturing of the channel 2 after an event comes on channel 1. This Method
			prevents capturing an event if no event came first on the desired channel. */
			HAL_DMA_Start(&hdma_tim2_ch1, (uint32_t)&timCcerRegCc2eVal, (uint32_t)&(htim2.Instance->CCER), 1);  //TIM2_CCER_ADDR
			/* Disable Capturing on channel 2 to be enabled later after an event on channel 1 comes. */
			htim2.Instance->CCER &= ~TIM_CCER_CC2E;
			/* Enable Capturing on channel 1. */
			htim2.Instance->CCER |= TIM_CCER_CC1E;
		}
	}

	/* TIM2 is used as time base for time capturing. */
	HAL_TIM_Base_Start(&htim2);
	/* TIM4 is used for time elapse event to check whether already the required data is transfered. */
	HAL_TIM_Base_Start_IT(&htim4);

	/* DMA requests enable */
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC2);

	if(counter.tiMode!=TI_MODE_EVENT_SEQUENCE_INDEP){
		/* Enable capturing */
		htim2.Instance->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);
	}
}

/**
 * @brief  Stops Counter Reciprocal method.
 * @params None
 * @retval None
 */
void TIM_TI_Stop(void)
{
	/* Abort DMA transfers */
	HAL_DMA_Abort(&hdma_tim2_ch1);
	HAL_DMA_Abort(&hdma_tim2_ch2_ch4);
	//	HAL_TIM_Base_Stop_DMA(&htim2);

	HAL_TIM_Base_Stop_IT(&htim4);
	HAL_TIM_Base_Stop(&htim2);

	/* Disable capturing */
	htim2.Instance->CCER &= ~TIM_CCER_CC1E;
	htim2.Instance->CCER &= ~TIM_CCER_CC2E;

	/* DMA requests disable */
	__HAL_TIM_DISABLE_DMA(&htim2, TIM_DMA_CC1);
	__HAL_TIM_DISABLE_DMA(&htim2, TIM_DMA_CC2);
}

/**
 * @brief  Initializes Counter Time Interval method.
 * @params None
 * @retval None
 */
void TIM_TI_Init(void)
{
	/* Do not run timer after initialization, wait for start command */
	htim2.Instance->CR1 &= ~TIM_CR1_CEN;
	/* Disable time elapse interrupt */
	HAL_TIM_Base_Stop_IT(&htim4);
	/* Disable capturing */
	htim2.Instance->CCER &= ~TIM_CCER_CC1E;
	htim2.Instance->CCER &= ~TIM_CCER_CC2E;
	/* Set IC1 prescaler to 1 */
	htim2.Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;
	/* Set IC2 prescaler to 1 */
	htim2.Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC;
	/* Select the valid trigger input TI1FP1 */
	htim2.Instance->SMCR &= ~TIM_SMCR_TS;
	htim2.Instance->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_TS_2;
	/* Configure the slave mode controller in Combined reset + trigger mode */
	htim2.Instance->SMCR &= ~TIM_SMCR_SMS;
	htim2.Instance->SMCR |= TIM_SMCR_SMS_3;

	/* The very first number transfered by DMA on first event (timer triggered)
		 is random number -> throw away */
	counter.bin = BIN0;
	/* AB event sequence first */
	counter.abba = BIN0;
}

/**
 * @brief  Deinits Counter Time Interval method.
 * @params None
 * @retval None
 */
void TIM_TI_Deinit(void)
{
	/* Disable capturing*/
	htim2.Instance->CCER &= ~TIM_CCER_CC1E;
	htim2.Instance->CCER &= ~TIM_CCER_CC2E;
	/* Select the active polarity for TI1FP1 (rising edge) */
	htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC1P | TIM_CCER_CC1NP);
	/* Select the active polarity for TI1FP2 (rising edge) */
	htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC2P | TIM_CCER_CC2NP);
	/* Unselect the trigger input */
	htim2.Instance->SMCR &= ~TIM_SMCR_TS;
	/* Disable the slave mode controller */
	htim2.Instance->SMCR &= ~TIM_SMCR_SMS;
}


/* ************************************************************************************** */
/* ------------------------------ IC duty cycle functions ------------------------------- */
/* ************************************************************************************** */
/**
 * @brief  Reconfigures DMA for duty cycle measurement under Counter Reciprocal method.
 * @params None
 * @retval None
 */
void TIM_IC_DutyCycleDmaRestart(void)
{
	HAL_DMA_Abort(&hdma_tim2_ch1);
	HAL_DMA_Abort(&hdma_tim2_ch2_ch4);

	/* Set DMA CNDTR buffer count */
	HAL_DMA_Start(&hdma_tim2_ch1, (uint32_t)&(htim2.Instance->CCR1), (uint32_t)counter.counterIc.ic1buffer, 1);
	HAL_DMA_Start(&hdma_tim2_ch2_ch4, (uint32_t)&(htim2.Instance->CCR2), (uint32_t)counter.counterIc.ic2buffer, 1);
}

/**
 * @brief  Initializes duty cycle measurement under Counter Reciprocal method.
 * @params None
 * @retval None
 */
void TIM_IC_DutyCycle_Init(void)
{
	/* Stop timer elapse event interrupt first */
	HAL_TIM_Base_Stop_IT(&htim4);

	/* Disable capturing to configure CCxS */
	htim2.Instance->CCER &= ~TIM_CCER_CC1E;
	htim2.Instance->CCER &= ~TIM_CCER_CC2E;

	if(counter.icDutyCycle == DUTY_CYCLE_CH1_ENABLED){
		/* Set IC1 prescaler to 1 */
		htim2.Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;
		/* Select the active input for CCR1 */
		htim2.Instance->CCMR1 &= ~TIM_CCMR1_CC1S;
		htim2.Instance->CCMR1 |= TIM_CCMR1_CC1S_0;
		/* Select the active polarity for TI1FP1 (rising edge) */
		htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC1P | TIM_CCER_CC1NP);
		/* Select the active input for CCR2 */
		htim2.Instance->CCMR1 &= ~TIM_CCMR1_CC2S;
		htim2.Instance->CCMR1 |= TIM_CCMR1_CC2S_1;
		/* Select the active polarity for TI1FP2 (falling edge) */
		htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC2NP);
		htim2.Instance->CCER |= (uint16_t)(TIM_CCER_CC2P);
		/* Select the valid trigger input */
		htim2.Instance->SMCR &= ~TIM_SMCR_TS;
		htim2.Instance->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_TS_2;
	}else{
		/* Set IC2 prescaler to 1 */
		htim2.Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC;
		/* Select the active input for CCR1 */
		htim2.Instance->CCMR1 &= ~TIM_CCMR1_CC1S;
		htim2.Instance->CCMR1 |= TIM_CCMR1_CC1S_1;
		/* Select the active polarity for TI1FP1 (falling edge) */
		htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC1NP);
		htim2.Instance->CCER |= (uint16_t)(TIM_CCER_CC1P);
		/* Select the active input for CCR2 */
		htim2.Instance->CCMR1 &= ~TIM_CCMR1_CC2S;
		htim2.Instance->CCMR1 |= TIM_CCMR1_CC2S_0;
		/* Select the active polarity for TI1FP2 (rising edge) */
		htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC2P | TIM_CCER_CC2NP);
		/* Select the valid trigger input */
		htim2.Instance->SMCR &= ~TIM_SMCR_TS;
		htim2.Instance->SMCR |= TIM_SMCR_TS_1 | TIM_SMCR_TS_2;
	}

	/* Configure the slave mode controller in reset mode */
	htim2.Instance->SMCR &= ~TIM_SMCR_SMS;
	htim2.Instance->SMCR |= TIM_SMCR_SMS_2;
}

/**
 * @brief  Deinits duty cycle measurement under Counter Reciprocal method.
 * @params None
 * @retval None
 */
void TIM_IC_DutyCycle_Deinit(void)
{
	/* Select the active input for CCR1 */
	htim2.Instance->CCMR1 &= ~TIM_CCMR1_CC1S;
	htim2.Instance->CCMR1 |= TIM_CCMR1_CC1S_0;
	/* Select the active polarity for TI1FP1 (rising edge) */
	htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC1P | TIM_CCER_CC1NP);
	/* Select the active input for CCR2 */
	htim2.Instance->CCMR1 &= ~TIM_CCMR1_CC2S;
	htim2.Instance->CCMR1 |= TIM_CCMR1_CC2S_0;
	/* Select the active polarity for TI1FP2 (rising edge) */
	htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC2P | TIM_CCER_CC2NP);
	/* Unselect the trigger input */
	htim2.Instance->SMCR &= ~TIM_SMCR_TS;
	/* Disable the slave mode controller */
	htim2.Instance->SMCR &= ~TIM_SMCR_SMS;
	/* Start DMAs */
	HAL_DMA_Start(&hdma_tim2_ch1, (uint32_t)&(htim2.Instance->CCR1), (uint32_t)counter.counterIc.ic1buffer, counter.counterIc.ic1BufferSize);
	HAL_DMA_Start(&hdma_tim2_ch2_ch4, (uint32_t)&(htim2.Instance->CCR2), (uint32_t)counter.counterIc.ic2buffer, counter.counterIc.ic2BufferSize);
	/* DMA requests enable */
	htim2.Instance->DIER |= TIM_DIER_CC1DE;
	htim2.Instance->DIER |= TIM_DIER_CC2DE;
	HAL_TIM_Base_Start_IT(&htim4);
	/* Enable capturing for IC mode */
	htim2.Instance->CCER |= TIM_CCER_CC1E;
	htim2.Instance->CCER |= TIM_CCER_CC2E;
}

/**
 * @brief  Starts duty cycle measurement under Counter Reciprocal method.
 * @params None
 * @retval None
 */
void TIM_IC_DutyCycle_Start(void)
{
	/* Set DMA CNDTR buffer count */
	HAL_DMA_Start(&hdma_tim2_ch1, (uint32_t)&(htim2.Instance->CCR1), (uint32_t)counter.counterIc.ic1buffer, 1);
	HAL_DMA_Start(&hdma_tim2_ch2_ch4, (uint32_t)&(htim2.Instance->CCR2), (uint32_t)counter.counterIc.ic2buffer, 1);

	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start_IT(&htim4);

	/* DMA requests enable */
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);
	__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC2);

	/* Enable capturing */
	htim2.Instance->CCER |= TIM_CCER_CC2E;
	htim2.Instance->CCER |= TIM_CCER_CC1E;

	/* The very first number transfered by DMA on first event (timer triggered)
		 is random number (who knows why) -> throw away */
	counter.bin = BIN0;
}

/**
 * @brief  Stops duty cycle measurement under Counter Reciprocal method.
 * @params None
 * @retval None
 */
void TIM_IC_DutyCycle_Stop(void)
{
	/* Abort DMA transfers */
	HAL_DMA_Abort(&hdma_tim2_ch1);
	HAL_DMA_Abort(&hdma_tim2_ch2_ch4);

	/* Disable capture to configure CCxS */
	htim2.Instance->CCER &= ~TIM_CCER_CC1E;
	htim2.Instance->CCER &= ~TIM_CCER_CC2E;

	/* DMA requests disable */
	__HAL_TIM_DISABLE_DMA(&htim2, TIM_DMA_CC1);
	__HAL_TIM_DISABLE_DMA(&htim2, TIM_DMA_CC2);
}

/* ************************************************************************************** */
/* ----------------------------- Specific counter functions ----------------------------- */
/* ************************************************************************************** */
/**
 * @brief  Selects the desired ETR prescaler ETPS in Counter. (TIM2 should be clocked to 144 MHz)
 * @note		Used for automatic ETR presacaler configuration. The ETR input may be fed only by cca 36 MHz.
 *					Therefore if higher input freq. then Prescaler must be higher as well.
 * @param  freq: frequency
 * @retval none
 */
void TIM_ETRP_Config(double freq)
{
	uint32_t smcr = htim2.Instance->SMCR;
	/* Check the range of the input frequency and set the ETR prescaler */
	if(freq < (counter.tim2PrphClk / 4)){
		htim2.Instance->SMCR &= ~TIM_SMCR_ETPS;													/* Set ETR prescaler to 1 */

	} else if ((freq >= (counter.tim2PrphClk / 4)) && freq < ((counter.tim2PrphClk / 2))){
		if ((smcr & TIM_SMCR_ETPS) != TIM_SMCR_ETPS_0){
			htim2.Instance->SMCR &= ~TIM_SMCR_ETPS;
			htim2.Instance->SMCR |= TIM_SMCR_ETPS_0;												/* Set ETR prescaler to 2 */
		}
	} else if ((freq >= (counter.tim2PrphClk / 2)) && (freq < (counter.tim2PrphClk))) {
		if ((smcr & TIM_SMCR_ETPS) != TIM_SMCR_ETPS_1){
			htim2.Instance->SMCR &= ~TIM_SMCR_ETPS;
			htim2.Instance->SMCR |= TIM_SMCR_ETPS_1;												/* Set ETR prescaler to 4 */
		}
	} else {
		if ((smcr & TIM_SMCR_ETPS) != TIM_SMCR_ETPS){
			htim2.Instance->SMCR &= ~TIM_SMCR_ETPS;
			htim2.Instance->SMCR |= TIM_SMCR_ETPS;													/* Set ETR prescaler to 8 */
		}
	}
}

/**
 * @brief  Selects the required prescaler of IC1 of TIM2 in Counter.
						Direct change of prescaler according to value given to function as parameter.
 * @param  prescVal: value of prescaler (1, 2, 4, 8)
 * @retval none
 */
void TIM_IC1_PSC_Config(uint8_t prescVal)
{
	htim2.Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;
	/* Save the real value of ICxPSC prescaler for later calculations */
	switch(prescVal){
	case 2:
		htim2.Instance->CCMR1 |= TIM_CCMR1_IC1PSC_0; break;
	case 4:
		htim2.Instance->CCMR1 |= TIM_CCMR1_IC1PSC_1; break;
	case 8:
		htim2.Instance->CCMR1 |= TIM_CCMR1_IC1PSC; break;
	default:
		htim2.Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC; break;
	}
}

/**
 * @brief  Selects the required prescaler of IC2 of TIM2 in Counter.
						Direct change of prescaler according to value given to function as parameter.
 * @param  prescVal: value of prescaler (1, 2, 4, 8)
 * @retval none
 */
void TIM_IC2_PSC_Config(uint8_t prescVal)
{
	htim2.Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC;
	/* Save the real value of ICxPSC prescaler for later calculations */
	switch(prescVal){
	case 2:
		htim2.Instance->CCMR1 |= TIM_CCMR1_IC2PSC_0; break;
	case 4:
		htim2.Instance->CCMR1 |= TIM_CCMR1_IC2PSC_1; break;
	case 8:
		htim2.Instance->CCMR1 |= TIM_CCMR1_IC2PSC; break;
	default:
		htim2.Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC; break;
	}
}

/**
 * @brief  Selects rising falling edges to be captured in Counter IC + Duty cycle & TI modes channel 1
 * @param  none
 * @retval none
 */
void TIM_IC1_RisingFalling(void)
{
	htim2.Instance->CCER |= (TIM_CCER_CC1P | TIM_CCER_CC1NP);
}

/**
 * @brief  Selects rising edge to be captured in Counter IC + Duty cycle & TI modes channel 1
 * @param  none
 * @retval none
 */
void TIM_IC1_RisingOnly(void)
{
	htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC1P | TIM_CCER_CC1NP);
}

/**
 * @brief  Selects falling edge to be captured in Counter IC + Duty cycle & TI modes channel 1
 * @param  none
 * @retval none
 */
void TIM_IC1_FallingOnly(void)
{
	htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC1NP);
	htim2.Instance->CCER |= (uint16_t)(TIM_CCER_CC1P);
}

/**
 * @brief  Selects rising falling edge to be captured in Counter IC + Duty cycle & TI modes channel 2
 * @param  none
 * @retval none
 */
void TIM_IC2_RisingFalling(void)
{
	htim2.Instance->CCER |= (TIM_CCER_CC2P | TIM_CCER_CC2NP);
}

/**
 * @brief  Selects rising edge to be captured in Counter IC + Duty cycle & TI modes channel 2
 * @param  none
 * @retval none
 */
void TIM_IC2_RisingOnly(void)
{
	htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC2P | TIM_CCER_CC2NP);
}

/**
 * @brief  Selects falling edge to be captured in Counter IC + Duty cycle & TI modes channel 2
 * @param  none
 * @retval none
 */
void TIM_IC2_FallingOnly(void)
{
	htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC2NP);
	htim2.Instance->CCER |= (uint16_t)(TIM_CCER_CC2P);
}

/**
 * @brief  Selects AB sequence in Time Interval of Couter measurement.
 * @param  none
 * @retval none
 */
void TIM_TI_Sequence_AB(void){
	/* Select the valid trigger input TI1FP1 */
	htim2.Instance->SMCR &= ~TIM_SMCR_TS;
	htim2.Instance->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_TS_2;
	/* Configure the slave mode controller in Combined reset + trigger mode */
	htim2.Instance->SMCR &= ~TIM_SMCR_SMS;
	htim2.Instance->SMCR |= TIM_SMCR_SMS_3;
	/* ABBA used for calculation decision in counterTiProcess() function.
		 Time t_AB - time delay between AB events measured. */
	counter.abba = BIN0;
}

/**
 * @brief  Selects BA sequence in Time Interval of Couter measurement.
 * @param  none
 * @retval none
 */
void TIM_TI_Sequence_BA(void){
	/* Select the valid trigger input TI2FP2 */
	htim2.Instance->SMCR &= ~TIM_SMCR_TS;
	htim2.Instance->SMCR |= TIM_SMCR_TS_1 | TIM_SMCR_TS_2;
	/* Configure the slave mode controller in Combined reset + trigger mode */
	htim2.Instance->SMCR &= ~TIM_SMCR_SMS;
	htim2.Instance->SMCR |= TIM_SMCR_SMS_3;
	/* ABBA used for calculation decision in counterTiProcess() function.
		 Time t_BA - time delay between BA events measured. */
	counter.abba = BIN1;
}

/**
 * @brief  Counter Time Interval measurement active edges reconfiguration function.
 * @param  none
 * @retval none
 */
void TIM_TI_ReconfigActiveEdges(void)
{
	if(counter.eventChan1==EVENT_RISING){
		htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC1P | TIM_CCER_CC1NP);
	}else{
		htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC1NP);
		htim2.Instance->CCER |= (uint16_t)(TIM_CCER_CC1P);
	}

	if(counter.eventChan2==EVENT_RISING){
		htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC2P | TIM_CCER_CC2NP);
	}else{
		htim2.Instance->CCER &= ~(uint16_t)(TIM_CCER_CC2NP);
		htim2.Instance->CCER |= (uint16_t)(TIM_CCER_CC2P);
	}
}

/**
 * @brief  Function settings ARR and PSC values of TIM4 (gate time) - ETR, REF
 * @params arr, psc
 * @retval none
 */
void TIM_ETR_ARR_PSC_Config(double gateTime)
{
	uint32_t periphClock = HAL_RCC_GetPCLK1Freq()*2;

	TIM_ReconfigPrecise(&htim4, periphClock, 1/gateTime);

	counter.counterEtr.arr = htim4.Instance->ARR;
	counter.counterEtr.psc = htim4.Instance->PSC;
	if(counter.state!=COUNTER_IC){
		xStartTime = xTaskGetTickCount();
		__HAL_TIM_ENABLE(&htim4);
		counter.sampleCntChange = SAMPLE_COUNT_CHANGED;
	}
}

void TIM_REF_SecondInputDisable(void){
	__HAL_TIM_DISABLE(&htim4);
}

void TIM_REF_Reconfig_cnt(uint32_t sampleCount)
{
	uint32_t dummy;
	uint32_t periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM34);
	//HAL_RCC_GetPCLK1Freq()*2;  // TIM4 periph clock

	xStartTime = xTaskGetTickCount();
	counter.sampleCntChange = SAMPLE_COUNT_CHANGED;

	TIM_Reconfig(&htim4,periphClock,sampleCount,&dummy,false);
	HAL_TIM_Base_Start(&htim4);
}

/**
 * @brief  Function getting ETRP (external trigger source prescaler) value of TIM2.
 * @params none
 * @retval etps: ETRP prescaler register value
 */
uint8_t TIM_ETPS_GetPrescaler(void)
{
	uint16_t etpsRegVal = ((htim2.Instance->SMCR) & TIM_SMCR_ETPS) >> 12;			/* ETR prescaler register value */
	return TIM_GetPrescaler(etpsRegVal);
}

/**
 * @brief  Function getting Counter IC Prescaler of channel 1.
 * @params None
 * @retval None
 */
uint8_t TIM_IC1PSC_GetPrescaler(void)
{
	uint32_t ic1psc = ((htim2.Instance->CCMR1) & TIM_CCMR1_IC1PSC_Msk) >> TIM_CCMR1_IC1PSC_Pos;
	return TIM_GetPrescaler(ic1psc);
}

/**
 * @brief  Function getting Counter IC Prescaler of channel 2.
 * @params None
 * @retval None
 */
uint8_t TIM_IC2PSC_GetPrescaler(void)
{
	uint32_t ic2psc = ((htim2.Instance->CCMR1) & TIM_CCMR1_IC2PSC_Msk) >> TIM_CCMR1_IC2PSC_Pos;
	return TIM_GetPrescaler(ic2psc);
}

/**
 * @brief  Returns a real value of given register value prescaler in Direct mode meas. of Counter.
 * @params regPrescValue: ETRP prescaler register value
 * @retval presc: real prescaler value used for later calculations
 */
uint8_t TIM_GetPrescaler(uint32_t regPrescValue)
{
	uint8_t presc = 1;
	/* Save the real value of ICxPSC prescaler for later calculations */
	switch(regPrescValue){
	case 0:
		presc = 1; break;
	case 1:
		presc = 2; break;
	case 2:
		presc = 4; break;
	case 3:
		presc = 8; break;
	default:
		break;
	}
	return presc;
}


void TIM_ClearFlagsIT(TIM_HandleTypeDef* htim_base){
	htim_base->Instance->SR = 0;
}

/**
 * @brief  Function testing if DMA transfer complete bit is set.
 * @params dmah: pointer to a DMA_HandleTypeDef structure that contains
 *         the configuration information for the specified DMA Channel.
 * @retval bool: true, false
 */
bool DMA_TransferComplete(DMA_HandleTypeDef *dmah)
{
	uint32_t dmaIsrReg = dmah->DmaBaseAddress->ISR;

	if(dmaIsrReg & (uint32_t)(DMA_FLAG_TC1 << dmah->ChannelIndex)){
		/* Clear the transfer complete flag */
		dmah->DmaBaseAddress->IFCR = DMA_FLAG_TC1 << dmah->ChannelIndex;
		//		}
		return true;
	} else {
		return false;
	}
}

/* The value of DMA buffer size can be changed only if aborted first */
/**
 * @brief  Restarts DMA in order to be able rewrite CDTR value (number of samples to be transferred).
 * @params dmah: pointer to a DMA_HandleTypeDef structure that contains
 *         the configuration information for the specified DMA Channel.
 * @retval None
 */
void DMA_Restart(DMA_HandleTypeDef *dmah)
{
	if(dmah == &hdma_tim2_ch1){
		HAL_DMA_Abort(&hdma_tim2_ch1);
		HAL_DMA_Start(&hdma_tim2_ch1, (uint32_t)&(htim2.Instance->CCR1), (uint32_t)counter.counterIc.ic1buffer, counter.counterIc.ic1BufferSize);
	}else{
		HAL_DMA_Abort(&hdma_tim2_ch2_ch4);
		HAL_DMA_Start(&hdma_tim2_ch2_ch4, (uint32_t)&(htim2.Instance->CCR2), (uint32_t)counter.counterIc.ic2buffer, counter.counterIc.ic2BufferSize);
	}
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif //USE_COUNTER
