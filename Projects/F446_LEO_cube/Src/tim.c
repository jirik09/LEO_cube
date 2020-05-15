/**
 ******************************************************************************
 * @file    tim.c
 * @author 	HeyBirdie
 * @date    July 15, 2019
 * @brief	This file provides code for the configuration of the TIM instances.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"
#include "counter.h"
#include "logic_analyzer.h"
#include "generator.h"
#include "mcu_config.h"
#include "stm32f4xx_ll_tim.h"

/** @defgroup Timers Timers
 * @{
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
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base) {

	/***************************** SCOPE **********************************/
#ifdef USE_SCOPE
	if (htim_base->Instance == TIM8) {
		TIM8_SCOPE_MspInit(htim_base);
	}
#endif //USE_SCOPE

	/**************************** GEN DAC ***********************************/
	/* Note: PC app must send the mode first even if only one
	 generator is implemented in device */
#if defined(USE_GEN) || defined(USE_GEN_PWM)
#ifdef USE_GEN
	/* DAC generator mode TIM decision */
	if (htim_base->Instance == TIM6) {
		TIM6_GEN_DAC_MspInit(htim_base);
	}
	if (htim_base->Instance == TIM7) {
		TIM7_GEN_DAC_MspInit(htim_base);
	}
#endif //USE_GEN

	/***************************** GEN PWM ***********************************/
#ifdef USE_GEN_PWM
	/* PWM generator mode TIM decision */
	if (generator.modeState == GENERATOR_PWM) {
		if (htim_base->Instance == TIM13) {
			TIM1_GEN_PWM_MspInit(htim_base);
		}
		if (htim_base->Instance == TIM3) {
			TIM3_GEN_PWM_MspInit(htim_base);
		}
		if (htim_base->Instance == TIM6) {
			TIM6_GEN_PWM_MspInit(htim_base);
		}
		if (htim_base->Instance == TIM7) {
			TIM7_GEN_PWM_MspInit(htim_base);
		}
	}
#endif //USE_GEN_PWM
#endif //USE_GEN || USE_GEN_PWM

	/***************************** SYNC PWM ********************************/
#ifdef USE_SYNC_PWM
	if (htim_base->Instance == TIM8) {
		TIM8_SYNC_PWM_MspInit(htim_base);
	}
#endif //USE_SYNC_PWM¨

	/***************************** LOG ANLYS  **********************************/
#ifdef USE_LOG_ANLYS
	if (htim_base->Instance == TIM1) {
		if (logAnlys.enable == LOGA_ENABLED) {
			TIM1_LOG_ANLYS_MspInit(htim_base);
		}
	}
	if(htim_base->Instance == TIM4) {
		if (logAnlys.enable == LOGA_ENABLED) {
			TIM4_LOG_ANLYS_MspInit(htim_base);
		}
	}
#endif //USE_LOG_ANLYS

	/****************************** COUNTER ***********************************/
#ifdef USE_COUNTER
	if (htim_base->Instance == TIM2) {
		if (counter.state == COUNTER_ETR || counter.state == COUNTER_REF) {
			TIM2_ETRorREF_MspInit(htim_base);
		} else if (counter.state == COUNTER_IC || counter.state == COUNTER_TI) {
			TIM2_ICorTI_MspInit(htim_base);
		}
	}
	if (htim_base->Instance == TIM4) {
#ifdef USE_LOG_ANLYS
		if (logAnlys.enable == LOGA_DISABLED) {
#endif //USE_LOG_ANLYS
			TIM4_REForICorTI_MspInit(htim_base);
#ifdef USE_LOG_ANLYS
		}
#endif //USE_LOG_ANLYS
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
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base) {

/***************************** SCOPE **********************************/
#ifdef USE_SCOPE
	if (htim_base->Instance == TIM8) {
		TIM8_SCOPE_MspDeinit(htim_base);
	}
#endif //USE_SCOPE

/**************************** GEN DAC  *********************************/
#if defined(USE_GEN) || defined(USE_GEN_PWM)
#ifdef USE_GEN
	if (generator.modeState == GENERATOR_DAC) {
		if (htim_base->Instance == TIM6) {
			TIM6_GEN_DAC_MspDeinit(htim_base);
		}
		if (htim_base->Instance == TIM7) {
			TIM7_GEN_DAC_MspDeinit(htim_base);
		}
	}
#endif //USE_GEN

/**************************** GEN PWM  *********************************/
#ifdef USE_GEN_PWM
	if (generator.modeState == GENERATOR_PWM) {
		if (htim_base->Instance == TIM13) {
			TIM1_GEN_PWM_MspDeinit(htim_base);
		}
		if (htim_base->Instance == TIM3) {
			TIM3_GEN_PWM_MspDeinit(htim_base);
		}
		if (htim_base->Instance == TIM6) {
			TIM6_GEN_PWM_MspDeinit(htim_base);
		}
		if (htim_base->Instance == TIM7) {
			TIM7_GEN_PWM_MspDeinit(htim_base);
		}
	}
#endif //USE_GEN_PWM
#endif //USE_GEN || USE_GEN_PWM

/**************************** SYNC PWM  *********************************/
#ifdef USE_SYNC_PWM
	if (htim_base->Instance == TIM8) {
		TIM8_SYNC_PWM_MspDeinit(htim_base);
	}
#endif //USE_SYNC_PWM

/**************************** LOG ANLYS  *********************************/
#ifdef USE_LOG_ANLYS
	if (htim_base->Instance == TIM1) {
		TIM1_LOG_ANLYS_MspDeinit(htim_base);
	}
	if (htim_base->Instance == TIM4) {
		if (logAnlys.enable == LOGA_ENABLED) {
			TIM4_LOG_ANLYS_MspDeinit(htim_base);
		}
	}
#endif //USE_LOG_ANLYS

/***************************** COUNTER  *********************************/
#ifdef USE_COUNTER
	if (htim_base->Instance == TIM2) {
		TIM2_CNT_MspDeinit(htim_base);
	}
	if (htim_base->Instance == TIM4) {
		TIM4_CNT_MspDeinit(htim_base);
	}
#endif //USE_COUNTER
}

/**
 * @}
 */

/** @defgroup Common_Timer_Functions Common Timer Functions.
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

//	htim_base->Init.Period = autoReloadReg;
//	htim_base->Init.Prescaler = prescaler;
//	HAL_TIM_Base_Init(htim_base);

	htim_base->Instance->ARR = autoReloadReg;
	htim_base->Instance->PSC = prescaler;
	LL_TIM_GenerateEvent_UPDATE(htim_base->Instance);

	return result;
}

/**
 * @brief  Common Timer reconfiguration function working with double.
 * @retval result: real frequency
 */
double TIM_ReconfigPrecise(TIM_HandleTypeDef* htim_base, uint32_t periphClock, double reqFreq) {

//	uint32_t arr, psc = 0;
//	uint32_t clkDiv;
//	double realFreq;
//
//	clkDiv = roundNumber((double)periphClock / reqFreq);
//
//	if(clkDiv <= 0xFFFF){
//		arr = clkDiv;
//		psc = 1;
//	}else{
//		for( ; psc==0; clkDiv--){
//			for(uint32_t pscTmp = 0xFFFF; pscTmp > 1; pscTmp--){
//				if((clkDiv % pscTmp) == 0){
//					if((clkDiv / pscTmp) <= 0xFFFF){
//						psc = pscTmp;
//						break;
//					}
//				}
//			}
//			if(psc != 0){
//				if(clkDiv / psc <= 0xFFFF){
//					break;
//				}
//			}
//		}
//		arr = clkDiv / psc;
//		if(arr < psc){
//			uint32_t swapVar = arr;
//			arr = psc;
//			psc = swapVar;
//		}
//	}
//
//	realFreq = periphClock / (double)(arr * psc);
//	htim_base->Instance->ARR = (arr - 1);
//	htim_base->Instance->PSC = (psc - 1);
//
//	return realFreq;

	int32_t clkDiv;
	uint16_t prescaler;
	uint16_t autoReloadReg;
	uint32_t errMinRatio = 0;
	double realFreq;
	uint8_t result = UNKNOW_ERROR;

	clkDiv = ((2 * periphClock / reqFreq) + 1) / 2; //to minimize rounding error

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

	realFreq = periphClock / (double)((prescaler + 1) * (autoReloadReg + 1));

	htim_base->Instance->ARR = autoReloadReg;
	htim_base->Instance->PSC = prescaler;
	LL_TIM_GenerateEvent_UPDATE(htim_base->Instance);

	return realFreq;
}

/**
 * @brief  Rounding function.
 * @retval rounded: rounded value
 */
uint32_t roundNumber(double num)
{
     uint32_t rounded = (uint32_t)(num + 0.5);
     return rounded;
}


/**
 * @}
 */

/**
 * @}
 */

/*********** END MY FRIEND ***********/
///**
//  ******************************************************************************
//  * File Name          : TIM.c
//  * Date               : 18/01/2015 10:00:31
//  * Description        : This file provides code for the configuration
//  *                      of the TIM instances.
//  ******************************************************************************
//  *
//  * COPYRIGHT(c) 2015 STMicroelectronics
//  *
//  * Redistribution and use in source and binary forms, with or without modification,
//  * are permitted provided that the following conditions are met:
//  *   1. Redistributions of source code must retain the above copyright notice,
//  *      this list of conditions and the following disclaimer.
//  *   2. Redistributions in binary form must reproduce the above copyright notice,
//  *      this list of conditions and the following disclaimer in the documentation
//  *      and/or other materials provided with the distribution.
//  *   3. Neither the name of STMicroelectronics nor the names of its contributors
//  *      may be used to endorse or promote products derived from this software
//  *      without specific prior written permission.
//  *
//  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  *
//  ******************************************************************************
//  */
//
///* Includes ------------------------------------------------------------------*/
//#include "tim.h"
//#include "err_list.h"
///* USER CODE BEGIN 0 */
//
///* USER CODE END 0 */
//#ifdef USE_SCOPE
//TIM_HandleTypeDef htim_scope;
//#endif //USE_SCOPE
//
//#ifdef USE_GEN
//TIM_HandleTypeDef htim6;
//TIM_HandleTypeDef htim7;
//#endif //USE_GEN
//
//#ifdef USE_SCOPE
///* TIM3 init function */
//void MX_TIM3_Init(void)
//{
//  TIM_MasterConfigTypeDef sMasterConfig;
//
//  htim_scope.Instance = TIM3;
//  htim_scope.Init.Prescaler = 0;
//  htim_scope.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim_scope.Init.Period = 2048;
//  htim_scope.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  HAL_TIM_Base_Init(&htim_scope);
//
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  HAL_TIMEx_MasterConfigSynchronization(&htim_scope, &sMasterConfig);
//
//}
//#endif //USE_SCOPE
//
//




//
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
//{
//	#ifdef USE_SCOPE
//  if(htim_base->Instance==TIM3)
//  {
//  /* USER CODE BEGIN TIM3_MspInit 0 */
//
//  /* USER CODE END TIM3_MspInit 0 */
//    /* Peripheral clock enable */
//    __TIM3_CLK_ENABLE();
//  /* USER CODE BEGIN TIM3_MspInit 1 */
//
//  /* USER CODE END TIM3_MspInit 1 */
//  }
//	#endif //USE_SCOPE
//
//		#ifdef USE_GEN
//	if(htim_base->Instance==TIM6){
//		__TIM6_CLK_ENABLE();
//	}
//	if(htim_base->Instance==TIM7){
//		__TIM7_CLK_ENABLE();
//	}
//	#endif //USE_GEN
//}
//
//void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
//{
//	#ifdef USE_SCOPE
//  if(htim_base->Instance==TIM3)
//  {
//  /* USER CODE BEGIN TIM3_MspDeInit 0 */
//
//  /* USER CODE END TIM3_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __TIM3_CLK_DISABLE();
//  /* USER CODE BEGIN TIM3_MspDeInit 1 */
//
//  /* USER CODE END TIM3_MspDeInit 1 */
//  }
//	#endif //USE_SCOPE
//
//
//	#ifdef USE_GEN
//	if(htim_base->Instance==TIM6){
//		__TIM6_CLK_DISABLE();
//	}
//	if(htim_base->Instance==TIM7){
//		__TIM7_CLK_DISABLE();
//	}
//	#endif //USE_GEN
//
//}
//
///* USER CODE BEGIN 1 */
//#ifdef USE_SCOPE
//uint8_t TIM_Reconfig_scope(uint32_t samplingFreq,uint32_t* realFreq){
//	return TIM_Reconfig(samplingFreq,&htim_scope,realFreq);
//}
//#endif //USE_SCOPE
//
//#ifdef USE_GEN
//uint8_t TIM_Reconfig_gen(uint32_t samplingFreq,uint8_t chan,uint32_t* realFreq){
//	if(chan==0){
//		return TIM_Reconfig(samplingFreq,&htim6,realFreq);
//	}else if(chan==1){
//		return TIM_Reconfig(samplingFreq,&htim7,realFreq);
//	}else{
//		return 0;
//	}
//}
//#endif //USE_GEN
//
//
//
//uint8_t TIM_Reconfig(uint32_t samplingFreq,TIM_HandleTypeDef* htim_base,uint32_t* realFreq){
//
//	int32_t clkDiv;
//	uint16_t prescaler;
//	uint16_t autoReloadReg;
//	uint32_t errMinRatio = 0;
//	uint8_t result = UNKNOW_ERROR;
//
//
//
//	clkDiv = ((2*HAL_RCC_GetPCLK2Freq() / samplingFreq)+1)/2; //to minimize rounding error
//
//	if(clkDiv == 0){ //error
//		result = GEN_FREQ_MISMATCH;
//	}else if(clkDiv <= 0x0FFFF){ //Sampling frequency is high enough so no prescaler needed
//		prescaler = 0;
//		autoReloadReg = clkDiv - 1;
//		result = 0;
//	}else{	// finding prescaler and autoReload value
//		uint32_t errVal = 0xFFFFFFFF;
//		uint32_t errMin = 0xFFFFFFFF;
//		uint16_t ratio = clkDiv>>16;
//		uint16_t div;
//
//		while(errVal != 0){
//			ratio++;
//			div = clkDiv/ratio;
//			errVal = clkDiv - (div*ratio);
//
//			if(errVal < errMin){
//				errMin = errVal;
//				errMinRatio = ratio;
//			}
//
//			if(ratio == 0xFFFF){ //exact combination wasnt found. we use best found
//				div = clkDiv/errMinRatio;
//				ratio = errMinRatio;
//				break;
//			}
//		}
//
//		if(ratio > div){
//			prescaler = div - 1;
//			autoReloadReg = ratio - 1;
//		}else{
//			prescaler = ratio - 1;
//			autoReloadReg = div - 1;
//		}
//
//		if(errVal){
//			result = GEN_FREQ_IS_INACCURATE;
//		}else{
//			result = 0;
//		}
//	}
//
//	*realFreq=HAL_RCC_GetPCLK2Freq()/((prescaler+1)*(autoReloadReg+1));
//  htim_base->Init.Prescaler = prescaler;
//  htim_base->Init.Period = autoReloadReg;
//  HAL_TIM_Base_Init(htim_base);
//	return result;
//
//}
//
//
//#ifdef USE_SCOPE
//void TIMScopeEnable(){
//	HAL_TIM_Base_Start(&htim_scope);
//}
//void TIMScopeDisable(){
//	HAL_TIM_Base_Stop(&htim_scope);
//}
//
//void TIMScopeInit(void){
//	MX_TIM3_Init();
//}
//
//uint32_t getMaxScopeSamplingFreq(uint8_t ADCRes){
//	if(ADCRes==12){
//		return 2000000;
//	}else if(ADCRes==8){
//		return 2500000;
//	}
//	return HAL_RCC_GetPCLK2Freq()/(ADCRes+2);
//}
//#endif //USE_SCOPE
//
//	#ifdef USE_GEN
//void TIMGenEnable(void){
//  HAL_TIM_Base_Start(&htim6);
//	HAL_TIM_Base_Start(&htim7);
//}
//void TIMGenDisable(void){
//  HAL_TIM_Base_Stop(&htim6);
//	HAL_TIM_Base_Stop(&htim7);
//}
//
//void TIMGenInit(){
//	MX_TIM6_Init();
//	MX_TIM7_Init();
//}
//	#endif //USE_GEN
//
//
//
///* USER CODE END 1 */
//
///**
//  * @}
//  */
//
///**
//  * @}
//  */
//
///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
