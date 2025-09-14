/*
 * adc_channels.h - STM32G474 ADC channel mapping (extern declarations)
 */
#ifndef STM32G4_ADC_CHANNELS_H_
#define STM32G4_ADC_CHANNELS_H_

#include <stdint.h>
#include "stm32g4xx_hal.h"
#include "mcu_config.h" /* MAX_ADC_CHANNELS */

/* Per-ADC channel counts (mirrors mcu_config original values) */
#define ADC1_NUM_CHANNELS 9
#define ADC2_NUM_CHANNELS 8
#define ADC3_NUM_CHANNELS 4
#define ADC4_NUM_CHANNELS 4

/* Default / Vref channel indices */
extern const uint8_t ANALOG_DEFAULT_INPUTS[MAX_ADC_CHANNELS];
extern const uint8_t ANALOG_VREF_INPUTS[MAX_ADC_CHANNELS];

/* ADC1 */
extern const uint16_t       ANALOG_PIN_ADC1[ADC1_NUM_CHANNELS];
extern GPIO_TypeDef * const ANALOG_GPIO_ADC1[ADC1_NUM_CHANNELS];
extern const uint32_t       ANALOG_CHANNEL_ADC1[ADC1_NUM_CHANNELS];
extern const char * const   ANALOG_CHANN_ADC1_NAME[ADC1_NUM_CHANNELS];
/* ADC2 */
extern const uint16_t       ANALOG_PIN_ADC2[ADC2_NUM_CHANNELS];
extern GPIO_TypeDef * const ANALOG_GPIO_ADC2[ADC2_NUM_CHANNELS];
extern const uint32_t       ANALOG_CHANNEL_ADC2[ADC2_NUM_CHANNELS];
extern const char * const   ANALOG_CHANN_ADC2_NAME[ADC2_NUM_CHANNELS];
/* ADC3 */
extern const uint16_t       ANALOG_PIN_ADC3[ADC3_NUM_CHANNELS];
extern GPIO_TypeDef * const ANALOG_GPIO_ADC3[ADC3_NUM_CHANNELS];
extern const uint32_t       ANALOG_CHANNEL_ADC3[ADC3_NUM_CHANNELS];
extern const char * const   ANALOG_CHANN_ADC3_NAME[ADC3_NUM_CHANNELS];
/* ADC4 */
extern const uint16_t       ANALOG_PIN_ADC4[ADC4_NUM_CHANNELS];
extern GPIO_TypeDef * const ANALOG_GPIO_ADC4[ADC4_NUM_CHANNELS];
extern const uint32_t       ANALOG_CHANNEL_ADC4[ADC4_NUM_CHANNELS];
extern const char * const   ANALOG_CHANN_ADC4_NAME[ADC4_NUM_CHANNELS];

/* Counts summary */
extern const uint8_t NUM_OF_ANALOG_INPUTS[MAX_ADC_CHANNELS];

#endif /* STM32G4_ADC_CHANNELS_H_ */
