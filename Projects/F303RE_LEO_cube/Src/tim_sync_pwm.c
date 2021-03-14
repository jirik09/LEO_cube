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

}

void MX_TIM3_SYNC_PWM_Init(void) {

}

void MX_TIM8_SYNC_PWM_Init(void) {

}

void TIM1_SYNC_PWM_MspInit(TIM_HandleTypeDef* htim_base) {

}

void TIM3_SYNC_PWM_MspInit(TIM_HandleTypeDef* htim_base) {

}

void TIM8_SYNC_PWM_MspInit(TIM_HandleTypeDef* htim_base) {

}


void TIM1_SYNC_PWM_MspDeinit(TIM_HandleTypeDef* htim_base) {
	/* Peripheral clock disable */
	__HAL_RCC_TIM1_CLK_DISABLE();
}

void TIM3_SYNC_PWM_MspDeinit(TIM_HandleTypeDef* htim_base) {
	/* Peripheral clock disable */
	__HAL_RCC_TIM3_CLK_DISABLE();
}

void TIM8_SYNC_PWM_MspDeinit(TIM_HandleTypeDef* htim_base) {
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
		syncPwm.chan1 = (state == 1) ? CH_ENABLE : CH_DISABLE;
	} else if (channel == 2) {
		syncPwm.chan2 = (state == 1) ? CH_ENABLE : CH_DISABLE;
	} else if (channel == 3) {
		syncPwm.chan3 = (state == 1) ? CH_ENABLE : CH_DISABLE;
	} else if (channel == 4) {
		syncPwm.chan4 = (state == 1) ? CH_ENABLE : CH_DISABLE;
	}
}

void TIM_SYNC_PWM_Start(void) {

}

void TIM_SYNC_PWM_Stop(void) {

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
	uint32_t periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM34);  // HAL_RCC_GetHCLKFreq();
	return TIM_ReconfigPrecise(&htim3, periphClock, freq);
}

double TIM_Reconfig_SyncPwm_Ch34(double freq) {
	uint32_t periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM8);
	return TIM_ReconfigPrecise(&htim8, periphClock, freq);
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
