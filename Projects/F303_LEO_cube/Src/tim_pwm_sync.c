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

TIM_HandleTypeDef htim8;
DMA_HandleTypeDef hdma_tim8_ch1;
DMA_HandleTypeDef hdma_tim8_ch2;
DMA_HandleTypeDef hdma_tim8_ch3_up;
DMA_HandleTypeDef hdma_tim8_ch4_trig_com;

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
static void MX_TIM8_SYNC_PWM_Init(void) {
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
}

void TIM8_SYNC_PWM_MspInit(TIM_HandleTypeDef* htim_base) {
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Peripheral clock enable */
	__HAL_RCC_TIM8_CLK_ENABLE()
	;

	/**TIM8 GPIO Configuration
	 PC6     ------> TIM8_CH1
	 PC7     ------> TIM8_CH2
	 PC8     ------> TIM8_CH3
	 PC9     ------> TIM8_CH4
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
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

	__HAL_LINKDMA(htim_base, hdma[TIM_DMA_ID_CC1], hdma_tim8_ch1);

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

	__HAL_LINKDMA(htim_base, hdma[TIM_DMA_ID_CC2], hdma_tim8_ch2);

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
	TIM8->DIER |= TIM_DIER_CC3DE; //__HAL_TIM_ENABLE_DMA(htim_base, TIM_DMA_CC3);

	__HAL_LINKDMA(htim_base, hdma[TIM_DMA_ID_CC3], hdma_tim8_ch3_up);
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

	__HAL_LINKDMA(htim_base, hdma[TIM_DMA_ID_CC4], hdma_tim8_ch4_trig_com);
}

void TIM8_SYNC_PWM_MspDeinit(TIM_HandleTypeDef* htim_base) {
	/* Peripheral clock disable */
	__HAL_RCC_TIM8_CLK_DISABLE();

	/* TIM8 DMA DeInit */
	HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC1]);
	HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC2]);
	HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC3]);
	HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC4]);
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
void TIM_SYNC_PWM_Deinit(void) {
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
void TIM_SYNC_PWM_ChannelState(uint8_t channel, uint8_t state) {
	if (channel == 1) {
		syncPwm.chan1 = (state == 1) ? CHAN_ENABLE : CHAN_DISABLE;
	} else if (channel == 2) {
		syncPwm.chan2 = (state == 1) ? CHAN_ENABLE : CHAN_DISABLE;
	} else if (channel == 3) {
		syncPwm.chan3 = (state == 1) ? CHAN_ENABLE : CHAN_DISABLE;
	} else if (channel == 4) {
		syncPwm.chan4 = (state == 1) ? CHAN_ENABLE : CHAN_DISABLE;
	}
}

/* Start generating Output Compare signals. */
/**
 * @brief  Starts generating synchronized PWM on the selected channels.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_Start(void) {
	if (syncPwm.chan1 == CHAN_ENABLE) {
		TIM8->CCR1 = syncPwm.dataEdgeChan1[1];
		TIM8->DIER |= TIM_DIER_CC1DE;
		HAL_DMA_Start(&hdma_tim8_ch1, (uint32_t) &syncPwm.dataEdgeChan1[0],
				(uint32_t) &(TIM8->CCR1), 2);
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
	}

	if (syncPwm.chan2 == CHAN_ENABLE) {
		TIM8->CCR2 = syncPwm.dataEdgeChan2[1];
		TIM8->DIER |= TIM_DIER_CC2DE;
		HAL_DMA_Start(&hdma_tim8_ch2, (uint32_t) &syncPwm.dataEdgeChan2[0],
				(uint32_t) &(TIM8->CCR2), 2);
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
	}

	if (syncPwm.chan3 == CHAN_ENABLE) {
		TIM8->CCR3 = syncPwm.dataEdgeChan3[1];
		TIM8->DIER |= TIM_DIER_CC3DE;
		HAL_DMA_Start(&hdma_tim8_ch3_up, (uint32_t) &syncPwm.dataEdgeChan3[0],
				(uint32_t) &(TIM8->CCR3), 2);
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_3, TIM_CCx_ENABLE);
	}

	if (syncPwm.chan4 == CHAN_ENABLE) {
		TIM8->CCR4 = syncPwm.dataEdgeChan4[1];
		TIM8->DIER |= TIM_DIER_CC4DE;
		HAL_DMA_Start(&hdma_tim8_ch4_trig_com,
				(uint32_t) &syncPwm.dataEdgeChan4[0], (uint32_t) &(TIM8->CCR4),
				2);
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
void TIM_SYNC_PWM_Stop(void) {
	/* Disable the peripheral */
	__HAL_TIM_DISABLE(&htim8);
	/* Master Output Enable Disable */
	__HAL_TIM_MOE_DISABLE(&htim8);

	if (syncPwm.chan1 == CHAN_ENABLE) {
		TIM8->DIER &= ~TIM_DIER_CC1DE;
		HAL_DMA_Abort(&hdma_tim8_ch1);
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
	}

	if (syncPwm.chan2 == CHAN_ENABLE) {
		TIM8->DIER &= ~TIM_DIER_CC2DE;
		HAL_DMA_Abort(&hdma_tim8_ch2);
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);
	}

	if (syncPwm.chan3 == CHAN_ENABLE) {
		TIM8->DIER &= ~TIM_DIER_CC3DE;
		HAL_DMA_Abort(&hdma_tim8_ch3_up);
		TIM_CCxChannelCmd(htim8.Instance, TIM_CHANNEL_3, TIM_CCx_DISABLE);
	}

	if (syncPwm.chan4 == CHAN_ENABLE) {
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
	if (syncPwm.stepMode == CHAN_ENABLE) {
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
void TIM_SYNC_PWM_DMA_ChanConfig(uint16_t ccr1st, uint16_t ccr2nd) {
	switch (syncPwm.channelToConfig) {
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
void TIM_SYNC_PWM_StepMode_Enable(void) {
	TIM8->CR1 |= TIM_CR1_OPM;
	syncPwm.stepMode = CHAN_ENABLE;
}

/**
 * @brief  Disable Step mode for Synch. PWMs.
 * @note		Disable one PWM period generation. Enable continuous mode.
 * @param  None
 * @retval None
 */
void TIM_SYNC_PWM_StepMode_Disable(void) {
	TIM8->CR1 &= ~TIM_CR1_OPM;
	syncPwm.stepMode = CHAN_DISABLE;
}

/**
 * @brief  Reconfiguration of PWM frequency.
 * @note		ARR & PSC calculated by host.
 * @params arrPsc: ARR and PSC register of TIM8
 * @retval None
 */
void TIM_ARR_PSC_Reconfig(uint32_t arrPsc) {
	htim8.Init.Prescaler = (uint16_t) (arrPsc >> 16);
	htim8.Init.Period = (uint16_t) arrPsc;
	HAL_TIM_Base_Init(&htim8);
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
