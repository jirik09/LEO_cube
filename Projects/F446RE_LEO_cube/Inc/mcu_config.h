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

#include "stm32f4xx_hal.h"
#include "firmware_version.h"
#include "resources.h"
//#include "usb_device.h"
#include "math.h"
#include "err_list.h"

#define IDN_STRING "STM32F446-Nucleo" //max 30 chars
#define MCU "STM32F446RE"

#define MCU_UID 0x1FFF7A10

#define AVDD_DEFAULT 3300
#define VREF_INT (uint16_t)*((uint16_t *)0x1FFF7A2A)

// Communication constatnts ===================================================
#define COMM_BUFFER_SIZE 1500
#define COMMS_BULK_SIZE 200
#define UART_SPEED 921600

#define USART_GPIO GPIOA
#define USART_TX GPIO_PIN_2
#define USART_RX GPIO_PIN_3
#define USART_TX_PIN_STR "PA2_" //must be 4 chars
#define USART_RX_PIN_STR "PA3_" //must be 4 chars 

#define USB_DP_PIN_STR "PA12" //must be 4 chars
#define USB_DM_PIN_STR "PA11" //must be 4 chars

// Scope constatnts ===================================================
#define SCOPE_RESOURCES DMA1_R|ADC123_R|TIM8_R
#define MAX_SAMPLING_FREQ_12B   2400000 //smps
#define MAX_SAMPLING_FREQ_8B    3200000 //smps
#define MAX_INTERLEAVE_FREQ_12B 4800000 //smps
#define MAX_INTERLEAVE_FREQ_8B  6400000 //smps
#define MAX_ADC_CHANNELS 3

#define MAX_SCOPE_BUFF_SIZE 50000 //in bytes
#define SCOPE_BUFFER_MARGIN 200

#define SCOPE_CH1_PIN_STR "A5__" //must be 4 chars
#define SCOPE_CH2_PIN_STR "A4__" //must be 4 chars
#define SCOPE_CH3_PIN_STR "A3__" //must be 4 chars
#define SCOPE_CH4_PIN_STR "----" //must be 4 chars


#define RANGE_1_LOW 0
#define RANGE_1_HI AVDD_DEFAULT
#define RANGE_2_LOW -AVDD_DEFAULT
#define RANGE_2_HI AVDD_DEFAULT*2
#define RANGE_3_LOW 0
#define RANGE_3_HI 0
#define RANGE_4_LOW 0
#define RANGE_4_HI 0

// Generator constatnts ===================================================
#define GENERATOR_RESOURCES DMA2_R|DAC12_R|TIM6_R|TIM7_R
#define DAC_RESOURCES DAC12_R
#define MAX_GENERATING_FREQ 2000000 //smps
#define GEN_TIM_PERIPH_CLOCK 150000000
#define MAX_DAC_CHANNELS 2
#define MAX_GENERATOR_BUFF_SIZE 5000
#define	DAC_DATA_DEPTH 12

#define GEN_RANGE_LOW  0
#define GEN_RANGE_HIGH  AVDD_DEFAULT

#define GEN_CH1_PIN_STR "A2__" //must be 4 chars
#define GEN_CH2_PIN_STR "D13_" //must be 4 chars

// PWM generator constants =================================================
#ifdef USE_GEN_PWM
#define GEN_PWM_RESOURCES DMA2_R|TIM6_R|TIM7_R|TIM13_R|TIM3_R
	#define GEN_PWM_CH1_PIN							"D5__" // PA6
	#define GEN_PWM_CH2_PIN							"D12_" // PB4

	#define MAX_GEN_PWM_CHANNELS 	2

	#define GEN_PWM_TIM_PERIPH_CLOCK	  (uint32_t) 75000000
#endif //USE_GEN_PWM

// Counter constatnts =======================================================
#ifdef USE_COUNTER
	/* TIM4 -> ARR & PSC set to gate 100 ms */
	#define TIM4_ARR										999
	#define TIM4_PSC										7199
	#define IC_THRESHOLD								20

	/* Instead send for. ex. "HF      " or "LF RF   " */
	#define COUNTER_MODES								"HF LF FR EV"

	/* When porting && less pins -> send "-- " */
	#define CNT_ETR_PIN									"A0 "
	#define CNT_IC_CH1_PIN							"A0 "	// PA0
	#define CNT_IC_CH2_PIN							"A1 "	// PA1
	#define CNT_REF1_PIN								"D7 "	// PA8
	#define CNT_REF2_PIN								"A0 "

	/* Define frequencies to send to PC application to process correct calculations. */
	#define CNT_COUNTER_PERIPH_CLOCK		(uint32_t)   75000000
	#define CNT_GATE_PERIPH_CLOCK			  (uint32_t) 37500000
#endif //USE_COUNTER

// Synchronized PWM generator constants ====================================
#ifdef USE_SYNC_PWM
	#define SYNC_PWM_TIM_PERIPH_CLOCK	  (uint32_t) 37500000
	#define MAX_SYNC_PWM_FREQ						(uint32_t) 100000
	#define MAX_SYNC_PWM_CHANNELS				(uint32_t) 4

	#define SYNC_PWM_CH1_PIN						"PC6_"
	#define SYNC_PWM_CH2_PIN						"PC7_"
	#define SYNC_PWM_CH3_PIN						"PC8_"
	#define SYNC_PWM_CH4_PIN						"PC9_"
#endif //USE_SYNC_PWM

//Definition of assert to check length of strings
#define CASSERT(ex) {char cassert[(ex)?1:-1];cassert[0] = 0;if(cassert[0] == 0){}}
void Error_Handler(void);

/* Definition of ADC and DMA for channel 1 */
//#define ADC_CH_1_CLK_EN() __ADC1_CLK_ENABLE()
//#define ADC_CH_1_CLK_DIS() __ADC1_CLK_DISABLE()
//#define GPIO_ADC_CH_1_CLK_EN() __GPIOC_CLK_ENABLE()
//#define ADC_CH_1  ADC1 //
//#define ADC_GPIO_CH_1  GPIOC
//#define ADC_PIN_CH_1  GPIO_PIN_1
//#define ADC_CHANNEL_CH_1  ADC_CHANNEL_11 //
//#define ADC_DMA_CHANNEL_CH_1  DMA_CHANNEL_0 //
//#define ADC_DMA_STREAM_CH_1  DMA2_Stream0 //

#endif /* STM32F4_CONFIG_H_ */
