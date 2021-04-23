/*
 *****************************************************************************
 * @file    adc_channels.h
 * @author  Y3288231
 * @date    Feb 2021
 * @brief   Hardware abstraction for communication
 *****************************************************************************
 */
#ifndef STM32F4_ADC_CHANNELS_H_
#define STM32F4_ADC_CHANNELS_H_

#define GPIOA_ADC               ((uint32_t) GPIOA)
#define GPIOB_ADC               ((uint32_t) GPIOB)
#define GPIOC_ADC               ((uint32_t) GPIOC)
#define GPIOD_ADC               ((uint32_t) GPIOD)


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


#endif /*STM32F4_ADC_CHANNELS_H_*/
