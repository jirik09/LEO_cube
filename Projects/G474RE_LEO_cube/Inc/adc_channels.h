/*
 *****************************************************************************
 * @file    adc_channels.h
 *****************************************************************************
 */
#ifndef STM32G4_ADC_CHANNELS_H_
#define STM32G4_ADC_CHANNELS_H_

#define GPIOA_ADC               ((uint32_t) GPIOA)
#define GPIOB_ADC               ((uint32_t) GPIOB)
#define GPIOC_ADC               ((uint32_t) GPIOC)
#define GPIOD_ADC               ((uint32_t) GPIOD)


//scope channels inputs
static const uint8_t ANALOG_DEFAULT_INPUTS[MAX_ADC_CHANNELS]={0,0,0,0}; //{2,4,2,1};
static const uint8_t ANALOG_VREF_INPUTS[MAX_ADC_CHANNELS]={8,7,3,3};


#define ADC1_NUM_CHANNELS 9
static const uint16_t ANALOG_PIN_ADC1[ADC1_NUM_CHANNELS] = {				GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_2,			GPIO_PIN_3,			GPIO_PIN_2,			GPIO_PIN_3,			GPIO_PIN_11,		0,						0};
static GPIO_TypeDef * ANALOG_GPIO_ADC1[ADC1_NUM_CHANNELS] = {				GPIOC,				GPIOC,				GPIOC,				GPIOC,				GPIOA,				GPIOA,				GPIOB,				0,						0};
static const uint32_t ANALOG_CHANNEL_ADC1[ADC1_NUM_CHANNELS] = {			ADC_CHANNEL_6,		ADC_CHANNEL_7,		ADC_CHANNEL_8,		ADC_CHANNEL_9,		ADC_CHANNEL_3,		ADC_CHANNEL_4,		ADC_CHANNEL_14, 	ADC_CHANNEL_16, 		ADC_CHANNEL_VREFINT};
static const char* ANALOG_CHANN_ADC1_NAME[ADC1_NUM_CHANNELS] = { 			"A5", 				"A4", 				"PC2", 				"PC3", 				"PA2", 				"PA3", 				"PB11", 			"Temp", 				"Vref" };

#define ADC2_NUM_CHANNELS 8
static const uint16_t ANALOG_PIN_ADC2[ADC2_NUM_CHANNELS] = {				GPIO_PIN_1,			GPIO_PIN_0,			GPIO_PIN_6,			GPIO_PIN_2,			GPIO_PIN_3,			GPIO_PIN_2,			GPIO_PIN_15,			0};
static GPIO_TypeDef * ANALOG_GPIO_ADC2[ADC2_NUM_CHANNELS] = {				GPIOC,				GPIOC,				GPIOA,				GPIOC,				GPIOC,				GPIOB,				GPIOB,					0};
static const uint32_t ANALOG_CHANNEL_ADC2[ADC2_NUM_CHANNELS] = {			ADC_CHANNEL_7,		ADC_CHANNEL_6,		ADC_CHANNEL_3,		ADC_CHANNEL_8,		ADC_CHANNEL_9,		ADC_CHANNEL_12,		ADC_CHANNEL_15,			ADC_CHANNEL_17};
static const char* ANALOG_CHANN_ADC2_NAME[ADC2_NUM_CHANNELS] = { 			"A4", 				"A5", 				"D12",				"PC2", 				"PC3", 				"PB2", 				"PB15", 				"Vref"};

#define ADC3_NUM_CHANNELS 4
static const uint16_t ANALOG_PIN_ADC3[ADC3_NUM_CHANNELS] = {				GPIO_PIN_0,			GPIO_PIN_1,			GPIO_PIN_13,		0};
static GPIO_TypeDef * ANALOG_GPIO_ADC3[ADC3_NUM_CHANNELS] = {				GPIOB,				GPIOB,				GPIOB,				0};
static const uint32_t ANALOG_CHANNEL_ADC3[ADC3_NUM_CHANNELS] = {			ADC_CHANNEL_12,		ADC_CHANNEL_1,		ADC_CHANNEL_5,		ADC_CHANNEL_VREFINT};
static const char* ANALOG_CHANN_ADC3_NAME[ADC3_NUM_CHANNELS] = { 			"A3",				"PB1", 				"PB13", 			"Vref" };

#define ADC4_NUM_CHANNELS 4
static const uint16_t ANALOG_PIN_ADC4[ADC4_NUM_CHANNELS] = {				GPIO_PIN_14,		GPIO_PIN_12,		GPIO_PIN_15,		0};
static GPIO_TypeDef * ANALOG_GPIO_ADC4[ADC4_NUM_CHANNELS] = {				GPIOB,				GPIOB,				GPIOB,				0};
static const uint32_t ANALOG_CHANNEL_ADC4[ADC4_NUM_CHANNELS] = {			ADC_CHANNEL_4,		ADC_CHANNEL_3,		ADC_CHANNEL_5,		ADC_CHANNEL_VREFINT};
static const char* ANALOG_CHANN_ADC4_NAME[ADC4_NUM_CHANNELS] = { 			"PB14", 			"PB12", 			"PB15", 			"Vref" };


static const uint8_t NUM_OF_ANALOG_INPUTS[MAX_ADC_CHANNELS]={ADC1_NUM_CHANNELS,ADC2_NUM_CHANNELS,ADC3_NUM_CHANNELS,ADC4_NUM_CHANNELS}; //number of ADC channels {ADC1,ADC2,ADC3,ADC4}


#endif /*STM32G4_ADC_CHANNELS_H_*/
