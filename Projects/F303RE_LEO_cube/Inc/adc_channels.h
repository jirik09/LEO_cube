/*
 *****************************************************************************
 * @file    adc_channels.h
 * @author  Y3288231
 * @date    Feb 2021
 * @brief   Hardware abstraction for communication
 *****************************************************************************
 */
#ifndef STM32F3_ADC_CHANNELS_H_
#define STM32F3_ADC_CHANNELS_H_

#define GPIOA_ADC               ((uint32_t) GPIOA)
#define GPIOB_ADC               ((uint32_t) GPIOB)
#define GPIOC_ADC               ((uint32_t) GPIOC)
#define GPIOD_ADC               ((uint32_t) GPIOD)


//scope channels inputs
static const uint8_t ANALOG_DEFAULT_INPUTS[MAX_ADC_CHANNELS]={2,4,2,1}; //{2,4,2,1};
static const uint8_t ANALOG_VREF_INPUTS[MAX_ADC_CHANNELS]={8,9,3,3};


#define ADC1_NUM_CHANNELS 9
static const uint16_t ANALOG_PIN_ADC1[ADC1_NUM_CHANNELS] = {				GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_2,			GPIO_PIN_3,			GPIO_PIN_11,		0,					0};
static const uint32_t ANALOG_GPIO_ADC1[ADC1_NUM_CHANNELS] = {				GPIOA_ADC,			GPIOA_ADC,			GPIOC_ADC,			GPIOC_ADC,			GPIOC_ADC,			GPIOC_ADC,			GPIOB_ADC,			0,					0};
static const uint32_t ANALOG_CHANNEL_ADC1[ADC1_NUM_CHANNELS] = {			ADC_CHANNEL_1,		ADC_CHANNEL_2,		ADC_CHANNEL_6,		ADC_CHANNEL_7,		ADC_CHANNEL_8,		ADC_CHANNEL_9,		ADC_CHANNEL_14, 	ADC_CHANNEL_16, 	ADC_CHANNEL_18};
static const char* ANALOG_CHANN_ADC1_NAME[ADC1_NUM_CHANNELS] = { 			"A0", 				"A1", 				"A5", 				"A4", 				"PC2", 				"PC3", 				"PB11", 			"Temp", 			"Vref" };

#define ADC2_NUM_CHANNELS 10
static const uint16_t ANALOG_PIN_ADC2[ADC2_NUM_CHANNELS] = {				GPIO_PIN_6,			GPIO_PIN_7,			GPIO_PIN_4,			GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_2,			GPIO_PIN_3,			GPIO_PIN_5,			GPIO_PIN_2,			0};
static const uint32_t ANALOG_GPIO_ADC2[ADC2_NUM_CHANNELS] = {				GPIOA_ADC,			GPIOA_ADC,			GPIOC_ADC,			GPIOC_ADC,			GPIOC_ADC,			GPIOC_ADC,			GPIOC_ADC,			GPIOC_ADC,			GPIOB_ADC,			0};
static const uint32_t ANALOG_CHANNEL_ADC2[ADC2_NUM_CHANNELS] = {			ADC_CHANNEL_3,		ADC_CHANNEL_4,		ADC_CHANNEL_5,		ADC_CHANNEL_6,		ADC_CHANNEL_7,		ADC_CHANNEL_8,		ADC_CHANNEL_9,		ADC_CHANNEL_11,		ADC_CHANNEL_12,		ADC_CHANNEL_18};
static const char* ANALOG_CHANN_ADC2_NAME[ADC2_NUM_CHANNELS] = { 			"D12", 				"D11", 				"PC4", 				"A5", 				"A4", 				"PC2", 				"PC3", 				"PC5", 			"	PB2", 				"Vref"};

#define ADC3_NUM_CHANNELS 4
static const uint16_t ANALOG_PIN_ADC3[ADC3_NUM_CHANNELS] = {				GPIO_PIN_1,			GPIO_PIN_13,		GPIO_PIN_0,			0};
static const uint32_t ANALOG_GPIO_ADC3[ADC3_NUM_CHANNELS] = {				GPIOB_ADC,			GPIOB_ADC,			GPIOB_ADC,			0};
static const uint32_t ANALOG_CHANNEL_ADC3[ADC3_NUM_CHANNELS] = {			ADC_CHANNEL_1,		ADC_CHANNEL_5,		ADC_CHANNEL_12,		ADC_CHANNEL_18};
static const char* ANALOG_CHANN_ADC3_NAME[ADC3_NUM_CHANNELS] = { 			"PB1", 				"PB13", 			"A3", 				"Vref" };

#define ADC4_NUM_CHANNELS 4
static const uint16_t ANALOG_PIN_ADC4[ADC4_NUM_CHANNELS] = {				GPIO_PIN_13,		GPIO_PIN_14,		GPIO_PIN_15,		0};
static const uint32_t ANALOG_GPIO_ADC4[ADC4_NUM_CHANNELS] = {				GPIOB_ADC,			GPIOB_ADC,			GPIOB_ADC,			0};
static const uint32_t ANALOG_CHANNEL_ADC4[ADC4_NUM_CHANNELS] = {			ADC_CHANNEL_3,		ADC_CHANNEL_4,		ADC_CHANNEL_5,		ADC_CHANNEL_18};
static const char* ANALOG_CHANN_ADC4_NAME[ADC4_NUM_CHANNELS] = { 			"PB13", 			"PB14", 			"PB15", 			"Vref" };


static const uint8_t NUM_OF_ANALOG_INPUTS[MAX_ADC_CHANNELS]={ADC1_NUM_CHANNELS,ADC2_NUM_CHANNELS,ADC3_NUM_CHANNELS,ADC4_NUM_CHANNELS}; //number of ADC channels {ADC1,ADC2,ADC3,ADC4}


#endif /*STM32F3_ADC_CHANNELS_H_*/
