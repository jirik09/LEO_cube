/**
 *****************************************************************************
 * @file    tim_pwm_sync.c
 * @author  HeyBirdie
 * @date    July 15, 2019
 * @brief   This file contains timer functions for synchronized PWM generator
 *****************************************************************************
 */

#include "tim.h"
#include "mcu_config.h"
#include "sync_pwm.h"
#include "stm32f3xx_ll_tim.h"

#ifdef USE_SYNC_PWM

/** @addtogroup Synch_PWM Synchronized PWM
 * @{
 */

/** @defgroup Synch_PWM_Timers Synchronized PWM Timers
 * @{
 */

/** @defgroup Synch_PWM_Timer_Variables Synchronized PWM Timer Variables
 * @{
 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim8;

/**
 * @}
 */

/** @defgroup Synch_PWM_Timer_Init_Functions Synchronized PWM Timer Initialization Functions
 * @{
 */

/**
 * @brief  TIM8 Configuration.
 * @note   Configures 4 channels for synchronized PWM outputs handled by one timer.
 * @param  None
 * @retval None
 */
void MX_TIM1_SYNC_PWM_Init(void) {
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 65535;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim1);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
}

void MX_TIM3_SYNC_PWM_Init(void) {
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_SlaveConfigTypeDef sSlaveConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 1999;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 35999;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim3);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

	HAL_TIM_PWM_Init(&htim3);

	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_COMBINED_RESETTRIGGER;
	sSlaveConfig.InputTrigger = TIM_TS_ITR0;
	HAL_TIM_SlaveConfigSynchronization(&htim3, &sSlaveConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_COMBINED_PWM2;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 9000;
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);

	sConfigOC.OCMode = TIM_OCMODE_COMBINED_PWM2;
	sConfigOC.Pulse = 18000;
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_3, TIM_CCx_ENABLE);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 27000;
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);
}

void MX_TIM8_SYNC_PWM_Init(void) {
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_SlaveConfigTypeDef sSlaveConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	htim8.Instance = TIM8;
	htim8.Init.Prescaler = 1999;
	htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim8.Init.Period = 35999;
	htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim8.Init.RepetitionCounter = 0;
	htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim8);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim8);

	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_COMBINED_RESETTRIGGER;
	sSlaveConfig.InputTrigger = TIM_TS_ITR0;
	HAL_TIM_SlaveConfigSynchronization(&htim8, &sSlaveConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 18000;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1);

	sConfigOC.OCMode = TIM_OCMODE_COMBINED_PWM2;
	sConfigOC.Pulse = 9000;
	HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2);
	TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 36000;
	HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3);

	sConfigOC.OCMode = TIM_OCMODE_COMBINED_PWM2;
	sConfigOC.Pulse = 27000;
	HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_4);
	TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_4, TIM_CCx_ENABLE);

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
	__HAL_TIM_MOE_ENABLE(&htim8);
}

void TIM1_SYNC_PWM_MspInit(TIM_HandleTypeDef *htim_base) {
	__HAL_RCC_TIM1_CLK_ENABLE();
}

void TIM3_SYNC_PWM_MspInit(TIM_HandleTypeDef *htim_base) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	__HAL_RCC_GPIOC_CLK_ENABLE();
	/**TIM3 GPIO Configuration
	 PC6     ------> TIM3_CH1
	 PC8     ------> TIM3_CH3
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	__HAL_RCC_TIM3_CLK_ENABLE();
}

void TIM8_SYNC_PWM_MspInit(TIM_HandleTypeDef *htim_base) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	__HAL_RCC_GPIOC_CLK_ENABLE();
	/**TIM8 GPIO Configuration
	 PC7     ------> TIM8_CH2
	 PC9     ------> TIM8_CH4
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_TIM8;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	__HAL_RCC_TIM8_CLK_ENABLE();
}

void TIM1_SYNC_PWM_MspDeinit(TIM_HandleTypeDef *htim_base) {
	/* Peripheral clock disable */
	__HAL_RCC_TIM1_CLK_DISABLE();
}

void TIM3_SYNC_PWM_MspDeinit(TIM_HandleTypeDef *htim_base) {
	/* Peripheral clock disable */
	__HAL_RCC_TIM3_CLK_DISABLE();
}

void TIM8_SYNC_PWM_MspDeinit(TIM_HandleTypeDef *htim_base) {
	/* Peripheral clock disable */
	__HAL_RCC_TIM8_CLK_DISABLE();
}

/**
 * @}
 */

/* ************************************************************************************** */
/* ---------------------------- SYNCHRONIZED PWM FUNCTIONS ------------------------------ */
/* ************************************************************************************** */

/** @defgroup Synchronized_PWM_TIM_Functions Synchronized PWM TIMers Functions.
 * @{
 */

/**
 * @brief  Initialization of Synchronized PWMs.
 * @note		TIM8.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_Init(void) {
	MX_TIM1_SYNC_PWM_Init();
	MX_TIM3_SYNC_PWM_Init();
	MX_TIM8_SYNC_PWM_Init();
}

/**
 * @brief  Deinit of Synchronized PWMs.
 * @note		TIM8 peripherla reset.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_Deinit(void) {
	HAL_TIM_Base_DeInit(&htim1);
	HAL_TIM_Base_DeInit(&htim8);
	HAL_TIM_Base_DeInit(&htim3);

	/* Reset TIM8 preipheral */
	__HAL_RCC_TIM1_FORCE_RESET();
	__HAL_RCC_TIM1_RELEASE_RESET();
	__HAL_RCC_TIM3_FORCE_RESET();
	__HAL_RCC_TIM3_RELEASE_RESET();
	__HAL_RCC_TIM8_FORCE_RESET();
	__HAL_RCC_TIM8_RELEASE_RESET();
}

/**
 * @brief  Sets channel state.
 * @note		Channel can be disabled.
 * @param  channel: channel number 1 - 4
 * @param  state: CHAN_ENABLE or CHAN_DISABLE
 * @retval None
 */
void TIM_SYNC_PWM_ChannelState(uint8_t channel, uint8_t state) {
	syncPwm.chan[channel] = (syncPwmStateTypeDef) state;
	/* TODO - function for dis/enabling the channel */
}

void TIM_SYNC_PWM_Start(void) {
	LL_TIM_EnableCounter(htim1.Instance);
	LL_TIM_DisableCounter(htim1.Instance);
}

void TIM_SYNC_PWM_Stop(void) {
	HAL_TIM_Base_Stop(&htim3);
	HAL_TIM_Base_Stop(&htim8);
}

/**
 * @brief  Enable Step mode for Synch. PWMs.
 * @note		Only one period of PWM is generated. Disable continuous mode.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_StepMode_Enable(void) {
	LL_TIM_SetOnePulseMode(htim3.Instance, LL_TIM_ONEPULSEMODE_SINGLE);
	LL_TIM_SetOnePulseMode(htim8.Instance, LL_TIM_ONEPULSEMODE_SINGLE);
	syncPwm.stepMode = CH_ENABLE;
}

/**
 * @brief  Disable Step mode for Synch. PWMs.
 * @note		Disable one PWM period generation. Enable continuous mode.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_StepMode_Disable(void) {
	LL_TIM_SetOnePulseMode(htim3.Instance, LL_TIM_ONEPULSEMODE_REPETITIVE);
	LL_TIM_SetOnePulseMode(htim8.Instance, LL_TIM_ONEPULSEMODE_REPETITIVE);
	syncPwm.stepMode = CH_DISABLE;
}

/**
 * @brief  Reconfiguration of PWM frequency.
 * @note		ARR & PSC calculated by host.
 * @params arrPsc: ARR and PSC register of TIM8
 * @retval None
 */
double TIM_Reconfig_SyncPwm_Ch12(double freq) {
	uint32_t periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM34); // HAL_RCC_GetHCLKFreq();
	return TIM_ReconfigPrecise(&htim3, periphClock, freq);
}

double TIM_Reconfig_SyncPwm_Ch34(double freq) {
	uint32_t periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM8);
	return TIM_ReconfigPrecise(&htim8, periphClock, freq);
}

void TIM_SYNC_PWM_SetChanDutyPhase(uint32_t channel, double dutyCycle, uint32_t phase) {
	float edge1 = phase / (float) 360;
	float edge2 = edge1 + (float) dutyCycle / 100;
	uint32_t temp, period;

	switch (channel) {
	case 1:
		period = LL_TIM_GetAutoReload(htim3.Instance);
		temp = (uint32_t)(period * edge1);
		LL_TIM_OC_SetCompareCH1(htim3.Instance, temp);
		temp = (uint32_t)(period * edge2);
		LL_TIM_OC_SetCompareCH2(htim3.Instance, temp);
		break;
	case 2:
		period = LL_TIM_GetAutoReload(htim8.Instance);
		temp = (uint32_t)(period * edge1);
		LL_TIM_OC_SetCompareCH2(htim8.Instance, temp);
		temp = (uint32_t)(period * edge2);
		LL_TIM_OC_SetCompareCH1(htim8.Instance, temp);
		break;
	case 3:
		period = LL_TIM_GetAutoReload(htim3.Instance);
		temp = (uint32_t)(period * edge1);
		LL_TIM_OC_SetCompareCH3(htim3.Instance, temp);
		temp = (uint32_t)(period * edge2);
		LL_TIM_OC_SetCompareCH4(htim3.Instance, temp);
		break;
	case 4:
		period = LL_TIM_GetAutoReload(htim8.Instance);
		temp = (uint32_t)(period * edge1);
		LL_TIM_OC_SetCompareCH4(htim8.Instance, temp);
		temp = (uint32_t)(period * edge2);
		LL_TIM_OC_SetCompareCH3(htim8.Instance, temp);
		break;
	default:
		break;
	}
}

void TIM_SYNC_PWM_SetChanInvert(uint8_t channel, uint8_t setInvert) {
	syncPwm.chanInvert[channel] = (syncPwmStateTypeDef) setInvert;

	switch (channel) {
	case 1:
		if (setInvert == 0) {
			LL_TIM_OC_SetMode(htim3.Instance, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_COMBINED_PWM2);
			LL_TIM_OC_SetMode(htim3.Instance, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);
		} else {
			LL_TIM_OC_SetMode(htim3.Instance, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_COMBINED_PWM1);
			LL_TIM_OC_SetMode(htim3.Instance, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM2);
		}
		break;
	case 2:
		if (setInvert == 0) {
			LL_TIM_OC_SetMode(htim8.Instance, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_COMBINED_PWM2);
			LL_TIM_OC_SetMode(htim8.Instance, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
		} else {
			LL_TIM_OC_SetMode(htim8.Instance, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_COMBINED_PWM1);
			LL_TIM_OC_SetMode(htim8.Instance, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM2);
		}
		break;
	case 3:
		if (setInvert == 0) {
			LL_TIM_OC_SetMode(htim3.Instance, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_COMBINED_PWM2);
			LL_TIM_OC_SetMode(htim3.Instance, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
		} else {
			LL_TIM_OC_SetMode(htim3.Instance, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_COMBINED_PWM1);
			LL_TIM_OC_SetMode(htim3.Instance, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM2);
		}
		break;
	case 4:
		if (setInvert == 0) {
			LL_TIM_OC_SetMode(htim8.Instance, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_COMBINED_PWM2);
			LL_TIM_OC_SetMode(htim8.Instance, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
		} else {
			LL_TIM_OC_SetMode(htim8.Instance, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_COMBINED_PWM1);
			LL_TIM_OC_SetMode(htim8.Instance, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM2);
		}
		break;
	default:
		break;
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

#endif //USE_SYNC_PWM
