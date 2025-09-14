/*
 *****************************************************************************
 * @file    adc_channels.h
 * @author  Y3288231
 * @date    Feb 2021
 * @brief   ADC channels, pins and names definition (header now holds only
 *          constants/macros and extern declarations – data lives in
 *          adc_channels.c to avoid multiple definitions and shrink build time)
 *****************************************************************************
 */
#ifndef STM32F3_ADC_CHANNELS_H_
#define STM32F3_ADC_CHANNELS_H_

#include <stdint.h>
#include "mcu_config.h"      /* For MAX_ADC_CHANNELS and GPIO pin macros */
#include "stm32f3xx_hal.h"   /* For GPIOx symbols & ADC_CHANNEL_x */

/* Helper macros mapping GPIO port symbolic values (cast for table storage) */
#define GPIOA_ADC               ((uint32_t)GPIOA)
#define GPIOB_ADC               ((uint32_t)GPIOB)
#define GPIOC_ADC               ((uint32_t)GPIOC)
#define GPIOD_ADC               ((uint32_t)GPIOD)

/* Scope default & reference input index tables (used at runtime) */
extern const uint8_t ANALOG_DEFAULT_INPUTS[MAX_ADC_CHANNELS];
extern const uint8_t ANALOG_VREF_INPUTS[MAX_ADC_CHANNELS];

/* ADC channel counts */
#define ADC1_NUM_CHANNELS 9
#define ADC2_NUM_CHANNELS 10
#define ADC3_NUM_CHANNELS 4
#define ADC4_NUM_CHANNELS 4

/* Per-ADC pin / GPIO / ADC-channel lookup tables */
extern const uint16_t ANALOG_PIN_ADC1[ADC1_NUM_CHANNELS];
extern const uint32_t ANALOG_GPIO_ADC1[ADC1_NUM_CHANNELS];
extern const uint32_t ANALOG_CHANNEL_ADC1[ADC1_NUM_CHANNELS];
extern const char*   ANALOG_CHANN_ADC1_NAME[ADC1_NUM_CHANNELS];

extern const uint16_t ANALOG_PIN_ADC2[ADC2_NUM_CHANNELS];
extern const uint32_t ANALOG_GPIO_ADC2[ADC2_NUM_CHANNELS];
extern const uint32_t ANALOG_CHANNEL_ADC2[ADC2_NUM_CHANNELS];
extern const char*   ANALOG_CHANN_ADC2_NAME[ADC2_NUM_CHANNELS];

extern const uint16_t ANALOG_PIN_ADC3[ADC3_NUM_CHANNELS];
extern const uint32_t ANALOG_GPIO_ADC3[ADC3_NUM_CHANNELS];
extern const uint32_t ANALOG_CHANNEL_ADC3[ADC3_NUM_CHANNELS];
extern const char*   ANALOG_CHANN_ADC3_NAME[ADC3_NUM_CHANNELS];

extern const uint16_t ANALOG_PIN_ADC4[ADC4_NUM_CHANNELS];
extern const uint32_t ANALOG_GPIO_ADC4[ADC4_NUM_CHANNELS];
extern const uint32_t ANALOG_CHANNEL_ADC4[ADC4_NUM_CHANNELS];
extern const char*   ANALOG_CHANN_ADC4_NAME[ADC4_NUM_CHANNELS];

/* Number of analog inputs on each ADC */
extern const uint8_t NUM_OF_ANALOG_INPUTS[MAX_ADC_CHANNELS];

#endif /*STM32F3_ADC_CHANNELS_H_*/
