/**
 ******************************************************************************
 * @file          : main.c
 * @date          : 18/01/2015 10:00:33
 * @brief         : Main program body
 ******************************************************************************
 *
 * COPYRIGHT(c) 2015 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "clock.h"
#include "usb_device.h"
#include "gpio.h"
#include "cmd_parser.h"
#include "comms.h"
#include "mcu_config.h"
#include "scope.h"
#include "generator.h"
#include "counter.h"
#include "sync_pwm.h"
#include "logic_analyzer.h"

/** @defgroup Main Main
 * @{
 */

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/**
 * @defgroup  Main_Private_Prototypes Main Private Function Prototypes
 * @{
 */
void SystemClock_Config(void);
static void StartThread(void const * argument);
/**
 * @}
 */

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  Main function.
 * @note 	Some periphs initialization, tasks creation and scheduler start.
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	__disable_irq();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MeasureRealAVDD();

	LED_On();
#ifdef USE_SCOPE
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_ADC3_Init();
	MX_ADC4_Init();
	MX_TIM15_Init();
	CalibrateADC();
	adcSetDefaultInputs();
#endif //USE_SCOPE

	/*
#ifdef USE_GEN

#endif //USE_GEN*/

#ifdef USE_SHIELD
	detectScopeShield();
#endif


	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Init code generated for FreeRTOS */
	/********************* Define Threads *********************/
	/* Create Start thread */
	osThreadDef(CMD_PARSER_TASK, CmdParserTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
	osThreadDef(USER_TASK, StartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	osThreadDef(COMM_TASK, CommTask, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE*2);
	//osThreadDef(LL_COMM_TASK, LLCommTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);

#ifdef USE_SCOPE
	osThreadDef(SCOPE_TASK, ScopeTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
	osThreadDef(SCOPE_TRIG_TASK, ScopeTriggerTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
#endif //USE_SCOPE

#ifdef USE_COUNTER
	osThreadDef(COUNTER_TASK, CounterTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
#endif //USE_COUNTER

#if defined(USE_GEN_SIGNAL) || defined(USE_GEN_PWM)
	osThreadDef(GENERATOR_TASK, GeneratorTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
#endif //USE_GEN_SIGNAL || USE_GEN_PWM

#ifdef USE_SYNC_PWM
	osThreadDef(SYNC_PWM_TASK, SyncPwmTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
#endif //USE_SYNC_PWM

#ifdef USE_LOG_ANLYS
	osThreadDef(LOG_ANLYS_TASK, LogAnlysTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
#endif //USE_LOG_ANLYS


	/********************* Create Threads *********************/
	osThreadCreate (osThread(CMD_PARSER_TASK), NULL);
	osThreadCreate (osThread(USER_TASK), NULL);
	osThreadCreate (osThread(COMM_TASK), NULL);
	//osThreadCreate (osThread(LL_COMM_TASK), NULL);

#ifdef USE_SCOPE
	osThreadCreate (osThread(SCOPE_TASK), NULL);
	osThreadCreate (osThread(SCOPE_TRIG_TASK), NULL);
#endif //USE_SCOPE

#ifdef USE_COUNTER
	osThreadCreate (osThread(COUNTER_TASK), NULL);
#endif //USE_COUNTER

#if defined(USE_GEN_SIGNAL) || defined(USE_GEN_PWM)
	osThreadCreate (osThread(GENERATOR_TASK), NULL);
#endif //USE_GEN_SIGNAL || USE_GEN_PWM

#ifdef USE_SYNC_PWM
	osThreadCreate (osThread(SYNC_PWM_TASK), NULL);
#endif //USE_SYNC_PWM

#ifdef USE_LOG_ANLYS
	osThreadCreate (osThread(LOG_ANLYS_TASK), NULL);
#endif //USE_LOG_ANLYS

	LED_Off();


	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */

	/* USER CODE BEGIN 3 */
	/* Infinite loop */
	while (1)
	{

	}
	/* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

static void StartThread(void const * argument)
{

	/* init code for USB_DEVICE */

	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for(;;)
	{
		//xQueueSendToBack(messageQueue, MSG_COMMS_FLUSH, portMAX_DELAY);
		osDelay(10);
	}

	/* USER CODE END 5 */

}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{

	while(1){

	}
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
