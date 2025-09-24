/*
 *****************************************************************************
 * @file    adc_channels.h
 * @author  Y3288231
 * @date    Feb 2021
 * @brief   Hardware abstraction for communication
 *****************************************************************************
 */

/** Peripheral resources */
#ifndef STM32F3_RESOURCES_H_
#define STM32F3_RESOURCES_H_

#define DMA1_R 0x0001
#define DMA2_R 0x0002

#define ADC12_R 0x0004
#define ADC34_R 0x0008

#define DAC12_R 0x0010

#define OPAMP1_R 0x0020
#define OPAMP2_R 0x0040
#define OPAMP3_R 0x0080
#define OPAMP4_R 0x0100

#define COMP1_R 0x0200
#define COMP2_R 0x0400
#define COMP3_R 0x0800
#define COMP4_R 0x1000
#define COMP5_R 0x2000
#define COMP6_R 0x4000
#define COMP7_R 0x8000

#define TIM1_R 0x00010000
#define TIM2_R 0x00020000
#define TIM3_R 0x00040000
#define TIM4_R 0x00080000
#define TIM6_R 0x00100000
#define TIM7_R 0x00200000
#define TIM8_R 0x00400000
#define TIM15_R 0x00800000
#define TIM16_R 0x01000000
#define TIM17_R 0x02000000

#define I2C12_R 0x04000000
#define SPI1_3_R 0x08000000

#define USART12_R 0x10000000
#define USART3_5_R 0x20000000

#define RESERVED1_R 0x40000000
#define RESERVED2_R 0x80000000

/** Pin index definitions (0..15) */
typedef enum {
	PIN0  = 0,
	PIN1  = 1,
	PIN2  = 2,
	PIN3  = 3,
	PIN4  = 4,
	PIN5  = 5,
	PIN6  = 6,
	PIN7  = 7,
	PIN8  = 8,
	PIN9  = 9,
	PIN10 = 10,
	PIN11 = 11,
	PIN12 = 12,
	PIN13 = 13,
	PIN14 = 14,
	PIN15 = 15
} gpio_pin_index_t;

/* Helper to form a single-bit mask from a pin index */
#ifndef BIT
#define BIT(n) ( (uint32_t) (1U << (n)) )
#endif

/* Aggregated GPIO port masks (A..D). Extend if more ports are needed. */
typedef struct {
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
} gpio_port_mask_t;

#ifndef GPIO_MASKS
#define GPIO_MASKS(ma, mb, mc, md) (gpio_port_mask_t){ (ma), (mb), (mc), (md) }
#endif

#endif /*STM32F3_RESOURCES_H_*/
