/**
  ******************************************************************************
  * @file		tim.h
  * @date   18/01/2015 10:00:31
  * @brief	This file provides code for the configuration
  *         of the TIM instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup Timers
  * @{
  */

uint8_t TIM_Reconfig(TIM_HandleTypeDef* htim_base, uint32_t periphClock, uint32_t samplingFreq, uint32_t* realFreq, _Bool isFreqPassed);
uint8_t TIM_Getconfig(uint32_t * arr, uint32_t *psc, uint32_t periphClock, uint32_t samplingFreq, uint32_t* realFreq, _Bool isFreqPassed);
double TIM_ReconfigPrecise(TIM_HandleTypeDef* htim_base, uint32_t periphClock, double reqFreq);
uint32_t roundNumber(double num);

/** @addtogroup Scope
  * @{
  */

/** @defgroup Scope_TIM_Prototypes Scope Timers Function Prototypes.
  * @{
  */

#ifdef USE_SCOPE

uint8_t TIM_Reconfig_scope(uint32_t samplingFreq,uint32_t* realFreq);

#endif //USE_SCOPE

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup Arbitrary_DAC_PWM_TIM_Prototypes Arbitrary DAC and PWM Timers Function Prototypes
  * @{
  */

#if defined(USE_GEN) || defined(USE_GEN_PWM)

uint8_t TIM_Reconfig_gen(uint32_t samplingFreq,uint8_t chan,uint32_t* realFreq);
double TIM_Reconfig_GenPwm(double reqFreq, uint8_t chan);
uint8_t TIM_Reconfig_gen_all(uint32_t samplingFreq,uint32_t* realFreq);

#endif //USE_GEN || USE_GEN_PWM

/**
  * @}
  */

/** @defgroup Synch_PWM_TIM_Prototypes Synchronized PWM Timers Function Prototypes.
  * @{
  */

#ifdef USE_SYNC_PWM

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim8;

  #define __STEP_MODE_ENABLE()  		TIM8->CR1 |= TIM_CR1_OPM
	#define __STEP_MODE_DISABLE()  		TIM8->CR1 &= ~TIM_CR1_OPM

#endif //USE_SYNC_PWM

/**
  * @}
  */

/** @addtogroup Scope
  * @{
  */

/** @addtogroup Scope_TIM_Prototypes Scope Timers Function Prototypes
  * @{
  */

#ifdef USE_SCOPE

void TIMScopeEnable(void);
void TIMScopeDisable(void);
void MX_TIM8_Init(void);
void TIM8_SCOPE_MspInit(TIM_HandleTypeDef* htim_base);
void TIM8_SCOPE_MspDeinit(TIM_HandleTypeDef* htim_base);
uint32_t getMaxScopeSamplingFreq(uint8_t ADCRes);
uint32_t getMaxScopeSamplingFreqInterleaved(uint8_t ADCRes);

#endif //USE_SCOPE

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup Arbitrary_DAC_PWM_TIM_Prototypes
  * @{
  */

#if defined(USE_GEN) || defined(USE_GEN_PWM)
#ifdef USE_GEN

void TIMGenEnable(void);
void TIMGenDisable(void);
void MX_TIM6_Init(void);
void MX_TIM7_Init(void);

void TIM6_GEN_DAC_MspInit(TIM_HandleTypeDef* htim_base);
void TIM7_GEN_DAC_MspInit(TIM_HandleTypeDef* htim_base);
void TIM6_GEN_DAC_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM7_GEN_DAC_MspDeinit(TIM_HandleTypeDef* htim_base);

void TIMGenInit(void);
void TIMGenPwmDeinit(void);
void TIMGenDacDeinit(void);

#endif //USE_GEN

/**
  * @}
  */

#ifdef USE_GEN_PWM

/** @defgroup Arbitrary_PWM_TIM_Prototypes Arbitrary PWM Timers Function Prototypes
  * @{
  */

/* PWM generatin timers */
void MX_TIM1_GEN_PWM_Init(void);
void MX_TIM3_GEN_PWM_Init(void);
/* DMA update timers */
void MX_TIM6_GEN_PWM_Init(void);
void MX_TIM7_GEN_PWM_Init(void);

void TIM1_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM3_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM6_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM7_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM1_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM3_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM6_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM7_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);

void TIM_DMA_Reconfig(uint8_t chan);
void TIM_GEN_PWM_PSC_Config(uint16_t pscVal, uint8_t chan);
void TIM_GEN_PWM_ARR_Config(uint16_t arrVal, uint8_t chan);

void TIMGenPwmInit(void);
void PWMGeneratingEnable(void);
void PWMGeneratingDisable(void);

#endif //USE_GEN_PWM
#endif //USE_GEN || USE_GEN_PWM

/**
  * @}
  */

/** @addtogroup Synch_PWM_TIM_Prototypes
  * @{
  */

#ifdef USE_SYNC_PWM

void MX_TIM1_SYNC_PWM_Init(void);
void MX_TIM3_SYNC_PWM_Init(void);
void MX_TIM8_SYNC_PWM_Init(void);
void TIM1_SYNC_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM3_SYNC_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM8_SYNC_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM1_SYNC_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM3_SYNC_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM8_SYNC_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);

void TIM_SYNC_PWM_Init(void);
void TIM_SYNC_PWM_Deinit(void);
void TIM_SYNC_PWM_Start(void);
void TIM_SYNC_PWM_Stop(void);

double TIM_SYNC_PWM_SetFreqCh1(double freq);
double TIM_SYNC_PWM_SetFreqCh2(double freq);

void TIM_SYNC_PWM_SetChanDutyPhase(uint32_t channel, double dutyCycle, double phase);
void TIM_SYNC_PWM_ChannelEnable(uint8_t channel, uint8_t state);
void TIM_SYNC_PWM_SetChanInvert(uint8_t channel, uint8_t setInvert);
void TIM_SYNC_PWM_SetChannelState(uint8_t channel);
void TIM_SYNC_PWM_ClearFlagsIT(TIM_HandleTypeDef* htim_base);

void TIM_SYNC_PWM_StepMode_Enable(void);
void TIM_SYNC_PWM_StepMode_Disable(void);
void TIM_SYNC_PWM_StepMode_EnableInterruptOnSlowTimer(_Bool enable);

#endif // USE_SYNC_PWM

/**
  * @}
  */

/** @addtogroup Logic_Analyzer_TIM_Prototypes
  * @{
  */

#ifdef USE_LOG_ANLYS

extern DMA_HandleTypeDef hdma_tim4_up;

void MX_TIM1_LOG_ANLYS_Init(void);
void MX_TIM4_LOG_ANLYS_Init(void);

void TIM1_LOG_ANLYS_MspInit(TIM_HandleTypeDef* htim_base);
void TIM4_LOG_ANLYS_MspInit(TIM_HandleTypeDef* htim_base);
void TIM4_LOG_ANLYS_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM1_LOG_ANLYS_MspDeinit(TIM_HandleTypeDef* htim_base);

void TIM_LogAnlys_Init(void);
void TIM_LogAnlys_Deinit(void);

void TIM_LogAnlys_Start(void);
void TIM_LogAnlys_Stop(void);

//void TIM_PostTrigger_ARR_PSC_Reconfig(uint32_t arrPsc);
//void TIM_SamplingFreq_ARR_PSC_Reconfig(uint32_t arrPsc);
void TIM_LogAnlys_SamplingFreq_Reconfig(uint32_t smplFreq);
void TIM_LogAnlys_PostTrigger_Reconfig(double posttrigInSec);

void GPIO_EnableTrigger(void);
void GPIO_DisableIRQ(void);
void TIM_PostTrigger_SoftwareStart(void);

void LOG_ANLYS_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void LOG_ANLYS_TriggerEventOccured(void);
void TIM_SamplingStop(void);

#endif //USE_LOG_ANLYS

/**
  * @}
  */

/** @defgroup Counter_TIM_Prototypes Counter Timers Function Prototypes
  * @{
  */

typedef enum{
	false = 0,
	true = 1
} bool;

#ifdef USE_COUNTER

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_tim2_up;
extern DMA_HandleTypeDef hdma_tim2_ch1;
extern DMA_HandleTypeDef hdma_tim2_ch2_ch4;

void TIM_doubleClockVal(void);

static void MX_TIM4_Init(void);
static void MX_TIM2_ETRorREF_Init(void);
static void MX_TIM2_ICorTI_Init(void);

void TIM2_ETRorREF_MspInit(TIM_HandleTypeDef* htim_base);
void TIM2_ICorTI_MspInit(TIM_HandleTypeDef* htim_base);
void TIM4_REForICorTI_MspInit(TIM_HandleTypeDef* htim_base);
void TIM2_CNT_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM4_CNT_MspDeinit(TIM_HandleTypeDef* htim_base);

/* Modes initialization functions */
void TIM_counter_etr_init(void);
void TIM_counter_ref_init(void);
void TIM_counter_ic_init(void);
void TIM_counter_ti_init(void);

void TIM_etr_deinit(void);
void TIM_ref_deinit(void);
void TIM_ic_deinit(void);
void TIM_ti_deinit(void);
void TIM_Counter_Deinit(void);

void TIM_ETR_Start(void);
void TIM_REF_Start(void);
void TIM_IC_Start(void);
void TIM_TI_Start(void);

void TIM_ETR_Stop(void);
void TIM_REF_Stop(void);
void TIM_IC_Stop(void);
void TIM_TI_Stop(void);
void TIM_TI_Init(void);
void TIM_TI_Deinit(void);

/* counter specific */
void TIM_ETRP_Config(double freq);
//void TIM_IC1PSC_Config(double freq);
//void TIM_IC2PSC_Config(double freq);
void TIM_IC1_PSC_Config(uint8_t prescVal);
void TIM_IC2_PSC_Config(uint8_t prescVal);
void TIM_ARR_PSC_Config(uint16_t arr, uint16_t psc);

void TIM_IC1_RisingFalling(void);
void TIM_IC2_RisingFalling(void);
void TIM_IC1_RisingOnly(void);
void TIM_IC2_RisingOnly(void);
void TIM_IC1_FallingOnly(void);
void TIM_IC2_FallingOnly(void);
void TIM_TI_Sequence_AB(void);
void TIM_TI_Sequence_BA(void);
void TIM_TI_ReconfigActiveEdges(void);

void TIM_IC_DutyCycle_Start(void);
void TIM_IC_DutyCycle_Stop(void);
void TIM_IC_DutyCycle_Init(void);
void TIM_IC_DutyCycle_Deinit(void);
void TIM_IC_DutyCycleDmaRestart(void);

void TIM_REF_SecondInputDisable(void);
void TIM_REF_Reconfig_cnt(uint32_t sampleCount);

void TIM_TI_Clear(void);

uint8_t TIM_ETPS_GetPrescaler(void);
uint8_t TIM_IC1PSC_GetPrescaler(void);
uint8_t TIM_IC2PSC_GetPrescaler(void);
uint8_t TIM_GetPrescaler(uint32_t regPrescValue);

bool DMA_TransferComplete(DMA_HandleTypeDef *dmah);
void DMA_Restart(DMA_HandleTypeDef *dmah);

void COUNTER_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void COUNTER_ETR_DMA_CpltCallback(DMA_HandleTypeDef *dmah);
void COUNTER_IC1_DMA_CpltCallback(DMA_HandleTypeDef *dmah);
void COUNTER_IC2_DMA_CpltCallback(DMA_HandleTypeDef *dmah);

#endif // USE_COUNTER
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
///**
//  ******************************************************************************
//  * File Name          : TIM.h
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
///* Define to prevent recursive inclusion -------------------------------------*/
//#ifndef __tim_H
//#define __tim_H
//#ifdef __cplusplus
// extern "C" {
//#endif
//
///* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx_hal.h"
//
//
//uint8_t TIM_Reconfig(uint32_t samplingFreq,TIM_HandleTypeDef* htim_base,uint32_t* realFreq);
//
//#ifdef USE_SCOPE
//uint8_t TIM_Reconfig_scope(uint32_t samplingFreq,uint32_t* realFreq);
//void MX_TIM3_Init(void);
//#endif //USE_SCOPE
//
//#ifdef USE_GEN
//uint8_t TIM_Reconfig_gen(uint32_t samplingFreq,uint8_t chan,uint32_t* realFreq);
//#endif //USE_GEN
//
//#ifdef USE_SCOPE
//uint32_t getMaxScopeSamplingFreq(uint8_t ADCRes);
//void TIMScopeEnable(void);
//void TIMScopeDisable(void);
//void TIMScopeInit(void);
//#endif //USE_SCOPE
//
//#ifdef USE_GEN
//void TIMGenEnable(void);
//void TIMGenDisable(void);
//void TIMGenInit(void);
//#endif //USE_GEN
//
//#ifdef __cplusplus
//}
//#endif
//#endif /*__ tim_H */
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
