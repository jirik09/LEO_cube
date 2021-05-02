/**
 ******************************************************************************
 * @file          : DAC.c
 * @date          : 18/01/2015 10:00:30
 * @brief         : This file provides code for the configuration
 *                      of the DAC instances.
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

#ifdef USE_GEN_SIGNAL
#include "dac.h"
#include "gpio.h"
#include "tim.h"
#include "generator.h"

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1_ch1;
DMA_HandleTypeDef hdma_dac1_ch2;

uint32_t outputBuffEn=DAC_OUTPUTBUFFER_ENABLE;

/* DAC init function */
void MX_DAC_Init(void){
	DAC_ChannelConfTypeDef sConfig;

	/**DAC Initialization */
	hdac.Instance = DAC;
	HAL_DAC_Init(&hdac);

	/**DAC channel OUT1 config */
	sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
	sConfig.DAC_OutputBuffer = outputBuffEn;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

	/**DAC channel OUT2 config */
	sConfig.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2);
}

void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac){
	GPIO_InitTypeDef GPIO_InitStruct;

	if(hdac->Instance==DAC1) {
		__HAL_RCC_DAC1_CLK_ENABLE();

		/** DAC GPIO Configuration
			PA4 --> DAC_OUT1
			PA5 --> DAC_OUT2 */
		GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		hdma_dac1_ch1.Instance = DMA1_Channel3;
		hdma_dac1_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_dac1_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_dac1_ch1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_dac1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_dac1_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_dac1_ch1.Init.Mode = DMA_CIRCULAR;
		hdma_dac1_ch1.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_dac1_ch1);
		__HAL_LINKDMA(hdac,DMA_Handle1,hdma_dac1_ch1);
		__HAL_DMA_REMAP_CHANNEL_ENABLE(HAL_REMAPDMA_TIM6_DAC1_CH1_DMA1_CH3);

		hdma_dac1_ch2.Instance = DMA1_Channel4;
		hdma_dac1_ch2.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_dac1_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_dac1_ch2.Init.MemInc = DMA_MINC_ENABLE;
		hdma_dac1_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_dac1_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_dac1_ch2.Init.Mode = DMA_CIRCULAR;
		hdma_dac1_ch2.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_dac1_ch2);
		__HAL_LINKDMA(hdac,DMA_Handle2,hdma_dac1_ch2);
		__HAL_DMA_REMAP_CHANNEL_ENABLE(HAL_REMAPDMA_TIM7_DAC1_CH2_DMA1_CH4);
	}
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac){
	if(hdac->Instance==DAC){
		__HAL_RCC_DAC1_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5);
		HAL_DMA_DeInit(hdac->DMA_Handle1);
		HAL_DMA_DeInit(hdac->DMA_Handle2);
	}
} 

/* USER CODE BEGIN 1 */
void DAC_SetMode_SignalGenerator(void){
	MX_DAC_Init();
	HAL_DAC_MspInit(&hdac);
}

void DAC_SetMode_VoltageSource(void){
	hdac.Instance = DAC;
	DAC_ChannelConfTypeDef sConfig;
	HAL_DAC_Init(&hdac);

	/**DAC channel OUT1 config */
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = outputBuffEn;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

	/**DAC channel OUT2 config */
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2);
}

void DAC_Output_Enable(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void DAC_Output_Disable(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void DAC_OutputBuffer_Enable(void){
	outputBuffEn=DAC_OUTPUTBUFFER_ENABLE;
}

void DAC_OutputBuffer_Disable(void){
	outputBuffEn=DAC_OUTPUTBUFFER_DISABLE;
}

void DAC_SetOutput(uint8_t chann, uint16_t val){
	if(chann==0){
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, val);
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	}
	if(chann==1){
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, val);
		HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	}
}

void DAC_Disable(void){
	HAL_DAC_Stop(&hdac,DAC_CHANNEL_1);
	HAL_DAC_Stop(&hdac,DAC_CHANNEL_2);
	//HAL_DMA_DeInit(hdac.DMA_Handle1);
	//HAL_DMA_DeInit(hdac.DMA_Handle2);
}

void DAC_Deinit(void){
	__HAL_RCC_DAC1_FORCE_RESET();
	__HAL_RCC_DAC1_RELEASE_RESET();
}

void DAC_DmaDeinit(void){
	HAL_DAC_MspDeInit(&hdac);
}

void DAC_DmaReconfig(uint8_t chan, uint32_t *buff, uint32_t len){
	uint32_t dacChannel=0;
	switch(chan){
	case 0:
		dacChannel=DAC_CHANNEL_1;
		break;
	case 1:
		dacChannel=DAC_CHANNEL_2;
		break;
	}
	HAL_DAC_Stop_DMA(&hdac,dacChannel);
	if(len>0){  //dont start DMA with length zero
		HAL_DAC_Start_DMA(&hdac, dacChannel, buff, len, DAC_ALIGN_12B_R);
	}
}
/* USER CODE END 1 */

/**
 * @}
 */

/**
 * @}
 */
#endif //USE_GEN_SIGNAL
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
