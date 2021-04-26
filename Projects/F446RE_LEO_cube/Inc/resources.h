/*
 *****************************************************************************
 * @file    adc_channels.h
 * @author  Y3288231
 * @date    Feb 2021
 * @brief   Hardware abstraction for communication
 *****************************************************************************
 */
#ifndef STM32F4_RESOURCES_H_
#define STM32F4_RESOURCES_H_

#define DMA1_R 0x0001
#define DMA2_R 0x0002

#define ADC123_R 0x0004
//#define ADC34_R 0x0008

#define DAC12_R 0x0010

/*#define OPAMP1_R 0x0020
#define OPAMP2_R 0x0040
#define OPAMP3_R 0x0080
#define OPAMP4_R 0x0100

#define COMP1_R 0x0200
#define COMP2_R 0x0400
#define COMP3_R 0x0800
#define COMP4_R 0x1000
#define COMP5_R 0x2000
#define COMP6_R 0x4000
#define COMP7_R 0x8000*/

#define TIM1_R  0x00001000
#define TIM2_R  0x00002000
#define TIM3_R  0x00004000
#define TIM4_R  0x00008000
#define TIM5_R  0x00010000
#define TIM6_R  0x00020000
#define TIM7_R  0x00040000
#define TIM8_R  0x00080000
#define TIM9_R  0x00100000
#define TIM10_R 0x00200000
#define TIM11_R 0x00400000
#define TIM12_R 0x00800000
#define TIM13_R 0x01000000
#define TIM14_R 0x02000000

#define I2C12_R 0x04000000
#define SPI1_3_R 0x08000000

#define USART12_R 0x10000000
#define USART3_5_R 0x20000000

#define RESERVED1_R 0x40000000
#define RESERVED2_R 0x80000000


#endif /*STM32F4_RESOURCES_H_*/
