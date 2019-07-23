/**
 *****************************************************************************
 * @file    tim_scope.c
 * @author  HeyBirdie
 * @date    July 13, 2019
 * @brief   This file contains timer functions for logic analyzer
 *****************************************************************************
 */

#include "tim.h"
#include "mcu_config.h"

#ifdef USE_SCOPE

/** @addtogroup Scope
 * @{
 */

/** @defgroup Scope_Timers Scope Timers
 * @{
 */

/** @defgroup Scope_Timer_Variables Scope Timer Variables
 * @{
 */

TIM_HandleTypeDef htim_scope;

/**
 * @}
 */

/** @defgroup Scope_Timer_Init_Functions Scope Timer Initialization Functions
 * @{
 */

/* TIM15 init function */
void MX_TIM15_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim_scope.Instance = TIM15;
	htim_scope.Init.Prescaler = 0;
	htim_scope.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_scope.Init.Period = 0;
	htim_scope.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim_scope);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim_scope, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim_scope, &sMasterConfig);

}

void TIM15_SCOPE_MspInit(TIM_HandleTypeDef* htim_base)
{
	__TIM15_CLK_ENABLE();
}

void TIM15_SCOPE_MspDeinit(TIM_HandleTypeDef* htim_base)
{
	__TIM15_CLK_DISABLE();
}

/**
 * @}
 */

/* ************************************************************************************** */
/* --------------------------------- SCOPE FUNCTIONS ------------------------------------ */
/* ************************************************************************************** */

/** @defgroup Scope_TIM_Functions Scope TIMers Functions.
 * @{
 */
void TIMScopeEnable(){
	HAL_TIM_Base_Start(&htim_scope);
}

void TIMScopeDisable(){
	HAL_TIM_Base_Stop(&htim_scope);
}

uint8_t TIM_Reconfig_scope(uint32_t samplingFreq,uint32_t* realFreq){
	uint32_t periphClock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM15);
			//HAL_RCC_GetHCLKFreq();
	return TIM_Reconfig(&htim_scope,periphClock,samplingFreq,realFreq, true);
}

uint32_t getMaxScopeSamplingFreq(uint8_t ADCRes){
	if(ADCRes==12){
		return MAX_SAMPLING_FREQ_12B;
	}else if(ADCRes==8){
		return MAX_SAMPLING_FREQ_8B;
	}
	return HAL_RCC_GetPCLK2Freq()/(ADCRes+2);
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

#endif //USE_SCOPE
