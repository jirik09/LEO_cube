/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_it.h"
#include "cmsis_os.h"
#include "comms_hal.h"
#include "counter.h"
#include "tim.h"
#include "logic_analyzer.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;
extern DMA_HandleTypeDef hdma_adc4;
extern DMA_HandleTypeDef hdma_dac1_ch1;
extern DMA_HandleTypeDef hdma_dac1_ch2;
extern TIM_HandleTypeDef htim20;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc2);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel3 global interrupt.
  */
void DMA1_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel3_IRQn 0 */

  /* USER CODE END DMA1_Channel3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc3);
  /* USER CODE BEGIN DMA1_Channel3_IRQn 1 */

  /* USER CODE END DMA1_Channel3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

  /* USER CODE END DMA1_Channel4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc4);
  /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

  /* USER CODE END DMA1_Channel4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA2_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

  /* USER CODE END DMA1_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_dac1_ch1);
  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel6 global interrupt.
  */
void DMA2_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel6_IRQn 0 */

  /* USER CODE END DMA1_Channel6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_dac1_ch2);
  /* USER CODE BEGIN DMA1_Channel6_IRQn 1 */

  /* USER CODE END DMA1_Channel6_IRQn 1 */
}

/**
  * @brief This function handles USB low priority interrupt remap.
  */
void USB_LP_IRQHandler(void)
{
  /* USER CODE BEGIN USB_LP_IRQn 0 */

  /* USER CODE END USB_LP_IRQn 0 */
 // HAL_PCD_IRQHandler(&hpcd_USB_FS);
  /* USER CODE BEGIN USB_LP_IRQn 1 */

  /* USER CODE END USB_LP_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
//void EXTI15_10_IRQHandler(void)
//{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
//  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
//}

/**
  * @brief This function handles TIM20 update interrupt.
  */
void TIM20_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM20_UP_IRQn 0 */

  /* USER CODE END TIM20_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim20);
  /* USER CODE BEGIN TIM20_UP_IRQn 1 */

  /* USER CODE END TIM20_UP_IRQn 1 */
}

/* USER CODE BEGIN 1 */
/**
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	char c = (uint16_t)(USART2->RDR & (uint16_t)0x01FF);
	commsRecieveUART(c);
	HAL_NVIC_ClearPendingIRQ(USART2_IRQn);
  /* USER CODE END USART2_IRQn 0 */

  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

#ifdef USE_COUNTER
/**
* @brief This function handles DMA1 channel2 global interrupt.
*/
void DMA1_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_tim2_up);
}
#endif //USE_COUNTER


#ifdef USE_LOG_ANLYS
/**
* @brief This function handles DMA1 channel7 global interrupt for Logic Analyzer.
*	Saves DMA CNDTR pointer state to find out later where the trigger occured.
* NOT USED ANYMORE !!!
*/
void DMA1_Channel7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_tim4_up);
	//HAL_DMA_IRQHandler(&hdma_usart2_tx);
	//huart2.gState =HAL_UART_STATE_READY;
//	  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

extern DMA_HandleTypeDef hdma_tim1_up;

/**
  * @brief  Logic Analyzer ISR triggered by an incoming signal edge on GPIO.
	*					Stores time of trigger and starts posttrigger time (TIM4) in order to sample the rest.
  */
void EXTI15_10_IRQHandler(void){
	TIM_PostTrigger_SoftwareStart();
	logAnlys.triggerPointer = hdma_tim1_up.Instance->CNDTR;
	LOG_ANLYS_handle_interrupt(EXTI->PR1 & 0x3fc0);
}

/**
  * @brief  Logic Analyzer ISR triggered by an incoming signal edge on GPIO.
	*					Stores time of trigger and starts posttrigger time (TIM4) in order to sample the rest.
  */
void EXTI9_5_IRQHandler(void){
	TIM_PostTrigger_SoftwareStart();
	logAnlys.triggerPointer = hdma_tim1_up.Instance->CNDTR;
	LOG_ANLYS_handle_interrupt(EXTI->PR1 & 0x3fc0); //mask the pending requests to get interrupts from selected pins only
}

/**
  * @brief  Logic Analyzer callback called from EXTI9_5_IRQHandler or EXTI15_10_IRQHandler.
	*					Distinguishes the source of the trigger.
  */
void LOG_ANLYS_handle_interrupt(uint32_t pr){
	uint8_t isRightPin = 0;

	if(pr & (1 << 6) ){ 	//pending request on pin 6
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
		if(logAnlys.trigConfig == TRIG_CHAN1){
			HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			isRightPin = 1;
		}
	}
	if(pr & (1 << 7) ){ 	//pending request on pin 7
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
		if(logAnlys.trigConfig == TRIG_CHAN2){
			HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			isRightPin = 1;
		}
	}
	if(pr & (1 << 8) ){ 	//pending request on pin 8
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
		if(logAnlys.trigConfig == TRIG_CHAN3){
			HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			isRightPin = 1;
		}
	}
	if(pr & (1 << 9) ){ 	//pending request on pin 9
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
		if(logAnlys.trigConfig == TRIG_CHAN4){
			HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			isRightPin = 1;
		}
	}
	if(pr & (1 << 10) ){ 	//pending request on pin 10
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
		if(logAnlys.trigConfig == TRIG_CHAN5){
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
			isRightPin = 1;
		}
	}
	if(pr & (1 << 11) ){ 	//pending request on pin 11
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
		if(logAnlys.trigConfig == TRIG_CHAN6){
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
			isRightPin = 1;
		}
	}
	if(pr & (1 << 12) ){ 	//pending request on pin 12
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
		if(logAnlys.trigConfig == TRIG_CHAN7){
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
			isRightPin = 1;
		}
	}
	if(pr & (1 << 13) ){ 	//pending request on pin 13
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
		if(logAnlys.trigConfig == TRIG_CHAN8){
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
			isRightPin = 1;
		}
	}

	if(isRightPin == 1){
		logAnlys.trigOccur = TRIG_OCCURRED;
//		TIM_PostTrigger_SoftwareStart();
	}
	else{
		//stop TIM4 and reset
		//HAL_TIM_Base_Stop(&htim4);
		TIM4->CR1 &= ~(TIM_CR1_CEN);
		__HAL_TIM_SET_COUNTER(&htim4, 0x00);
	}
}


#endif //USE_LOG_ANLYS

/**
  * @brief  ISR used by Counter and Logic Analyzer.
  */
#if defined(USE_COUNTER) || defined(USE_LOG_ANLYS)
/**
* @brief This function handles TIM4 global interrupt.
*/
void TIM4_IRQHandler(void)
{
//  HAL_TIM_IRQHandler(&htim4);
#ifdef USE_LOG_ANLYS
	if(logAnlys.enable == LOGA_ENABLED){
		LOG_ANLYS_PeriodElapsedCallback(&htim4);
	}else{
#endif //USE_LOG_ANLYS
		COUNTER_PeriodElapsedCallback(&htim4);
#ifdef USE_LOG_ANLYS
	}
#endif //USE_LOG_ANLYS
}
#endif // USE_COUNTER || USE_LOG_ANLYS

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
