/*
  *****************************************************************************
  * @file    mcu_config.h
  * @author  Y3288231
  * @date    jan 15, 2014
  * @brief   Hardware abstraction for communication
  ***************************************************************************** 
*/ 
#ifndef STM32F4_CONFIG_H_
#define STM32F4_CONFIG_H_

#include "stm32g4xx_hal.h"
/* Prevent duplicate definition warning for __ALIGN_END between HAL and USB Device library */
#ifdef __ALIGN_END
#undef __ALIGN_END
#endif
//#include "stm32f3xx_nucleo.h"
#include "firmware_version.h"
//#include "usb_device.h"
#include "math.h"
#include "err_list.h"

/* Rely on HAL/USB headers for __ALIGN_END definition; no local override to prevent redefinition warnings. */

#define IDN_STRING "Nucleo-G474RE" //max 30 chars
#define SHIELD_STRING " + Shield LEO V0.1"
#define SHIELD_STRING_2 " + Shield LEO V0.2"
#define MCU "STM32G474RE"

#define MCU_UID (uint8_t)*((uint8_t *) 0x1FFF7590)

/* -------------------------------------------------------------------------
 * Resource bit flags (DMAx_R, TIMx_R, ADCxxx_R, etc.) expected by Common layer
 * Older MCU variants include these via resources.h; replicate minimal set here
 * for G474 to satisfy comms serialization without pulling legacy header.
 * Bits must be consistent across variants so PC application decodes properly.
 * Assign unique bit positions matching F303/F446 ordering (implicit contract).
 * ------------------------------------------------------------------------- */
#ifndef RESOURCES_DEFINED_G474
#define RESOURCES_DEFINED_G474
/* NOTE: If resources.h is later centralized, remove these duplicates. */
#define DMA1_R   (1u<<0)
#define DMA2_R   (1u<<1)
#define ADC12_R  (1u<<2)
#define ADC34_R  (1u<<3)
#define ADC123_R (ADC12_R) /* For legacy F4 style triple ADC macro reuse */
#define TIM1_R   (1u<<4)
#define TIM2_R   (1u<<5)
#define TIM3_R   (1u<<6)
#define TIM4_R   (1u<<7)
#define TIM6_R   (1u<<8)
#define TIM7_R   (1u<<9)
#define TIM8_R   (1u<<10)
#define DAC12_R  (1u<<11)
/* Additional flags that may be serialized elsewhere */
#define TIM13_R  (1u<<12)
#endif

// Communication constatnts ===================================================
#define COMM_BUFFER_SIZE 512
//#define COMM_TX_BUFFER_SIZE 256
#define COMMS_BULK_SIZE 200
//#define UART_SPEED 115200
#define UART_SPEED 921600

#define USART_GPIO GPIOA
#define USART_TX GPIO_PIN_2
#define USART_RX GPIO_PIN_3
#define USART_TX_PIN_STR "PA2_" //must be 4 chars
#define USART_RX_PIN_STR "PA3_" //must be 4 chars

#define USB_DP_PIN_STR "PA12" //must be 4 chars
#define USB_DM_PIN_STR "PA11" //must be 4 chars

// Scope shield identification ================================================
#define D7_GPIO GPIOA
#define D7_PIN GPIO_PIN_8
#define D8_GPIO GPIOA
#define D8_PIN GPIO_PIN_9



// Scope constants ===========================================================
/* Provide unified resource bitmask for PC application */
#ifndef SCOPE_RESOURCES
#define SCOPE_RESOURCES DMA1_R|ADC123_R|TIM2_R
#endif
#define MAX_SAMPLING_FREQ_12B 4000000 //smps
#define MAX_SAMPLING_FREQ_8B 4800000 //smps
#define MAX_INTERLEAVE_FREQ_12B 8000000 //smps
#define MAX_INTERLEAVE_FREQ_8B 9600000 //smps
#define MAX_ADC_CHANNELS 4
#ifndef MAX_INTERLEAVED_CHANNELS
#define MAX_INTERLEAVED_CHANNELS 1 /* G474 design allows interleave of 2 ADCs for 1 channel */
#endif
//#define MIN_ADC_SAMPLING_TIME 2 //1.5 + 0.5 cycle from ADC resolution


#define MAX_SCOPE_BUFF_SIZE 60000//40000 //in bytes
#define SCOPE_BUFFER_MARGIN 100

#define SCOPE_CH1_PIN_STR "A5__" //must be 4 chars
#define SCOPE_CH2_PIN_STR "A4__" //must be 4 chars
#define SCOPE_CH3_PIN_STR "A3__" //must be 4 chars
#define SCOPE_CH4_PIN_STR "PB14" //must be 4 chars

#define SCOPE_VREF 3300
#ifndef AVDD_DEFAULT
#define AVDD_DEFAULT SCOPE_VREF
#endif
#ifndef VREF_INT
/* Map to calibrated internal reference used on F3/F4 (unit: mV) */
#define VREF_INT SCOPE_VREF_INT
#endif
#define MCU_VREF_PLUS 3300// G474 and new nucleos allows to use VDD 3.3V or generated 3.25V voltage reference for VREF+ pin. Enter the right value
#define SCOPE_VREF_INT (uint16_t)*((uint16_t *)0x1FFF75AA)*3000/MCU_VREF_PLUS //Vrefint is calibrated with Vcc 3.0V therefore it has to be rescaled.

/* Default scope voltage ranges (same semantic as other MCUs) */
#define RANGE_1_LOW 0
#define RANGE_1_HI SCOPE_VREF
#define RANGE_2_LOW -SCOPE_VREF
#define RANGE_2_HI (SCOPE_VREF*2)
#define RANGE_3_LOW 0
#define RANGE_3_HI (SCOPE_VREF*2)
#define RANGE_4_LOW 0
#define RANGE_4_HI 0

#ifdef USE_SHIELD
	#define HIGH_RANGE 16666 //without calibration 16666  16820
	#define MID_RANGE  3300  //without calibration 3300  3258
	#define LOW_RANGE  330   //without calibration 166

	#define SHIELD_RANGE_1_LOW 0
	#define SHIELD_RANGE_1_HI 2*MID_RANGE
	#define SHIELD_RANGE_2_LOW -MID_RANGE
	#define SHIELD_RANGE_2_HI MID_RANGE
	#define SHIELD_RANGE_3_LOW -HIGH_RANGE//+150
	#define SHIELD_RANGE_3_HI HIGH_RANGE
	#define SHIELD_RANGE_4_LOW -LOW_RANGE//-9
	#define SHIELD_RANGE_4_HI LOW_RANGE
#endif


/* ADC channel mapping moved to adc_channels.[ch] */
#include "adc_channels.h"
// Generator constatnts =======================================================

/* Generator constants ===================================================== */
#define GENERATOR_RESOURCES DMA2_R|DAC12_R|TIM6_R|TIM7_R
#ifndef DAC_RESOURCES
#define DAC_RESOURCES DAC12_R
#endif
#define MAX_GENERATING_FREQ 2000000 /* smps base max */
/* Legacy compatibility: provide expected macros used by common generator code */
#ifndef MAX_DAC_GENERATING_FREQ
#define MAX_DAC_GENERATING_FREQ  MAX_GENERATING_FREQ
#endif
#ifndef MAX_PWM_GENERATING_FREQ
#define MAX_PWM_GENERATING_FREQ  4800000
#endif
#ifndef GEN_TIM_PERIPH_CLOCK
#define GEN_TIM_PERIPH_CLOCK 168000000
#endif
#define MAX_DAC_CHANNELS 2
#define MAX_GENERATOR_BUFF_SIZE 10000//2000
#define DAC_DATA_DEPTH 12

/* Legacy range macros used by comms (mapped to G474 values) */
#ifndef GEN_RANGE_LOW
#define GEN_RANGE_LOW 0
#endif
/* GEN_VREF defined below; ensure ordering so GEN_RANGE_HIGH resolves correctly */
#ifndef GEN_RANGE_HIGH
#define GEN_RANGE_HIGH  GEN_VREF
#endif

#define GEN_VREF 3300
#define GEN_VDDA 3300
#define GEN_VREF_INT 1210

#ifdef USE_SHIELD
	#define SHIELD_GEN_HIGH   5000 //without calibration 5000  4898-5
	#define SHIELD_GEN_LOW   -5000 //without calibration -5000  -4898-5
#endif

/* Crystal accuracy (ppm) for counter error estimation */
#ifndef NUCLEO_CRYSTAL_ERROR_PPM
#define NUCLEO_CRYSTAL_ERROR_PPM  20
#endif
#ifndef NUCLEO_CRYSTAL_ERROR
#define NUCLEO_CRYSTAL_ERROR (20.0 / 1000000)
#endif

/* Generator function name compatibility (underscore variants) */
/* Forward prototypes to remove implicit declaration warnings (adapt to actual signatures) */
#ifdef __cplusplus
extern "C" {
#endif
/* Actual G474 implementation uses the following names (see tim_gen.c):
 *  TIMGenInit / TIMGenDacDeinit / TIMGenEnable / TIMGenDisable
 *  TIMGenPwmInit / TIMGenPwmDeinit / PWMGeneratingEnable / PWMGeneratingDisable
 *  TIM_DMA_Reconfig / TIM_Reconfig_gen / TIM_Reconfig_GenPwm
 */
void TIMGenInit(void);
void TIMGenDacDeinit(void);
void TIMGenEnable(void);
void TIMGenDisable(void);
void TIMGenPwmInit(void);
void TIMGenPwmDeinit(void);
void PWMGeneratingEnable(void);
void PWMGeneratingDisable(void);
void TIM_DMA_Reconfig(uint8_t channel);
uint8_t TIM_Reconfig_gen(uint32_t samplingFreq,uint8_t chan,uint32_t* realFreq);
double  TIM_Reconfig_GenPwm(double reqFreq, uint8_t chan);
uint8_t TIM_Reconfig_gen_all(uint32_t samplingFreq, uint32_t *realFreq);
/* Provide optional pattern stubs if not implemented */
void TIM_GenPattern_DmaReconfig(void);
void TIM_GenPattern_Start(void);
void TIM_GenPattern_Stop(void);
/* Data transfer (legacy aliases map to TIM_Reconfig_gen) */
/* realFreqOut pointer used for returning effective frequency */
/* Sync PWM (tim_sync_pwm.c) forward prototypes */
void TIM_SYNC_PWM_Init(void);
void TIM_SYNC_PWM_Deinit(void);
void TIM_SYNC_PWM_Start(void);
void TIM_SYNC_PWM_Stop(void);
void TIM_SYNC_PWM_ChannelState(uint8_t channel, uint8_t state);
double TIM_SYNC_PWM_SetFreqCh1(double freq);
/* Generic Sync PWM channel-based API (macros in common layer map legacy names
 * like TIM_SYNC_PWM_SetFreqCh1 to TIM_SYNC_PWM_SetFreqChannel(0, freq) etc.) */
double TIM_SYNC_PWM_SetFreqChannel(uint8_t channel, double freq);
void   TIM_SYNC_PWM_SetChannelDutyPhase(uint32_t channel, double dutyCycle, double phase);
void   TIM_SYNC_PWM_SetChannelInvert(uint8_t channel, uint8_t setInvert);
void TIM_SYNC_PWM_StepMode_Enable(void);
void TIM_SYNC_PWM_StepMode_Disable(void);
/* ADC measured VDD accessor expected by common comms code */
int32_t getRealAVDD(void);
#ifdef __cplusplus
}
#endif
/* Macro aliases expected by legacy common code */
/* Map legacy expected names to actual G474 implementations */
#define TIM_GenSignal_Init               TIMGenInit
#define TIM_GenSignal_Deinit             TIMGenDacDeinit
#define TIM_GenSignal_Start              TIMGenEnable
#define TIM_GenSignal_Stop               TIMGenDisable
#define TIM_GenPwm_Init                  TIMGenPwmInit
#define TIM_GenPwm_Deinit                TIMGenPwmDeinit
#define TIM_GenPwm_Start                 PWMGeneratingEnable
#define TIM_GenPwm_Stop                  PWMGeneratingDisable
#define TIM_GenPwm_FreqReconfig          TIM_Reconfig_GenPwm
#define TIM_GenPwm_DmaReconfig           TIM_DMA_Reconfig
/* Legacy data transfer frequency helpers */
#define TIM_DataTransfer_FreqReconfig(freq,chan,realPtr)   TIM_Reconfig_gen((freq),(chan),(realPtr))
/* All-channels variant: iterate in calling code if needed; provide macro returning error=0 */
#define TIM_DataTransfer_FreqReconfigAll(freq,realPtr)     TIM_Reconfig_gen_all((freq),(realPtr))
/* Pattern support not present: provide safe no-op aliases unless implemented */
#ifndef TIM_GenPattern_Init
#define TIM_GenPattern_Init()            ((void)0)
#endif
#ifndef TIM_GenPattern_DmaReconfig
#define TIM_GenPattern_DmaReconfig()     ((void)0)
#endif
#ifndef TIM_GenPattern_Start
#define TIM_GenPattern_Start()           ((void)0)
#endif
#ifndef TIM_GenPattern_Stop
#define TIM_GenPattern_Stop()            ((void)0)
#endif
#ifndef TIM_GenPattern_Deinit
#define TIM_GenPattern_Deinit()          ((void)0)
#endif
/* Sync PWM legacy aliases (wrap channel oriented generic functions) */
#ifndef TIM_SYNC_PWM_SetFreqCh1
#define TIM_SYNC_PWM_SetFreqCh1(freq)            TIM_SYNC_PWM_SetFreqChannel(0,(freq))
#endif
#ifndef TIM_SYNC_PWM_SetFreqCh2
#define TIM_SYNC_PWM_SetFreqCh2(freq)            TIM_SYNC_PWM_SetFreqChannel(1,(freq))
#endif
/* Only Ch1 & Ch2 implemented for frequency set; keep others as no-ops */
/* Frequency setters for Ch3/Ch4 not implemented: keep harmless stubs */
#ifndef TIM_SYNC_PWM_SetFreqCh3
#define TIM_SYNC_PWM_SetFreqCh3(freq)            (0.0)
#endif
#ifndef TIM_SYNC_PWM_SetFreqCh4
#define TIM_SYNC_PWM_SetFreqCh4(freq)            (0.0)
#endif
/* Do NOT create self-referential macros for SetChanDutyPhase/Invert to avoid
 * macro expansion on function definitions provided in tim_sync_pwm.c */

/* DAC compatibility (no-op on G474 unless implemented elsewhere) */
#ifndef DAC_Deinit
#define DAC_Deinit() ((void)0)
#endif
#ifndef DAC_SetMode_SignalGenerator
#define DAC_SetMode_SignalGenerator() ((void)0)
#endif
#ifndef DAC_SetMode_VoltageSource
#define DAC_SetMode_VoltageSource() ((void)0)
#endif
#ifndef DAC_OutputBuffer_Enable
#define DAC_OutputBuffer_Enable() ((void)0)
#endif
#ifndef DAC_OutputBuffer_Disable
#define DAC_OutputBuffer_Disable() ((void)0)
#endif
#ifndef DAC_SetOutput
#define DAC_SetOutput(val,ch) ((void)(val),(void)(ch))
#endif
/* Map legacy DAC wrapper names (expected by Common/generator.c) to G4 names */
#ifndef DAC_DmaReconfig
#define DAC_DmaReconfig               DAC_DMA_Reconfig
#endif
#ifndef DAC_Output_Enable
#define DAC_Output_Enable             DACEnableOutput
#endif
#ifndef DAC_Output_Disable
#define DAC_Output_Disable            DACDisableOutput
#endif
#ifndef DAC_Disable
#define DAC_Disable                   DACDisableOutput
#endif
/* Provide minimal getRealAVDD fallback if ADC code hasn't updated value yet */
#ifndef GET_REAL_AVDD_DEFAULT
#define GET_REAL_AVDD_DEFAULT  (AVDD_DEFAULT)
#endif

/* If actual implementation absent, supply a weak stub in a later C file (see note). */

/* UART init mapping: enforce single definition for this variant. */
#ifdef MX_UART_Init
#undef MX_UART_Init
#endif
#define MX_UART_Init MX_USART1_UART_Init

/* Pattern generator linkage helpers: if pattern generation not supported on this
 * variant ensure common code still links by mapping calls to stub functions that
 * we provide in generator.c. (Do not macro-map to ((void)0) again to avoid hiding
 * the real weak stubs and reintroducing undefined references at link time.) */
#if !defined(USE_GEN_PATTERN)
#undef genPatternInit
#undef genPatternGeneratingEnable
#undef genPatternGeneratingDisable
#undef genPatternPortDataShift
/* Forward declare stub signatures (implemented as no-op in generator.c) */
void genPatternInit(void);
void genPatternGeneratingEnable(void);
void genPatternGeneratingDisable(void);
void genPatternPortDataShift(uint16_t *mem, uint16_t dataLength);
#endif

#define GEN_CH1_PIN_STR "A2__" //must be 4 chars
#define GEN_CH2_PIN_STR "D13_" //must be 4 chars

// Counter constatnts =======================================================
#ifdef USE_COUNTER
	/* Resource bitmask (replicate semantics of older MCUs) */
	#ifndef COUNTER_RESOURCES
	#define COUNTER_RESOURCES TIM2_R|TIM4_R
	#endif
	/* TIM4 -> ARR & PSC set to gate 100 ms */
	#define TIM4_ARR                                        999
	#define TIM4_PSC                                        16799
	#define IC_THRESHOLD                                    20
	/* Modes string (legacy) */
	#define COUNTER_MODES                                   "HF LF FR EV"
	/* Frequency ranges (copied from F303, adapt clocks if needed) */
	#define CNT_HF_MAX       (uint32_t)300000000
	#define CNT_HF_MIN_TG_01 (uint32_t)38000
	#define CNT_HF_MIN_TG_05 (uint32_t)16970
	#define CNT_HF_MIN_TG_1  (uint32_t)12000
	#define CNT_HF_MIN_TG_5  (uint32_t)5366
	#define CNT_HF_MIN_TG_10 (uint32_t)3800
	#define CNT_LF_MAX       (uint32_t)1000000
	#define CNT_LF_MIN       (double)0.033528
	#define CNT_REF_INPUT_MAX (uint32_t)36000000
	#define CNT_REF_CHAN_MAX  (uint32_t)36000000
	/* Legacy pin string macros expected by comms (map to existing short forms) */
	#define CNT_IC_PIN_CH1 "A0__"
	#define CNT_IC_PIN_CH2 "A1__"
	#define CNT_REF_PIN_IN1 "D7__"
	#define CNT_REF_PIN_IN2 "A0__"
	#define CNT_TI_PIN_CH1  "A0__"
	#define CNT_TI_PIN_CH2  "A1__"
	/* Original pin macros kept for local code */
	#define CNT_ETR_PIN       "A0 "
	#define CNT_IC_CH1_PIN    "A0 "
	#define CNT_IC_CH2_PIN    "A1 "
	#define CNT_REF1_PIN      "D7 "
	#define CNT_REF2_PIN      "A0 "
	/* Peripheral clocks */
	#define CNT_COUNTER_PERIPH_CLOCK   (uint32_t)168000000
	#define CNT_GATE_PERIPH_CLOCK      (uint32_t)168000000
#endif //USE_COUNTER

// PWM generator constants =================================================
#ifdef USE_GEN_PWM
	#ifndef GEN_PWM_RESOURCES
	#define GEN_PWM_RESOURCES DMA2_R|TIM6_R|TIM7_R|TIM1_R|TIM3_R
	#endif
	#define GEN_PWM_CH1_PIN                         "D8__" // PA9
	#define GEN_PWM_CH2_PIN                         "D5__" // PB4
	#define MAX_GEN_PWM_CHANNELS  2
	/* Provide single legacy macro name used by comms for peripheral clock */
	#ifndef GEN_PWM_TIM_PERIPH_CLOCK
	#define GEN_PWM_TIM_PERIPH_CLOCK (uint32_t)168000000
	#endif
	/* Retain per-channel clocks if used elsewhere */
	#define GEN_PWM_CH1_TIM_PERIPH_CLOCK (uint32_t)168000000
	#define GEN_PWM_CH2_TIM_PERIPH_CLOCK (uint32_t)168000000
#endif //USE_GEN_PWM

// Synchronized PWM generator constants ====================================
#ifdef USE_SYNC_PWM
	#ifndef SYNC_PWM_RESOURCES
	#define SYNC_PWM_RESOURCES TIM1_R|TIM3_R|TIM8_R
	#endif
	#define SYNC_PWM_TIM_PERIPH_CLOCK   (uint32_t)168000000
	#define MAX_SYNC_PWM_FREQ           (uint32_t)100000
	#define MAX_SYNC_PWM_CHANNELS       (uint32_t)4
	/* Legacy dependence & disable state macros (match F303/F446 semantics) */
	#ifndef SYNC_PWM_CHANNELS_DEPENDENCE
	#define SYNC_PWM_CHANNELS_DEPENDENCE (uint32_t)0
	#endif
	#ifndef SYNC_PWM_DRIVE_DISAB_CHANx
	#define SYNC_PWM_DRIVE_DISAB_CHANx (uint32_t)0
	#endif
	#ifndef SYNC_PWM_DRIVE_DISAB_CHANy
	#define SYNC_PWM_DRIVE_DISAB_CHANy (uint32_t)1
	#endif
	#ifndef SYNC_PWM_FREQ_DISAB_CHANx
	#define SYNC_PWM_FREQ_DISAB_CHANx  (uint32_t)2
	#endif
	#ifndef SYNC_PWM_FREQ_DISAB_CHANy
	#define SYNC_PWM_FREQ_DISAB_CHANy  (uint32_t)3
	#endif
	#define SYNC_PWM_CH1_PIN            "PC6_"
	#define SYNC_PWM_CH2_PIN            "PC7_"
	#define SYNC_PWM_CH3_PIN            "PC8_"
	#define SYNC_PWM_CH4_PIN            "PC9_"
#endif //USE_SYNC_PWM

// Logic Analyzer constants ====================================
#ifdef USE_LOG_ANLYS
	/* Nucleo F303RE - TIM4 (posttrigger), TIM1 (timebase) */
	#define LOG_ANLYS_POSTTRIG_PERIPH_CLOCK	   (uint32_t) 168000000
	#define LOG_ANLYS_TIMEBASE_PERIPH_CLOCK		 (uint32_t) 168000000
	/* Sampling frequency depends on DMA transfer possibilities. Half of APB clock freq. */
	#define LOG_ANLYS_SAMPLING_FREQ						 (uint32_t) (84000000 / 2)
	#define LOG_ANLYS_BUFFER_LENGTH						 20000 //(uint32_t) (MAX_SCOPE_BUFF_SIZE / 2);
	#define LOG_ANLYS_CHANNELS_NUM							8
	
	#define LOG_ANLYS_PIN_CH1						"PB6_"
	#define LOG_ANLYS_PIN_CH2						"PB7_"
	#define LOG_ANLYS_PIN_CH3						"PB8_"
	#define LOG_ANLYS_PIN_CH4						"PB9_"
	#define LOG_ANLYS_PIN_CH5						"PB10"
	#define LOG_ANLYS_PIN_CH6						"PB11"
	#define LOG_ANLYS_PIN_CH7						"PB12"
	#define LOG_ANLYS_PIN_CH8						"PB13"
	
#endif //USE_LOG_ANLYS

//Definition of assert to check length of strings (no unused typedef warning)
#undef CASSERT
#define CASSERT(ex) do { (void)sizeof(char[(ex)?1:-1]); } while(0)

/* Counter gate timing legacy alias (F303 name -> G4 implementation) */
#ifndef TIM_ETR_ARR_PSC_Config
#define TIM_ETR_ARR_PSC_Config(gateTime) TIM_ARR_PSC_Config(gateTime)
#endif

/* Ensure signal generator code paths enabled if generic USE_GEN defined */
#if defined(USE_GEN) && !defined(USE_GEN_SIGNAL)
#define USE_GEN_SIGNAL
#endif

/* Provide Error_Handler prototype for modules performing implicit call */
void Error_Handler(void);


#endif /* STM32F4_CONFIG_H_ */
