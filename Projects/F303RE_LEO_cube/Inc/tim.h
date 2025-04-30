/**
 ******************************************************************************
 * @file		tim.h
 * @date   18/01/2015 10:00:31
 * @brief	This file provides code for the configuration
 *         of the TIM instances.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

static TIM_HandleTypeDef htim1;
static TIM_HandleTypeDef htim3;
static TIM_HandleTypeDef htim4;

typedef enum{
	false = 0,
	true = 1
} bool;

uint8_t TIM_Reconfig(TIM_HandleTypeDef* htim_base, uint32_t periphClock, uint32_t samplingFreq, uint32_t* realFreq, _Bool isFreqPassed);
uint8_t TIM_Getconfig(uint32_t * arr, uint32_t * psc, uint32_t periphClock, uint32_t samplingFreq, uint32_t* realFreq, _Bool isFreqPassed);
double TIM_ReconfigPrecise(TIM_HandleTypeDef* htim_base, uint32_t periphClock, double reqFreq);
uint32_t roundNumber(double num);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(USE_GEN_SIGNAL) || defined(USE_GEN_PWM) || defined(USE_GEN_PATTERN)

void MX_TIM6_Init(void);
void MX_TIM7_Init(void);
uint8_t TIM_DataTransfer_FreqReconfig(uint32_t samplingFreq,uint8_t chan,uint32_t* realFreq);
uint8_t TIM_DataTransfer_FreqReconfigAll(uint32_t samplingFreq,uint32_t* realFreq);

#endif //USE_GEN_SIGNAL || USE_GEN_PWM || USE_GEN_PATTERN
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_SCOPE

void TIMScopeEnable(void);
void TIMScopeDisable(void);
void MX_TIM15_Init(void);
void TIM15_SCOPE_MspInit(TIM_HandleTypeDef* htim_base);
void TIM15_SCOPE_MspDeinit(TIM_HandleTypeDef* htim_base);
uint32_t getMaxScopeSamplingFreq(uint8_t ADCRes);
uint32_t getMaxScopeSamplingFreqInterleaved(uint8_t ADCRes);
uint8_t TIM_Reconfig_scope(uint32_t samplingFreq,uint32_t* realFreq);

#endif //USE_SCOPE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(USE_GEN_SIGNAL) || defined(USE_GEN_PWM) || defined(USE_GEN_PATTERN)
#ifdef USE_GEN_SIGNAL

void TIM6_GEN_SIGNAL_MspInit(TIM_HandleTypeDef* htim_base);
void TIM7_GEN_SIGNAL_MspInit(TIM_HandleTypeDef* htim_base);
void TIM6_GEN_SIGNAL_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM7_GEN_SIGNAL_MspDeinit(TIM_HandleTypeDef* htim_base);

void TIM_GenSignal_Init(void);
void TIM_GenSignal_Deinit(void);
void TIM_GenSignal_Start(void);
void TIM_GenSignal_Stop(void);

#endif //USE_GEN_SIGNAL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_GEN_PWM

void MX_TIM1_GEN_PWM_Init(void);
void MX_TIM3_GEN_PWM_Init(void);

void TIM1_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM3_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM6_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM7_GEN_PWM_MspInit(TIM_HandleTypeDef* htim_base);
void TIM1_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM3_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM6_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);
void TIM7_GEN_PWM_MspDeinit(TIM_HandleTypeDef* htim_base);

void TIM_GenPwm_Init(void);
void TIM_GenPwm_Deinit(void);
void TIM_GenPwm_Start(void);
void TIM_GenPwm_Stop(void);
void TIM_GenPwm_DmaReconfig(uint8_t chan);
double TIM_GenPwm_FreqReconfig(double reqFreq, uint8_t chan);

#endif //USE_GEN_PWM
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_GEN_PATTERN

#define GEN_PATTERN_CH0_Pin GPIO_PIN_2
#define GEN_PATTERN_CH1_Pin GPIO_PIN_3
#define GEN_PATTERN_CH2_Pin GPIO_PIN_4
#define GEN_PATTERN_CH3_Pin GPIO_PIN_5
#define GEN_PATTERN_CH4_Pin GPIO_PIN_6
#define GEN_PATTERN_CH5_Pin GPIO_PIN_7
#define GEN_PATTERN_CH6_Pin GPIO_PIN_8
#define GEN_PATTERN_CH7_Pin GPIO_PIN_9
#define GEN_PATTERN_GPIO_Port GPIOC
//#define __GEN_PATTERN_CLOCK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
//#define __GEN_PATTERN_CLOCK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE()

void TIM6_GEN_PATTERN_MspInit(TIM_HandleTypeDef* htim_base);
void TIM6_GEN_PATTERN_MspDeinit(TIM_HandleTypeDef* htim_base);

void TIM_GenPattern_Init(void);
void TIM_GenPattern_Deinit(void);
void TIM_GenPattern_Start(void);
void TIM_GenPattern_Stop(void);
void TIM_GenPattern_DmaReconfig(void);

#endif //USE_GEN_PATTERN
#endif //USE_GEN_SIGNAL || USE_GEN_PWM || USE_GEN_PATTERN
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_SYNC_PWM

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim8;

#define __STEP_MODE_ENABLE()  		TIM8->CR1 |= TIM_CR1_OPM
#define __STEP_MODE_DISABLE()  		TIM8->CR1 &= ~TIM_CR1_OPM

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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_COUNTER

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_tim2_up;
extern DMA_HandleTypeDef hdma_tim2_ch1;
extern DMA_HandleTypeDef hdma_tim2_ch2_ch4;

void TIM_doubleClockVal(void);

void MX_TIM4_Init(void);
void MX_TIM2_ETRorREF_Init(void);
void MX_TIM2_ICorTI_Init(void);

void TIM2_CNT_ETRorREF_MspInit(TIM_HandleTypeDef* htim_base);
void TIM2_CNT_ICorTI_MspInit(TIM_HandleTypeDef* htim_base);
void TIM4_CNT_REForICorTI_MspInit(TIM_HandleTypeDef* htim_base);
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
void TIM_ETR_ARR_PSC_Config(double gateTime);

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

void TIM_REF_InputEnableDisable(_Bool enable);
void TIM_REF_Reconfig_cnt(uint32_t sampleCount);

void TIM_TI_Clear(void);

uint8_t TIM_ETPS_GetPrescaler(void);
uint8_t TIM_IC1PSC_GetPrescaler(void);
uint8_t TIM_IC2PSC_GetPrescaler(void);
uint8_t TIM_GetPrescaler(uint32_t regPrescValue);
void TIM_ClearFlagsIT(TIM_HandleTypeDef* htim_base);

bool DMA_TransferComplete(DMA_HandleTypeDef *dmah);
void DMA_Restart(DMA_HandleTypeDef *dmah);

void COUNTER_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void COUNTER_ETR_DMA_CpltCallback(DMA_HandleTypeDef *dmah);
void COUNTER_IC1_DMA_CpltCallback(DMA_HandleTypeDef *dmah);
void COUNTER_IC2_DMA_CpltCallback(DMA_HandleTypeDef *dmah);

#endif // USE_COUNTER
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */

/****************************** END MY FRIEND ******************************/
