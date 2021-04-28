/**
 *****************************************************************************
 * @file    tim_gen.c
 * @author  HeyBirdie
 * @date    July 15, 2019
 * @brief   This file contains timer functions for arbitrary generators (PWM + ADC)
 *****************************************************************************
 */

#include "tim.h"
#include "dac.h"
#include "gpio.h"
#include "mcu_config.h"
#include "generator.h"

#include "stm32f3xx_ll_tim.h"

#if defined(USE_GEN_SIGNAL) || defined(USE_GEN_PWM) || defined(USE_GEN_PATTERN)

/** @addtogroup Generator
 * @{
 */

/** @defgroup Generator_Timers Generator Timers
 * @{
 */

/** @defgroup Generator_Timer_Variables Generator Timer Variables
 * @{
 */

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim6_up;
DMA_HandleTypeDef hdma_tim7_up;

/**
 * @}
 */

/** @defgroup Arbitrary_Generator_Timer_Init_Functions Arbitrary Generator Timer Initialization Functions
 * @{
 */

/**
 * @brief  TIM6 Configuration.
 * @note   TIM6 configuration is based on APB1 frequency
 * @note   TIM6 Update event occurs each TIM6CLK/256
 * @param  None
 * @retval None
 */
void MX_TIM6_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;

	/*##-1- Configure the TIM peripheral #######################################*/
	/* Time base configuration */
	htim6.Instance = TIM6;

	htim6.Init.Period = 0x7FF;
	htim6.Init.Prescaler = 0;
	htim6.Init.ClockDivision = 0;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&htim6);

	/* TIM6 TRGO selection */
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);

	/*##-2- Enable TIM peripheral counter ######################################*/
	//HAL_TIM_Base_Start(&htim6);
}

/**
 * @brief  TIM6 Configuration.
 * @note   TIM6 configuration is based on APB1 frequency
 * @note   TIM6 Update event occurs each TIM6CLK/256
 * @param  None
 * @retval None
 */
void MX_TIM7_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;

	/*##-1- Configure the TIM peripheral #######################################*/
	/* Time base configuration */
	htim7.Instance = TIM7;

	htim7.Init.Period = 0x7FF;
	htim7.Init.Prescaler = 0;
	htim7.Init.ClockDivision = 0;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&htim7);

	/* TIM6 TRGO selection */
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig);

	/*##-2- Enable TIM peripheral counter ######################################*/
	//HAL_TIM_Base_Start(&htim6);
}

#ifdef USE_GEN_PWM

/**
 * @brief  TIM1 Configuration.
 * @note   TIM1 generates PWM on a given channel.
 * @param  None
 * @retval None
 */
void MX_TIM1_GEN_PWM_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

	/* ARR = 1024 (10 bit resolution in default). F303 -> PA9 -> D8 -> Channel 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 1023;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim1);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

	HAL_TIM_PWM_Init(&htim1);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 512;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);

	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);

	HAL_TIM_Base_MspInit(&htim1);
}

/**
 * @brief  TIM3 Configuration.
 * @note   TIM3 generates PWM on a given channel.
 * @param  None
 * @retval None
 */
void MX_TIM3_GEN_PWM_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	/* F303 -> PB4 -> D5 -> Channel 2  */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 511;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim3);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

	HAL_TIM_PWM_Init(&htim3);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 256;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);

	HAL_TIM_Base_MspInit(&htim3);
}

/**
 * @brief  TIM6 Configuration for arbitrary DAC & PWM generators.
 * @note   For PWM gen: TIM6 handles the periodic change of PWM duty cycle using DMA -> Memory to Capture/Compare reg. transfers.
 *					The DMA is triggered by predefined frequency in order to tranfer data from RAM (arbitrary waveform) to TIM1 CC reg.
 * @note 	For DAC gen: TIM6 handles the periodic change of DAC value acording to predefined arbitrary waveform.
 * @param  None
 * @retval None
 */
void MX_TIM6_GEN_PWM_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;

	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 0;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 0x7FF;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim6);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);

	TIM6_GEN_PWM_MspInit(&htim6);

}

/**
 * @brief  TIM7 Configuration for arbitrary DAC & PWM generators.
 * @note   For PWM gen: TIM7 handles the periodic change of PWM duty cycle using DMA -> Memory to Capture/Compare reg. transfers.
 *					The DMA is triggered by predefined frequency in order to tranfer data from RAM (arbitrary waveform) to TIM3 CC reg.
 * @note 	For DAC gen: TIM7 handles the periodic change of DAC value acording to predefined arbitrary waveform.
 * @param  None
 * @retval None
 */
void MX_TIM7_GEN_PWM_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 0;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 0x7FF;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim7);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig);

	TIM7_GEN_PWM_MspInit(&htim7);
}

#endif //USE_GEN_PWM

#ifdef USE_GEN_SIGNAL

void TIM6_GEN_DAC_MspInit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
}

void TIM7_GEN_DAC_MspInit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM7_CLK_ENABLE();
}

void TIM6_GEN_DAC_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM6_CLK_DISABLE();
}

void TIM7_GEN_DAC_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM7_CLK_DISABLE();
}

#endif //USE_GEN_SIGNAL

#ifdef USE_GEN_PWM

void TIM1_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_TIM1_CLK_ENABLE();

	/**TIM1 GPIO Configuration
		PA9     ------> TIM1_CH2
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void TIM3_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_TIM3_CLK_ENABLE();

	/**TIM3 GPIO Configuration
		PB4     ------> TIM3_CH1
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void TIM6_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM6_CLK_ENABLE();

	/* Peripheral DMA init*/
	hdma_tim6_up.Instance = DMA1_Channel3;
	hdma_tim6_up.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tim6_up.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim6_up.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim6_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim6_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim6_up.Init.Mode = DMA_CIRCULAR;
	hdma_tim6_up.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&hdma_tim6_up);
	__HAL_TIM_ENABLE_DMA(&htim6, TIM_DMA_UPDATE);
	__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim6_up);
}

void TIM7_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM7_CLK_ENABLE();

	/* Peripheral DMA init*/
	hdma_tim7_up.Instance = DMA1_Channel4;   // DMA2_Channel4
	hdma_tim7_up.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tim7_up.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim7_up.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim7_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim7_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim7_up.Init.Mode = DMA_CIRCULAR;
	hdma_tim7_up.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&hdma_tim7_up);
	__HAL_TIM_ENABLE_DMA(&htim7, TIM_DMA_UPDATE);
	__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim7_up);
}

void TIM1_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM1_CLK_DISABLE();
}

void TIM3_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM3_CLK_DISABLE();
}

void TIM6_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM6_CLK_DISABLE();
	/* Peripheral DMA DeInit*/
	HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_UPDATE]);
}

void TIM7_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_TIM7_CLK_DISABLE();
	/* Peripheral DMA DeInit*/
	HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_UPDATE]);
}

#endif //USE_GEN_PWM

/**
 * @}
 */

/* ************************************************************************************** */
/* ------------------------------- GENERATOR FUNCTIONS ---------------------------------- */
/* ************************************************************************************** */

/** @defgroup Generator_TIM_Functions Generator TIMers Functions.
 * @{
 */

/**
 * @brief  This function calls Common timer reconfiguration function TIM_Reconfig().
 * @note		Reconfigures timer as close as possible to required value (recalculates timer registers).
 * @param  samplingFreq: required frequency of the timer
 * @param  chan: channel number 0 - 1
 * @param  *realFreq: pointer to calculated real frequency
 * @retval status
 */
uint8_t TIM_Reconfig_gen(uint32_t samplingFreq,uint8_t chan,uint32_t* realFreq){
	/* RCC_PERIPHCLK_TIM6 and TIM7 defines missing in order to use with HAL_RCCEx_GetPeriphCLKFreq fun */
	uint32_t periphClock = HAL_RCC_GetPCLK1Freq()*2;
	if(chan==0){
		return TIM_Reconfig(&htim6,periphClock,samplingFreq,realFreq,true);
	}else if(chan==1){
		return TIM_Reconfig(&htim7,periphClock,samplingFreq,realFreq,true);
	}else{
		return 0;
	}
}

uint8_t TIM_Reconfig_gen_all(uint32_t samplingFreq,uint32_t* realFreq){
	uint32_t psc=0;
	uint32_t arr=0;
	uint8_t result = TIM_Getconfig(&arr, &psc, HAL_RCC_GetPCLK1Freq()*2, samplingFreq, realFreq, true);

	htim6.Instance->PSC = psc;
	htim7.Instance->PSC = psc;

	htim6.Instance->ARR = arr;
	htim7.Instance->ARR = arr;
	LL_TIM_GenerateEvent_UPDATE(htim6.Instance);
	LL_TIM_GenerateEvent_UPDATE(htim7.Instance);

	return result;
}

#ifdef USE_GEN_PWM

double TIM_Reconfig_GenPwm(double reqFreq, uint8_t chan){
	uint32_t periphClock;
	uint32_t psc=0;
	uint32_t arr=0;
	uint32_t realFreq;
	if(chan==0){
		periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM1);
		arr = (periphClock / reqFreq)-1;
		while(arr>65535){
			arr = (arr+1)/2-1;
			psc = (psc+1)*2-1;
		}
		realFreq = periphClock/(arr+1)/(psc+1);
		htim1.Instance->PSC = psc;
		htim1.Instance->ARR = arr;
		LL_TIM_GenerateEvent_UPDATE(htim1.Instance);

	}else if(chan==1){
		periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM34);
		arr = (periphClock / reqFreq)-1;
		while(arr>65535){
			arr = (arr+1)/2-1;
			psc = (psc+1)*2-1;
		}
		realFreq = periphClock/(arr+1)/(psc+1);
		htim3.Instance->PSC = psc;
		htim3.Instance->ARR = arr;
		LL_TIM_GenerateEvent_UPDATE(htim3.Instance);
	}
	return realFreq;
}

#endif //USE_GEN_PWM

/**
 * @brief  Enable TIM6 & TIM7 that trigger DMA - generating DAC.
 * @param  None
 * @retval None
 */
void TIMGenEnable(void){
	__HAL_TIM_ENABLE(&htim6);
	__HAL_TIM_ENABLE(&htim7);
}

/**
 * @brief  Disable TIM6 & TIM7 - stop triggering DMA / generating DAC.
 * @param  None
 * @retval None
 */
void TIMGenDisable(void){
	__HAL_TIM_DISABLE(&htim6);
	__HAL_TIM_DISABLE(&htim7);
}

/**
 * @brief  Initialization of arbitrary DAC generator.
 * @note 	TIM6 & TIM7 & DAC.
 * @param  None
 * @retval None
 */
void TIMGenInit(void){
	MX_TIM6_Init();
	MX_TIM7_Init();
}

/**
 * @brief  Deinit of arbitrary DAC generator.
 * @note 	Peripherals reset TIM6 & TIM7 & DAC.
 * @param  None
 * @retval None
 */
void TIMGenDacDeinit(void){
	//	HAL_TIM_Base_DeInit(&htim6);
	//	HAL_TIM_Base_DeInit(&htim7);

	/* Reset TIM peripherals */
	__HAL_RCC_TIM6_FORCE_RESET();
	__HAL_RCC_TIM6_RELEASE_RESET();

	__HAL_RCC_TIM7_FORCE_RESET();
	__HAL_RCC_TIM7_RELEASE_RESET();
}

#ifdef USE_GEN_PWM

/**
 * @brief  Reconfigures the number of PWM Duty Cycle changes in one waveform period.
 * @note		The number of DC cahnges is represented by CNDTR register of DMA peripheral.
 * @param  chan: channel number 0 or 1 (TIM6 or TIM7)
 * @retval None
 */
void DMA_GEN_PWM_Reconfig(uint8_t chan){
	if(chan==0){
		HAL_DMA_Abort(&hdma_tim6_up);
		HAL_DMA_Start(&hdma_tim6_up, (uint32_t)generator.pChanMem[0], (uint32_t)&(htim1.Instance->CCR2)/*(TIM1->CCR2)*/, generator.oneChanSamples[0]);
	}else if(chan==1){
		HAL_DMA_Abort(&hdma_tim7_up);
		HAL_DMA_Start(&hdma_tim7_up, (uint32_t)generator.pChanMem[1], (uint32_t)&(htim3.Instance->CCR1)/*(TIM3->CCR1)*/, generator.oneChanSamples[1]);
	}
}

/**
 * @brief  Enables/Starts generating of PWM and its simultaneous Duty Cycle changes.
 * @note		The PWM generated by TIM6 (ch2) & TIM7 (ch1). The DMA for DC change triggered by TIM1 (for TIM6) & TIM3 (for TIM7).
 * @param  None
 * @retval None
 */
void PWMGeneratingEnable(void){
	if(generator.numOfChannles==1){
		/* After sole Generator initialization, PWM generator do not enter TIMGenPwmInit()
		function and thus UDE bits are not configured. Must be set here. */
		__HAL_TIM_ENABLE_DMA(&htim6, TIM_DMA_UPDATE);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIM_Base_Start(&htim6);
	}else if(generator.numOfChannles>1){
		__HAL_TIM_ENABLE_DMA(&htim6, TIM_DMA_UPDATE);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIM_Base_Start(&htim6);
		__HAL_TIM_ENABLE_DMA(&htim7, TIM_DMA_UPDATE);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		HAL_TIM_Base_Start(&htim7);
	}
}

/**
 * @brief  Disables/Stops generating of PWM and its simultaneous Duty Cycle changes.
 * @note		The PWM generated by TIM6 (ch2) & TIM7 (ch1). The DMA for DC change triggered by TIM1 (for TIM6) & TIM3 (for TIM7).
 * @param  None
 * @retval None
 */
void PWMGeneratingDisable(void){
	if(generator.numOfChannles==1){
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
		HAL_TIM_Base_Stop(&htim6);
	}else if(generator.numOfChannles>1){
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
		HAL_TIM_Base_Stop(&htim6);
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
		HAL_TIM_Base_Stop(&htim7);
	}
}

/**
 * @brief  Initialization of arbitrary PWM generator.
 * @note		TIM6 & TIM7 (PWM gen.) and TIM1 & TIM3 (DMA for Duty Cycle change).
 * @param  None
 * @retval None
 */
void TIMGenPwmInit(void){
	MX_TIM1_GEN_PWM_Init();
	MX_TIM6_GEN_PWM_Init();
	MX_TIM3_GEN_PWM_Init();			// PWM generation
	MX_TIM7_GEN_PWM_Init();			// DMA transaction timing
}

/**
 * @brief  Deinit of arbitrary PWM generator.
 * @note		Resetting peripherals TIM6 & TIM7 (PWM gen.) and TIM1 & TIM3 (DMA for Duty Cycle change).
 * @param  None
 * @retval None
 */
void TIMGenPwmDeinit(void){
	__HAL_RCC_TIM6_FORCE_RESET();
	__HAL_RCC_TIM6_RELEASE_RESET();
	__HAL_RCC_TIM7_FORCE_RESET();
	__HAL_RCC_TIM7_RELEASE_RESET();
	__HAL_RCC_TIM1_FORCE_RESET();
	__HAL_RCC_TIM1_RELEASE_RESET();
	__HAL_RCC_TIM3_FORCE_RESET();
	__HAL_RCC_TIM3_RELEASE_RESET();
}

#endif //USE_GEN_PWM


#ifdef USE_GEN_PATTERN

void TIMGenPatternInit(void){
	MX_TIM6_Init();
	DMAGenPatternInit(&htim6);
}

void DMAGenPatternInit(TIM_HandleTypeDef* htim_base){
	__HAL_RCC_DMA2_CLK_ENABLE();

	hdma_tim6_up.Instance = DMA2_Channel3;
	hdma_tim6_up.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tim6_up.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim6_up.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim6_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim6_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim6_up.Init.Mode = DMA_CIRCULAR;
	hdma_tim6_up.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&hdma_tim6_up);

    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim6_up);
    __HAL_TIM_ENABLE_DMA(htim_base, TIM_DMA_UPDATE);
}

void DMA_GEN_PATTERN_Reconfig(void){
	HAL_DMA_Abort(&hdma_tim6_up);
	HAL_DMA_Start(&hdma_tim6_up, (uint32_t)generator.pChanMem[0], (uint32_t)&(GEN_PATTERN_GPIO_Port->ODR), generator.oneChanSamples[0]);
}

void TIMGenPatternDeinit(void){
	__HAL_RCC_TIM6_FORCE_RESET();
	__HAL_RCC_TIM6_RELEASE_RESET();
	__HAL_RCC_TIM7_FORCE_RESET();
	__HAL_RCC_TIM7_RELEASE_RESET();
    //HAL_DMA_DeInit(htim6->hdma[TIM_DMA_ID_UPDATE]);
}

#endif // USE_GEN_PATTERN

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif // USE_GEN_SIGNAL || USE_GEN_PWM || USE_GEN_PATTERN
