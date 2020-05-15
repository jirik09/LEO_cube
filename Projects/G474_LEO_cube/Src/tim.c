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
#include "stm32g4xx_hal_tim.h"
#include "stm32g4xx_ll_tim.h"

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
	if (htim_base->Instance == TIM15) {
		TIM15_SCOPE_MspInit(htim_base);
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
		if (htim_base->Instance == TIM1) {
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
#endif
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
	if (htim_base->Instance == TIM15) {
		TIM15_SCOPE_MspDeinit(htim_base);
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
		if (htim_base->Instance == TIM1) {
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
