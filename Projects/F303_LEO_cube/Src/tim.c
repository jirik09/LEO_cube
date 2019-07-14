/**
 ******************************************************************************
 * @file           		: tim.c
 * @date               : 18/01/2015 10:00:31
 * @brief			        : This file provides code for the configuration
 *                      of the TIM instances.
 ******************************************************************************
 *
 * COPYRIGHT(c) 2015 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "dac.h"
#include "tim.h"
#include "counter.h"
#include "sync_pwm.h"
#include "logic_analyzer.h"
#include "generator.h"
#include "mcu_config.h"
#include "stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32f3xx_ll_tim.h"

/** @defgroup Timers Timers
 * @{
 */

/** @defgroup Arbitrary_DAC_PWM_Variables Arbitrary DAC & PWM Generator Variables
 * @{
 */
#if defined(USE_GEN) || defined(USE_GEN_PWM)
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
#ifdef USE_GEN_PWM
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim6_up;
DMA_HandleTypeDef hdma_tim7_up;
#endif //USE_GEN_PWM
#endif // USE_GEN || USE_GEN_PWM
/**
 * @}
 */

/** @defgroup Synch_PWM_Variables Synchronized PWM Variables
 * @{
 */
#ifdef USE_SYNC_PWM
TIM_HandleTypeDef htim8;
DMA_HandleTypeDef hdma_tim8_ch1;
DMA_HandleTypeDef hdma_tim8_ch2;
DMA_HandleTypeDef hdma_tim8_ch3_up;
DMA_HandleTypeDef hdma_tim8_ch4_trig_com;
#endif // USE_SYNC_PWM
/**
 * @}
 */


/** @defgroup Timers_Init_Functions Timers Initialization Functions Section.
 * @{
 */

/** @defgroup Arbitrary_DAC_TIM_Inits Arbitrary DAC Generator TIMers Initialization Functions
 * @{
 */
/**             
 * @brief  TIM6 Configuration.
 * @note   TIM6 configuration is based on APB1 frequency
 * @note   TIM6 Update event occurs each TIM6CLK/256
 * @param  None
 * @retval None
 */
#ifdef USE_GEN
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
#endif //USE_GEN


/**             
 * @brief  TIM6 Configuration.
 * @note   TIM6 configuration is based on APB1 frequency
 * @note   TIM6 Update event occurs each TIM6CLK/256
 * @param  None
 * @retval None
 */
#ifdef USE_GEN
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
#endif //USE_GEN
/**
 * @}
 */

/* ************************************************************************************** */
/* ------------------------------- START OF PWM GENERATOR ------------------------------- */
/** @defgroup Arbitrary_PWM_TIM_Inits Arbitrary PWM Generator TIMers Initialization Functions
 * @{
 */
#ifdef USE_GEN_PWM
/**             
 * @brief  TIM1 Configuration.
 * @note   TIM1 generates PWM on a given channel.
 * @param  None
 * @retval None
 */
static void MX_TIM1_GEN_PWM_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

	/* ARR = 1024 (10 bit resolution in default) */
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
static void MX_TIM3_GEN_PWM_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

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
static void MX_TIM6_GEN_PWM_Init(void)
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
}

/**             
 * @brief  TIM7 Configuration for arbitrary DAC & PWM generators.
 * @note   For PWM gen: TIM7 handles the periodic change of PWM duty cycle using DMA -> Memory to Capture/Compare reg. transfers.
 *					The DMA is triggered by predefined frequency in order to tranfer data from RAM (arbitrary waveform) to TIM3 CC reg.
 * @note 	For DAC gen: TIM7 handles the periodic change of DAC value acording to predefined arbitrary waveform.
 * @param  None
 * @retval None
 */
static void MX_TIM7_GEN_PWM_Init(void)
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
}

#endif //USE_GEN_PWM
/**
 * @}
 */
/* -------------------------------- END OF PWM GENERATOR -------------------------------- */
/* ************************************************************************************** */


/* ************************************************************************************** */
/* ---------------------------- START OF SYNC PWM GENERATOR ----------------------------- */
/** @defgroup Synch_PWM_TIM_Inits Synchronizes PWM Generator TIMers Initialization Functions
 * @{
 */
#ifdef USE_SYNC_PWM
/**             
 * @brief  TIM8 Configuration.
 * @note   Configures 4 channels for synchronized PWM outputs handled by one timer.
 * @param  None
 * @retval None
 */
static void MX_TIM8_SYNC_PWM_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

	/* TIM8 running on 72 MHz - Run 1 Hz by default. */
	htim8.Instance = TIM8;
	htim8.Init.Prescaler = 1151; // 1151
	htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim8.Init.Period = 62499; // 62499
	htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim8.Init.RepetitionCounter = 0;
	htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim8);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig);

	HAL_TIM_OC_Init(&htim8);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;			
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	//	sConfigOC.Pulse = 0;
	sConfigOC.Pulse = syncPwm.dataEdgeChan1[1];
	HAL_TIM_OC_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1);
	sConfigOC.Pulse = syncPwm.dataEdgeChan2[1];
	HAL_TIM_OC_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2);
	sConfigOC.Pulse = syncPwm.dataEdgeChan3[1];
	HAL_TIM_OC_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3);
	sConfigOC.Pulse = syncPwm.dataEdgeChan4[1];
	HAL_TIM_OC_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_4);

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
	HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig);

	/* CCRx DMA request enable */
	//	TIM8->DIER |= TIM_DIER_CC1DE;
	//	TIM8->DIER |= TIM_DIER_CC2DE;
	//	TIM8->DIER |= TIM_DIER_CC3DE;
	//	TIM8->DIER |= TIM_DIER_CC4DE;
}
#endif // USE_SYNC_PWM
/**
 * @}
 */
/* ----------------------------- END OF SYNC PWM GENERATOR ------------------------------ */
/* ************************************************************************************** */

/**
 * @}
 */

/** @defgroup Common_GPIOs_DMAs_TIM_Inits Common GPIOs & DMAs Initialization Function.
 * @{
 */
/**             
 * @brief  This function configures GPIOs and DMAs used by the functionalities.
 * @note   Called from Timers initialization functions.
 * @param  htim_base: pointer to timer's handler
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef GPIO_InitStruct;

#ifdef USE_SCOPE
	if(htim_base->Instance==TIM15)
	{
		TIM15_SCOPE_MspInit(htim_base);
	}
#endif //USE_SCOPE

	/* Note: PC app must send the mode first even if only one 
		 generator is implemented in device */
#if defined(USE_GEN) || defined(USE_GEN_PWM)
#ifdef USE_GEN
	/* DAC generator mode TIM decision */
	if(generator.modeState==GENERATOR_DAC){
		if(htim_base->Instance==TIM6){
			__TIM6_CLK_ENABLE();
		}
		if(htim_base->Instance==TIM7){
			__TIM7_CLK_ENABLE();
		}
	}
#endif //USE_GEN

#ifdef USE_GEN_PWM
	/* PWM generator mode TIM decision */
	if(generator.modeState==GENERATOR_PWM){
		if(htim_base->Instance==TIM1){
			__TIM1_CLK_ENABLE();

			/**TIM1 GPIO Configuration
				PA9     ------> TIM1_CH2 
			 */
			GPIO_InitStruct.Pin = GPIO_PIN_9;
			GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		}
		if(htim_base->Instance==TIM3){
			__TIM3_CLK_ENABLE();

			/**TIM3 GPIO Configuration
				PB4     ------> TIM3_CH1 
			 */
			GPIO_InitStruct.Pin = GPIO_PIN_4;
			GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		}
		if(htim_base->Instance==TIM6){
			__TIM6_CLK_ENABLE();

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
			TIM6->DIER |= TIM_DIER_UDE;

			__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim6_up);
		}
		if(htim_base->Instance==TIM7){
			__TIM7_CLK_ENABLE();

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
			TIM7->DIER |= TIM_DIER_UDE;

			__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim7_up);
		}
	}
#endif //USE_GEN_PWM
#endif //USE_GEN || USE_GEN_PWM

#ifdef USE_SYNC_PWM
	if(htim_base->Instance==TIM8)
	{
		/* Peripheral clock enable */
		__HAL_RCC_TIM8_CLK_ENABLE();

		/**TIM8 GPIO Configuration
    PC6     ------> TIM8_CH1
    PC7     ------> TIM8_CH2
    PC8     ------> TIM8_CH3
    PC9     ------> TIM8_CH4 
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_TIM8;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/* TIM8 DMA Init */
		/* TIM8_CH1 Init */
		hdma_tim8_ch1.Instance = DMA2_Channel3;
		hdma_tim8_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_tim8_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_tim8_ch1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_tim8_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_tim8_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_tim8_ch1.Init.Mode = DMA_CIRCULAR;
		hdma_tim8_ch1.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_tim8_ch1);
		TIM8->DIER |= TIM_DIER_CC1DE;

		__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC1],hdma_tim8_ch1);

		/* TIM8_CH2 Init */
		hdma_tim8_ch2.Instance = DMA2_Channel5;
		hdma_tim8_ch2.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_tim8_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_tim8_ch2.Init.MemInc = DMA_MINC_ENABLE;
		hdma_tim8_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_tim8_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_tim8_ch2.Init.Mode = DMA_CIRCULAR;
		hdma_tim8_ch2.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_tim8_ch2);
		TIM8->DIER |= TIM_DIER_CC2DE;

		__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC2],hdma_tim8_ch2);

		/* TIM8_CH3_UP Init */
		hdma_tim8_ch3_up.Instance = DMA2_Channel1;
		hdma_tim8_ch3_up.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_tim8_ch3_up.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_tim8_ch3_up.Init.MemInc = DMA_MINC_ENABLE;
		hdma_tim8_ch3_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_tim8_ch3_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_tim8_ch3_up.Init.Mode = DMA_CIRCULAR;
		hdma_tim8_ch3_up.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_tim8_ch3_up);
		TIM8->DIER |= TIM_DIER_CC3DE;  //__HAL_TIM_ENABLE_DMA(htim_base, TIM_DMA_CC3);

		__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC3],hdma_tim8_ch3_up);
		//__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim8_ch3_up);

		/* TIM8_CH4_TRIG_COM Init */
		hdma_tim8_ch4_trig_com.Instance = DMA2_Channel2;
		hdma_tim8_ch4_trig_com.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_tim8_ch4_trig_com.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_tim8_ch4_trig_com.Init.MemInc = DMA_MINC_ENABLE;
		hdma_tim8_ch4_trig_com.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_tim8_ch4_trig_com.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_tim8_ch4_trig_com.Init.Mode = DMA_CIRCULAR;
		hdma_tim8_ch4_trig_com.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_tim8_ch4_trig_com);
		TIM8->DIER |= TIM_DIER_CC4DE;

		__HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC4],hdma_tim8_ch4_trig_com);
	}
#endif //USE_SYNC_PWM

#ifdef USE_LOG_ANLYS
	if(htim_base->Instance==TIM1){
		if(logAnlys.enable==LOGA_ENABLED){
			TIM1_LOG_ANLYS_MspInit(htim_base);
		}
	}

	if(htim_base->Instance==TIM4){
		if(logAnlys.enable==LOGA_ENABLED){
			TIM4_LOG_ANLYS_MspInit(htim_base);
		}
	}
#endif //USE_LOG_ANLYS



#ifdef USE_COUNTER

	if(htim_base->Instance==TIM2){
		if(counter.state==COUNTER_ETR||counter.state==COUNTER_REF){
			TIM2_ETRorREF_MspInit(htim_base);
		}else if(counter.state==COUNTER_IC||counter.state==COUNTER_TI){
			TIM2_ICorTI_MspInit(htim_base);
		}
	}

	if(htim_base->Instance==TIM4){
		if(logAnlys.enable==LOGA_DISABLED){
			TIM4_REForICorTI_MspInit(htim_base);
		}
	}

#endif //USE_COUNTER
}
/**
 * @}
 */

/** @defgroup Common_GPIOs_DMAs_TIM_Deinits Common GPIOs & DMAs Deinitialization Function.
 * @{
 */
/**             
 * @brief  This function deinitializes GPIOs and DMAs used by the functionalities.
 * @param  htim_base: pointer to timer's handler
 * @retval None
 */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
#ifdef USE_SCOPE
	if(htim_base->Instance==TIM15)
	{
		TIM15_SCOPE_MspDeinit(htim_base);
	}
#endif //USE_SCOPE

#if defined(USE_GEN) || defined(USE_GEN_PWM)
#ifdef USE_GEN
	if(generator.modeState==GENERATOR_DAC){
		if(htim_base->Instance==TIM6){
			__TIM6_CLK_DISABLE();
		}
		if(htim_base->Instance==TIM7){
			__TIM7_CLK_DISABLE();
		}
	}
#endif //USE_GEN

#ifdef USE_GEN_PWM
	if(generator.modeState==GENERATOR_PWM){
		if(htim_base->Instance==TIM1){
			__TIM1_CLK_DISABLE();
		}
		if(htim_base->Instance==TIM3){
			__TIM3_CLK_DISABLE();
		}
		if(htim_base->Instance==TIM6){
			__TIM6_CLK_DISABLE();
			/* Peripheral DMA DeInit*/
			HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_UPDATE]);
		}
		if(htim_base->Instance==TIM7){
			__TIM7_CLK_DISABLE();
			/* Peripheral DMA DeInit*/
			HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_UPDATE]);
		}
	}
#endif //USE_GEN_PWM

#endif //USE_GEN || USE_GEN_PWM

#ifdef USE_SYNC_PWM
	if(htim_base->Instance==TIM8)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM8_CLK_DISABLE();

		/* TIM8 DMA DeInit */
		HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC1]);
		HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC2]);
		HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC3]);
		HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC4]);
	}
#endif //USE_SYNC_PWM

#ifdef USE_LOG_ANLYS
	if(htim_base->Instance==TIM1){
		TIM1_LOG_ANLYS_MspDeinit(htim_base);
	}

	if(htim_base->Instance==TIM4){
		if(logAnlys.enable==ENABLE){
			TIM4_LOG_ANLYS_MspDeinit(htim_base);
		}
	}

#endif //USE_LOG_ANLYS

#ifdef USE_COUNTER
	if(htim_base->Instance==TIM2){
		TIM2_CNT_MspDeinit(htim_base);
	}

	if(htim_base->Instance==TIM4){
		TIM4_CNT_MspDeinit(htim_base);
	}
#endif //USE_COUNTER
} 
/**
 * @}
 */

/* ************************************************************************************** */
/* ----------------------------- GEN and PWM GEN FUNCTIONS ------------------------------ */
/* ************************************************************************************** */
/** @defgroup Arbitrary_DAC_PWM_TIM_Functions Arbitrary DAC & PWM Generators Functions.
 * @{
 */
#if defined(USE_GEN) || defined(USE_GEN_PWM)
/**             
 * @brief  This function calls Common timer reconfiguration function TIM_Reconfig().
 * @note		Reconfigures timer as close as possible to required value (recalculates timer registers).
 * @param  samplingFreq: required frequency of the timer
 * @param  chan: channel number 0 - 1
 * @param  *realFreq: pointer to calculated real frequency
 * @retval status
 */
uint8_t TIM_Reconfig_gen(uint32_t samplingFreq,uint8_t chan,uint32_t* realFreq){
	uint32_t periphClock = HAL_RCC_GetPCLK1Freq()*2;
	if(chan==0){
		return TIM_Reconfig(&htim6,periphClock,samplingFreq,realFreq,true);
	}else if(chan==1){
		return TIM_Reconfig(&htim7,periphClock,samplingFreq,realFreq,true);
	}else{
		return 0;
	}
}

/**             
 * @brief  Enable TIM6 & TIM7 that trigger DMA - generating DAC.
 * @param  None
 * @retval None
 */
void TIMGenEnable(void){
	HAL_TIM_Base_Start(&htim6);
	HAL_TIM_Base_Start(&htim7);
}

/**             
 * @brief  Disable TIM6 & TIM7 - stop triggering DMA / generating DAC.
 * @param  None
 * @retval None
 */
void TIMGenDisable(void){
	HAL_TIM_Base_Stop(&htim6);
	HAL_TIM_Base_Stop(&htim7);
}

/**             
 * @brief  Initialization of arbitrary DAC generator.
 * @note 	TIM6 & TIM7 & DAC.
 * @param  None
 * @retval None
 */
void TIMGenInit(void){
	MX_DAC_Init();
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
	RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;	

	RCC->APB1RSTR |= RCC_APB1RSTR_TIM7RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM7RST;	

	/* Reset DAC peripheral */
	RCC->APB1RSTR |= RCC_APB1RSTR_DAC1RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_DAC1RST;		
}
#endif //USE_GEN || USE_GEN_PWM
/**
 * @}
 */

/* ************************************************************************************** */
/* ------------------- PWM GENERATOR (CHANGING DUTY CYCLE) FUNCTIONS -------------------- */
/* ************************************************************************************** */
/** @defgroup Arbitrary_PWM_TIM_Functions Arbitrary PWM Generator Functions.
 * @{
 */
#ifdef USE_GEN_PWM
/**             
 * @brief  Reconfigures the number of PWM Duty Cycle changes in one waveform period.
 * @note		The number of DC cahnges is represented by CNDTR register of DMA peripheral.
 * @param  chan: channel number 0 or 1 (TIM6 or TIM7)
 * @retval None
 */
void TIM_DMA_Reconfig(uint8_t chan){	
	if(chan==0){
		HAL_DMA_Abort(&hdma_tim6_up);	
		HAL_DMA_Start(&hdma_tim6_up, (uint32_t)generator.pChanMem[0], (uint32_t)&(TIM1->CCR2), generator.oneChanSamples[0]);
	}else if(chan==1){
		HAL_DMA_Abort(&hdma_tim7_up);
		HAL_DMA_Start(&hdma_tim7_up, (uint32_t)generator.pChanMem[1], (uint32_t)&(TIM3->CCR1), generator.oneChanSamples[1]);
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
		TIM6->DIER |= TIM_DIER_UDE;			
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIM_Base_Start(&htim6);				
	}else if(generator.numOfChannles>1){	
		TIM6->DIER |= TIM_DIER_UDE;			
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIM_Base_Start(&htim6);	
		TIM7->DIER |= TIM_DIER_UDE;			
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
	//	HAL_TIM_Base_DeInit(&htim1);
	//	HAL_TIM_Base_DeInit(&htim6);
	//	HAL_TIM_Base_DeInit(&htim3);
	//	HAL_TIM_Base_DeInit(&htim7);

	/* Reset TIM peripherals */
	RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;	

	RCC->APB1RSTR |= RCC_APB1RSTR_TIM7RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM7RST;	

	RCC->APB2RSTR |= RCC_APB2RSTR_TIM1RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM1RST;	

	RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;	
}

//void TIMGenPWMEnable(void){
//  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
//	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
//}

//void TIMGenPWMDisable(void){
//  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
//	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
//}

/**             
 * @brief  Configuration of Timer Prescaler (PSC).
 * @note		Calculated for TIM1 & TIM3 by host application.
 * @param  pscVal: Prescaler value
 * @param  chan: channel number 1 or 2
 * @retval None
 */
void TIM_GEN_PWM_PSC_Config(uint16_t pscVal, uint8_t chan){
	if(chan == 1){
		TIM1->PSC = pscVal;
	}else{
		TIM3->PSC = pscVal;
	}
}

/**             
 * @brief  Configuration of Timer Auto-Reload register (ARR).
 * @note		Calculated for TIM1 & TIM3 by host application.
 * @param  arrVal: Auto-Reload register value
 * @param  chan: channel number 1 or 2
 * @retval None
 */
void TIM_GEN_PWM_ARR_Config(uint16_t arrVal, uint8_t chan){
	if(chan == 1){
		TIM1->ARR = arrVal;
	}else{
		TIM3->ARR = arrVal;
	}	
}

#endif //USE_GEN_PWM
/**
 * @}
 */


/* ************************************************************************************* */
/* ----------------------- SYNCHRONIZED PWM GENERATOR FUNCTIONS ------------------------ */
/* ************************************************************************************* */
/** @defgroup Synch_PWM_Functions Synchronized PWM Generator Functions.
 * @{
 */
#ifdef USE_SYNC_PWM
/**             
 * @brief  Initialization of Synchronized PWMs.
 * @note		TIM8.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_Init(void){		
	//	htim8.State = HAL_TIM_STATE_RESET;
	MX_TIM8_SYNC_PWM_Init();
	/* Very thanks to optimization 3, TIM Base Init function 
		is not called from SYNC PWM Initi function. */
	HAL_TIM_Base_Init(&htim8);
}

/**             
 * @brief  Deinit of Synchronized PWMs.
 * @note		TIM8 peripherla reset.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_Deinit(void){
	HAL_TIM_Base_DeInit(&htim8);	

	/* Reset TIM8 preipheral */
	RCC->APB2RSTR |= RCC_APB2RSTR_TIM8RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM8RST;	
}

/* Set the channel to be enabled or disabled. */
/**             
 * @brief  Sets channel state.
 * @note		Channel can be disabled.
 * @param  channel: channel number 1 - 4
 * @param  state: CHAN_ENABLE or CHAN_DISABLE
 * @retval None
 */
void TIM_SYNC_PWM_ChannelState(uint8_t channel, uint8_t state)
{
	if(channel == 1){
		syncPwm.chan1 = (state == 1) ? CHAN_ENABLE : CHAN_DISABLE;		
	}else if(channel == 2){
		syncPwm.chan2 = (state == 1) ? CHAN_ENABLE : CHAN_DISABLE;	
	}else if(channel == 3){
		syncPwm.chan3 = (state == 1) ? CHAN_ENABLE : CHAN_DISABLE;	
	}else if(channel == 4){
		syncPwm.chan4 = (state == 1) ? CHAN_ENABLE : CHAN_DISABLE;	
	}
}

/* Start generating Output Compare signals. */
/**             
 * @brief  Starts generating synchronized PWM on the selected channels.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_Start(void)
{		
	if(syncPwm.chan1 == CHAN_ENABLE){		
		TIM8->CCR1 = syncPwm.dataEdgeChan1[1];		
		TIM8->DIER |= TIM_DIER_CC1DE;
		HAL_DMA_Start(&hdma_tim8_ch1, (uint32_t)&syncPwm.dataEdgeChan1[0], (uint32_t)&(TIM8->CCR1), 2);						
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);	
	}

	if(syncPwm.chan2 == CHAN_ENABLE){	
		TIM8->CCR2 = syncPwm.dataEdgeChan2[1];		
		TIM8->DIER |= TIM_DIER_CC2DE;				
		HAL_DMA_Start(&hdma_tim8_ch2, (uint32_t)&syncPwm.dataEdgeChan2[0], (uint32_t)&(TIM8->CCR2), 2);						
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);		
	}

	if(syncPwm.chan3 == CHAN_ENABLE){		
		TIM8->CCR3 = syncPwm.dataEdgeChan3[1];
		TIM8->DIER |= TIM_DIER_CC3DE;		
		HAL_DMA_Start(&hdma_tim8_ch3_up, (uint32_t)&syncPwm.dataEdgeChan3[0], (uint32_t)&(TIM8->CCR3), 2);	
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_3, TIM_CCx_ENABLE);				
	}

	if(syncPwm.chan4 == CHAN_ENABLE){			
		TIM8->CCR4 = syncPwm.dataEdgeChan4[1];
		TIM8->DIER |= TIM_DIER_CC4DE;
		HAL_DMA_Start(&hdma_tim8_ch4_trig_com, (uint32_t)&syncPwm.dataEdgeChan4[0], (uint32_t)&(TIM8->CCR4), 2);		
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_4, TIM_CCx_ENABLE);	
	}	
	/* Master Output Enable. */
	__HAL_TIM_MOE_ENABLE(&htim8);	
	/* Start generating. */
	TIM8->CR1 |= TIM_CR1_CEN;		
}

/* Stop generating Output Compare signals. */
/**             
 * @brief  Stops generating synchronized PWM.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_Stop(void)
{	
	/* Disable the peripheral */
	__HAL_TIM_DISABLE(&htim8); 
	/* Master Output Enable Disable */
	__HAL_TIM_MOE_DISABLE(&htim8);

	if(syncPwm.chan1 == CHAN_ENABLE){		
		TIM8->DIER &= ~TIM_DIER_CC1DE;		
		HAL_DMA_Abort(&hdma_tim8_ch1);	
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);		
	}

	if(syncPwm.chan2 == CHAN_ENABLE){		
		TIM8->DIER &= ~TIM_DIER_CC2DE;				
		HAL_DMA_Abort(&hdma_tim8_ch2);		
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);		
	}

	if(syncPwm.chan3 == CHAN_ENABLE){	
		TIM8->DIER &= ~TIM_DIER_CC3DE;				
		HAL_DMA_Abort(&hdma_tim8_ch3_up);		
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_3, TIM_CCx_DISABLE);		
	}

	if(syncPwm.chan4 == CHAN_ENABLE){	
		TIM8->DIER &= ~TIM_DIER_CC4DE;		
		HAL_DMA_Abort(&hdma_tim8_ch4_trig_com);		
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_4, TIM_CCx_DISABLE);				
	}

	/* Save configuration. */
	syncPwm.timAutoReloadReg = TIM8->ARR;
	syncPwm.timPrescReg = TIM8->PSC;

	/* There are DMA pending requests when stopped. Unfortunately 
	cannot be cleared in another way. */
	RCC->APB2RSTR |= RCC_APB2RSTR_TIM8RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM8RST;	

	MX_TIM8_SYNC_PWM_Init();

	/* Set One Pulse Mode after reset if previously enabled. Funny thing is 
	the peripheral does not work if CEN bit is not enabled right after OPM bit. 
	And even funnier is it does not generate a pulse. Bloody hell! */
	if(syncPwm.stepMode==CHAN_ENABLE){	
		TIM_SYNC_PWM_StepMode_Enable();
	}

	/* Load previous configuration. */
	htim8.Init.Prescaler = syncPwm.timPrescReg;
	htim8.Init.Period = syncPwm.timAutoReloadReg;
	HAL_TIM_Base_Init(&htim8);
}

/**             
 * @brief  Synch. PWM channels configuration.
 * @note		Since only one TIMer is used for generaring synchronized PWMs the value of CCR reg. must be changed on the fly (using DMA from Memory to CCR).
 * @note		Configure the required DMA for n-th channel. CCR register in time of rising edge and falling edge.
						The channel number is sent in previous message.
 * @param  ccr1st:	the value of CCR for the 1st edge
 * @param  ccr2st:	the value of CCR for the 2st edge
 * @retval None
 */
void TIM_SYNC_PWM_DMA_ChanConfig(uint16_t ccr1st, uint16_t ccr2nd)
{
	switch (syncPwm.channelToConfig)
	{
	case SYNC_PWM_CHANNEL1:
		syncPwm.dataEdgeChan1[0] = ccr2nd;
		syncPwm.dataEdgeChan1[1] = ccr1st;
		break;
	case SYNC_PWM_CHANNEL2:
		syncPwm.dataEdgeChan2[0] = ccr2nd;
		syncPwm.dataEdgeChan2[1] = ccr1st;
		break;
	case SYNC_PWM_CHANNEL3:
		syncPwm.dataEdgeChan3[0] = ccr2nd;
		syncPwm.dataEdgeChan3[1] = ccr1st;
		break;
	case SYNC_PWM_CHANNEL4:
		syncPwm.dataEdgeChan4[0] = ccr2nd;
		syncPwm.dataEdgeChan4[1] = ccr1st;
		break;
	default:
		break;
	}	
}

/**             
 * @brief  Enable Step mode for Synch. PWMs.
 * @note		Only one period of PWM is generated. Disable continuous mode.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_StepMode_Enable(void)
{	
	TIM8->CR1 |= TIM_CR1_OPM;			
	syncPwm.stepMode = CHAN_ENABLE;
}

/**             
 * @brief  Disable Step mode for Synch. PWMs.
 * @note		Disable one PWM period generation. Enable continuous mode.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_StepMode_Disable(void)
{
	TIM8->CR1 &= ~TIM_CR1_OPM;	
	syncPwm.stepMode = CHAN_DISABLE;
}

/**
 * @brief  Reconfiguration of PWM frequency.
 * @note		ARR & PSC calculated by host.
 * @params arrPsc: ARR and PSC register of TIM8
 * @retval None
 */
void TIM_ARR_PSC_Reconfig(uint32_t arrPsc)
{								
	htim8.Init.Prescaler = (uint16_t)(arrPsc >> 16);
	htim8.Init.Period = (uint16_t)arrPsc;
	HAL_TIM_Base_Init(&htim8);
}

#endif //USE_SYNC_PWM
/**
 * @}
 */

/** @defgroup Common_TIM_Functions Common TIM Functions.
 * @{
 */
/**             
 * @brief  Common Timer reconfiguration function.
 * @param  isFreqPassed: tell whether a required frequency is passed
 * 						or whether (ARR*PSC) is passed to samplingFreq parameter
 * @retval None
 */

uint8_t TIM_Reconfig(TIM_HandleTypeDef* htim_base, uint32_t periphClock,
		uint32_t samplingFreq, uint32_t* realFreq, _Bool isFreqPassed) {

	int32_t clkDiv;
	uint16_t prescaler;
	uint16_t autoReloadReg;
	uint32_t errMinRatio = 0;
	uint8_t result = UNKNOW_ERROR;

	if (isFreqPassed == true) {
		clkDiv = ((2 * periphClock / samplingFreq) + 1) / 2; //to minimize rounding error
	} else {
		clkDiv = samplingFreq;
	}

	if (clkDiv == 0) { //error
		result = GEN_FREQ_MISMATCH;
	} else if (clkDiv <= 0x0FFFF) { //Sampling frequency is high enough so no prescaler needed
		prescaler = 0;
		autoReloadReg = clkDiv - 1;
		result = 0;
	} else {	// finding prescaler and autoReload value
		uint32_t errVal = 0xFFFFFFFF;
		uint32_t errMin = 0xFFFFFFFF;
		uint16_t ratio = clkDiv >> 16;
		uint16_t div;

		while (errVal != 0) {
			ratio++;
			div = clkDiv / ratio;
			errVal = clkDiv - (div * ratio);

			if (errVal < errMin) {
				errMin = errVal;
				errMinRatio = ratio;
			}

			if (ratio == 0xFFFF) { //exact combination wasnt found. we use best found
				div = clkDiv / errMinRatio;
				ratio = errMinRatio;
				break;
			}
		}

		if (ratio > div) {
			prescaler = div - 1;
			autoReloadReg = ratio - 1;
		} else {
			prescaler = ratio - 1;
			autoReloadReg = div - 1;
		}

		if (errVal) {
			result = GEN_FREQ_IS_INACCURATE;
		} else {
			result = 0;
		}
	}

	if (realFreq != 0) {
		*realFreq = periphClock / ((prescaler + 1) * (autoReloadReg + 1));
		//		if(*realFreq>MAX_SAMPLING_FREQ && autoReloadReg<0xffff){
		//			autoReloadReg++;
		//			*realFreq=HAL_RCC_GetPCLK2Freq()/((prescaler+1)*(autoReloadReg+1));
		//		}
	}

	htim_base->Init.Period = autoReloadReg;
	htim_base->Init.Prescaler = prescaler;
	HAL_TIM_Base_Init(htim_base);

	return result;

}
/**
 * @}
 */

/**
 * @}
 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
