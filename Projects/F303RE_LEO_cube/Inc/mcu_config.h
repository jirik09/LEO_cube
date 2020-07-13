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

#include "stm32f3xx_hal.h"
#include "stm32f3xx_nucleo.h"
#include "firmware_version.h"
//#include "usb_device.h"
#include "math.h"
#include "err_list.h"

#define IDN_STRING "Nucleo-F303RE" //max 30 chars
#define SHIELD_STRING " + Shield LEO V0.1"
#define SHIELD_STRING_2 " + Shield LEO V0.2"
#define MCU "STM32F303RE"

#define MCU_UID (uint8_t)*((uint8_t *)0x1FFFF7AC)

// Communication constatnts ===================================================
#define COMM_BUFFER_SIZE 1500
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

#define PA13_GPIO GPIOA
#define PA13_PIN GPIO_PIN_13
#define PA14_GPIO GPIOA
#define PA14_PIN GPIO_PIN_14


// Scope constatnts ===========================================================
#define MAX_SAMPLING_FREQ_12B 4000000 //smps
#define MAX_SAMPLING_FREQ_8B 4800000 //smps
#define MAX_INTERLEAVE_FREQ_12B 6000000 //smps
#define MAX_INTERLEAVE_FREQ_8B 7200000 //smps
#define MAX_ADC_CHANNELS 4
//#define MIN_ADC_SAMPLING_TIME 2 //1.5 + 0.5 cycle from ADC resolution


#define MAX_SCOPE_BUFF_SIZE 30000//40000 //in bytes
#define SCOPE_BUFFER_MARGIN 100

#define SCOPE_CH1_PIN_STR "A5__" //must be 4 chars
#define SCOPE_CH2_PIN_STR "A4__" //must be 4 chars
#define SCOPE_CH3_PIN_STR "A3__" //must be 4 chars
#define SCOPE_CH4_PIN_STR "PB14" //must be 4 chars

#define SCOPE_VREF 3300
#define SCOPE_VREF_INT (uint16_t)*((uint16_t *)0x1FFFF7BA)

#define RANGE_1_LOW 0
#define RANGE_1_HI SCOPE_VREF
#define RANGE_2_LOW -SCOPE_VREF
#define RANGE_2_HI SCOPE_VREF*2
#define RANGE_3_LOW 0
#define RANGE_3_HI SCOPE_VREF*2
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


//scope channels inputs
static const uint8_t ANALOG_DEFAULT_INPUTS[MAX_ADC_CHANNELS]={2,4,2,1}; //{2,4,2,1};
static const uint8_t ANALOG_VREF_INPUTS[MAX_ADC_CHANNELS]={8,9,3,3};


#define ADC1_NUM_CHANNELS 9
static const uint16_t ANALOG_PIN_ADC1[ADC1_NUM_CHANNELS] = {				GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_2,			GPIO_PIN_3,			GPIO_PIN_11,		0,							0};
static GPIO_TypeDef * ANALOG_GPIO_ADC1[ADC1_NUM_CHANNELS] = {				GPIOA,					GPIOA,					GPIOC,					GPIOC,					GPIOC,					GPIOC,					GPIOB,					0,							0};
static const uint32_t ANALOG_CHANNEL_ADC1[ADC1_NUM_CHANNELS] = {		ADC_CHANNEL_1,	ADC_CHANNEL_2,	ADC_CHANNEL_6,	ADC_CHANNEL_7,	ADC_CHANNEL_8,	ADC_CHANNEL_9,	ADC_CHANNEL_14, ADC_CHANNEL_16, ADC_CHANNEL_18};
static const char* ANALOG_CHANN_ADC1_NAME[ADC1_NUM_CHANNELS] = { 		"A0", 			"A1", 			"A5", 			"A4", 			"PC2", 			"PC3", 			"PB11", 		"Temp", 		"Vref" };

#define ADC2_NUM_CHANNELS 10
static const uint16_t ANALOG_PIN_ADC2[ADC2_NUM_CHANNELS] = {				GPIO_PIN_6,			GPIO_PIN_7,			GPIO_PIN_4,			GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_2,			GPIO_PIN_3,			GPIO_PIN_5,			GPIO_PIN_2,			0};
static GPIO_TypeDef * ANALOG_GPIO_ADC2[ADC2_NUM_CHANNELS] = {				GPIOA,					GPIOA,					GPIOC,					GPIOC,					GPIOC,					GPIOC,					GPIOC,					GPIOC,					GPIOB,					0};
static const uint32_t ANALOG_CHANNEL_ADC2[ADC2_NUM_CHANNELS] = {		ADC_CHANNEL_3,	ADC_CHANNEL_4,	ADC_CHANNEL_5,	ADC_CHANNEL_6,	ADC_CHANNEL_7,	ADC_CHANNEL_8,	ADC_CHANNEL_9,	ADC_CHANNEL_11,	ADC_CHANNEL_12,	ADC_CHANNEL_18};
static const char* ANALOG_CHANN_ADC2_NAME[ADC2_NUM_CHANNELS] = { 		"D12", 			"D11", 			"PC4", 			"A5", 			"A4", 			"PC2", 			"PC3", 			"PC5", 			"PB2", 			"Vref"};

#define ADC3_NUM_CHANNELS 4
static const uint16_t ANALOG_PIN_ADC3[ADC3_NUM_CHANNELS] = {				GPIO_PIN_1,			GPIO_PIN_13,		GPIO_PIN_0,			0};
static GPIO_TypeDef * ANALOG_GPIO_ADC3[ADC3_NUM_CHANNELS] = {				GPIOB,					GPIOB,					GPIOB,					0};
static const uint32_t ANALOG_CHANNEL_ADC3[ADC3_NUM_CHANNELS] = {		ADC_CHANNEL_1,	ADC_CHANNEL_5,	ADC_CHANNEL_12,	ADC_CHANNEL_18};
static const char* ANALOG_CHANN_ADC3_NAME[ADC3_NUM_CHANNELS] = { 		"PB1", 					"PB13", 				"A3", 					"Vref" };

#define ADC4_NUM_CHANNELS 4
static const uint16_t ANALOG_PIN_ADC4[ADC4_NUM_CHANNELS] = {				GPIO_PIN_13,		GPIO_PIN_14,		GPIO_PIN_15,		0};
static GPIO_TypeDef * ANALOG_GPIO_ADC4[ADC4_NUM_CHANNELS] = {				GPIOB,					GPIOB,					GPIOB,					0};
static const uint32_t ANALOG_CHANNEL_ADC4[ADC4_NUM_CHANNELS] = {		ADC_CHANNEL_3,	ADC_CHANNEL_4,	ADC_CHANNEL_5,	ADC_CHANNEL_18};
static const char* ANALOG_CHANN_ADC4_NAME[ADC4_NUM_CHANNELS] = { 		"PB13", 				"PB14", 				"PB15", 				"Vref" };


static const uint8_t NUM_OF_ANALOG_INPUTS[MAX_ADC_CHANNELS]={ADC1_NUM_CHANNELS,ADC2_NUM_CHANNELS,ADC3_NUM_CHANNELS,ADC4_NUM_CHANNELS}; //number of ADC channels {ADC1,ADC2,ADC3,ADC4}
// Generator constatnts =======================================================

#define MAX_GENERATING_FREQ 2000000 //smps
#define MAX_DAC_CHANNELS 2
#define MAX_GENERATOR_BUFF_SIZE 8000//2000
#define	DAC_DATA_DEPTH 12

#define GEN_VREF 3300
#define GEN_VDDA 3300
#define GEN_VREF_INT 1210

#ifdef USE_SHIELD
#define SHIELD_GEN_HIGH   5000 //without calibration 5000  4898-5
#define SHIELD_GEN_LOW   -5000 //without calibration -5000  -4898-5
#endif

#define GEN_CH1_PIN_STR "A2__" //must be 4 chars
#define GEN_CH2_PIN_STR "D13_" //must be 4 chars

// Counter constatnts =======================================================
#ifdef USE_COUNTER
/* TIM4 -> ARR & PSC set to gate 100 ms */
#define TIM4_ARR										999
#define TIM4_PSC										7199
#define IC_THRESHOLD								20

/* Counter Modes supported */
#define COUNTER_MODE_HF								"HF__"
#define COUNTER_MODE_LF								"LF__"
#define COUNTER_MODE_FR								"FR__"
#define COUNTER_MODE_TI								"TI__"

/* When porting && less pins -> send "-- " */
#define CNT_ETR_PIN								"A0__"
#define CNT_IC_CH1_PIN							"A0__"	// PA0
#define CNT_IC_CH2_PIN							"A1__"	// PA1
#define CNT_REF1_PIN							"D7__"	// PA8
#define CNT_REF2_PIN							"A0__"
#define CNT_TI_CH1_PIN							"A0__"	// PA0
#define CNT_TI_CH2_PIN							"A1__"	// PA1

/* Define frequencies to send to PC application to process correct calculations. */
#define CNT_COUNTER_PERIPH_CLOCK		(uint32_t) 144000000
#define CNT_GATE_PERIPH_CLOCK			  (uint32_t) 72000000
#endif //USE_COUNTER

// PWM generator constants =================================================
#ifdef USE_GEN_PWM
#define GEN_PWM_CH1_PIN							"D8__" // PA9
#define GEN_PWM_CH2_PIN							"D5__" // PB4

#define MAX_GEN_PWM_CHANNELS 	2

#define GEN_PWM_CH1_TIM_PERIPH_CLOCK	  (uint32_t) 144000000
#define GEN_PWM_CH2_TIM_PERIPH_CLOCK	  (uint32_t) 72000000
#endif //USE_GEN_PWM

// Synchronized PWM generator constants ====================================
#ifdef USE_SYNC_PWM
#define SYNC_PWM_TIM_PERIPH_CLOCK	  (uint32_t) 72000000
#define MAX_SYNC_PWM_FREQ						(uint32_t) 100000
#define MAX_SYNC_PWM_CHANNELS				(uint32_t) 4

#define SYNC_PWM_CH1_PIN						"PC6_"
#define SYNC_PWM_CH2_PIN						"PC7_"
#define SYNC_PWM_CH3_PIN						"PC8_"
#define SYNC_PWM_CH4_PIN						"PC9_"
#endif //USE_SYNC_PWM

// Logic Analyzer constants ====================================
#ifdef USE_LOG_ANLYS
/* Nucleo F303RE - TIM4 (posttrigger), TIM1 (timebase) */
#define LOG_ANLYS_POSTTRIG_PERIPH_CLOCK	   (uint32_t) 72000000
#define LOG_ANLYS_TIMEBASE_PERIPH_CLOCK		 (uint32_t) 144000000
/* Sampling frequency depends on DMA transfer possibilities. Half of APB clock freq. */
#define LOG_ANLYS_SAMPLING_FREQ						 (uint32_t) (72000000 / 2)
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

//Definition of assert to check length of strings
#define CASSERT(ex) {typedef char cassert_type[(ex)?1:-1];}


#endif /* STM32F4_CONFIG_H_ */
