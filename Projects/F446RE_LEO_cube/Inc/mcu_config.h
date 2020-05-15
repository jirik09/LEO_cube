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
//#include "usb_device.h"
#include "math.h"
#include "err_list.h"

#define IDN_STRING "STM32F446-Nucleo" //max 30 chars
#define MCU "STM32F446RE"

// Communication constatnts ===================================================
#define COMM_BUFFER_SIZE 512
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
#define MAX_SAMPLING_FREQ_12B   2400000 //smps
#define MAX_SAMPLING_FREQ_8B    3200000 //smps
#define MAX_INTERLEAVE_FREQ_12B 4800000 //smps
#define MAX_INTERLEAVE_FREQ_8B  6400000 //smps
#define MAX_ADC_CHANNELS 3

#define MAX_SCOPE_BUFF_SIZE 60000 //in bytes
#define SCOPE_BUFFER_MARGIN 200

#define SCOPE_CH1_PIN_STR "A5__" //must be 4 chars
#define SCOPE_CH2_PIN_STR "A4__" //must be 4 chars
#define SCOPE_CH3_PIN_STR "A3__" //must be 4 chars
#define SCOPE_CH4_PIN_STR "----" //must be 4 chars

#define SCOPE_VREF 3300
#define SCOPE_VREF_INT (uint16_t)*((uint16_t *)0x1FFF7A2A)

#define RANGE_1_LOW 0
#define RANGE_1_HI SCOPE_VREF
#define RANGE_2_LOW -SCOPE_VREF
#define RANGE_2_HI SCOPE_VREF*2
#define RANGE_3_LOW 0
#define RANGE_3_HI 0
#define RANGE_4_LOW 0
#define RANGE_4_HI 0

//scope channels inputs
static const uint8_t ANALOG_DEFAULT_INPUTS[MAX_ADC_CHANNELS]={0,0,0};
static const uint8_t ANALOG_VREF_INPUTS[MAX_ADC_CHANNELS]={6,2,1};

#define ADC1_NUM_CHANNELS 7
static const uint16_t ANALOG_PIN_ADC1[ADC1_NUM_CHANNELS] = {			GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_0,			0,					0};
static GPIO_TypeDef * ANALOG_GPIO_ADC1[ADC1_NUM_CHANNELS] = {			GPIOB,				GPIOC,				GPIOC,				GPIOA,				GPIOA,				0,					0};
static const uint32_t ANALOG_CHANNEL_ADC1[ADC1_NUM_CHANNELS] = {		ADC_CHANNEL_8,		ADC_CHANNEL_11,		ADC_CHANNEL_10,		ADC_CHANNEL_1,		ADC_CHANNEL_0,	 	ADC_CHANNEL_16, 	ADC_CHANNEL_17};
static const char* ANALOG_CHANN_ADC1_NAME[ADC1_NUM_CHANNELS] = { 		"A3", 				"A4", 				"A5", 				"A1", 				"A0", 				"Temp", 			"Vref" };

#define ADC2_NUM_CHANNELS 3
static const uint16_t ANALOG_PIN_ADC2[ADC2_NUM_CHANNELS] = {			GPIO_PIN_1,			GPIO_PIN_0,			0};
static GPIO_TypeDef * ANALOG_GPIO_ADC2[ADC2_NUM_CHANNELS] = {			GPIOC,				GPIOC,				0};
static const uint32_t ANALOG_CHANNEL_ADC2[ADC2_NUM_CHANNELS] = {		ADC_CHANNEL_11,		ADC_CHANNEL_10,		ADC_CHANNEL_17};
static const char* ANALOG_CHANN_ADC2_NAME[ADC2_NUM_CHANNELS] = { 		"A4", 				"A5", 				"Vref" };

#define ADC3_NUM_CHANNELS 2
static const uint16_t ANALOG_PIN_ADC3[ADC3_NUM_CHANNELS] = {			GPIO_PIN_0,			0};
static GPIO_TypeDef * ANALOG_GPIO_ADC3[ADC3_NUM_CHANNELS] = {			GPIOC,				0};
static const uint32_t ANALOG_CHANNEL_ADC3[ADC3_NUM_CHANNELS] = {		ADC_CHANNEL_10,		ADC_CHANNEL_17};
static const char* ANALOG_CHANN_ADC3_NAME[ADC3_NUM_CHANNELS] = { 		"A5", 				"Vref" };

#define ADC4_NUM_CHANNELS 1
static const uint16_t ANALOG_PIN_ADC4[ADC4_NUM_CHANNELS] = {			0};
static GPIO_TypeDef * ANALOG_GPIO_ADC4[ADC4_NUM_CHANNELS] = {			0};
static const uint32_t ANALOG_CHANNEL_ADC4[ADC4_NUM_CHANNELS] = {		0};
static const char* ANALOG_CHANN_ADC4_NAME[ADC4_NUM_CHANNELS] = { 		"NA" };



static const uint8_t NUM_OF_ANALOG_INPUTS[MAX_ADC_CHANNELS]={ADC1_NUM_CHANNELS,ADC2_NUM_CHANNELS,ADC3_NUM_CHANNELS}; //number of ADC channels {ADC1,ADC2,ADC3,ADC4}


// Generator constatnts ===================================================

#define MAX_GENERATING_FREQ 2000000 //smps
#define MAX_DAC_CHANNELS 2
#define MAX_GENERATOR_BUFF_SIZE 2000
#define	DAC_DATA_DEPTH 12

#define GEN_VDDA 3300
#define GEN_VREF 3300
#define GEN_VREF_INT 1200

#define GEN_CH1_PIN_STR "A2__" //must be 4 chars
#define GEN_CH2_PIN_STR "D13_" //must be 4 chars

// PWM generator constants =================================================
#ifdef USE_GEN_PWM
	#define GEN_PWM_CH1_PIN							"D5__" // PA6
	#define GEN_PWM_CH2_PIN							"D12_" // PB4

	#define MAX_GEN_PWM_CHANNELS 	2

	#define GEN_PWM_CH1_TIM_PERIPH_CLOCK	  (uint32_t) 150000000
	#define GEN_PWM_CH2_TIM_PERIPH_CLOCK	  (uint32_t) 75000000
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
#define CASSERT(ex) {typedef char cassert_type[(ex)?1:-1];}


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
