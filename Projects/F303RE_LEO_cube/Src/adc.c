/**
 ******************************************************************************
 * @file          : ADC.c
 * @date          : 18/01/2015 10:00:30
 * @brief         : This file provides code for the configuration
 *                      of the ADC instances.
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
#include "adc.h"

#include "tim.h"
#include "gpio.h"
#include "dma.h"
#include "mcu_config.h"
#include "adc_channels.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
#ifdef USE_SCOPE

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
ADC_HandleTypeDef hadc4;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;
DMA_HandleTypeDef hdma_adc3;
DMA_HandleTypeDef hdma_adc4;


int32_t VddValue_mV = -1;


static int ADC12_CLK_ENABLED=0;
static int ADC34_CLK_ENABLED=0;

//uint16_t Data[3][32];
uint32_t ADCResolution=ADC_RESOLUTION_12B;
uint32_t ADCSamplingTime=ADC_SAMPLETIME_1CYCLE_5;
uint8_t ADCInterleaved = 0;
uint8_t ADCChannel[MAX_ADC_CHANNELS]={0};

/* ADC1 for VREF init function */
void MX_Vref_Init(void)
{

	ADC_ChannelConfTypeDef sConfig;

	/**Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	HAL_ADC_Init(&hadc1);

	/**Configure Regular Channel
	 */
	sConfig.Channel = ANALOG_CHANNEL_ADC1[ANALOG_VREF_INPUTS[0]];
	sConfig.Rank = 1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

}


/* ADC1 init function */
void MX_ADC1_Init(void)
{

	ADC_ChannelConfTypeDef sConfig;

	/**Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc1.Init.Resolution = ADCResolution;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T15_TRGO;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	HAL_ADC_Init(&hadc1);

	/**Configure Regular Channel
	 */
	sConfig.Channel = ANALOG_CHANNEL_ADC1[ADCChannel[0]];
	sConfig.Rank = 1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.SamplingTime = ADCSamplingTime;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
}

/* ADC2 init function */
void MX_ADC2_Init(void)
{

	ADC_ChannelConfTypeDef sConfig;

	/**Common config
	 */
	hadc2.Instance = ADC2;
	hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc2.Init.Resolution = ADCResolution;
	hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc2.Init.ContinuousConvMode = DISABLE;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T15_TRGO;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.NbrOfConversion = 1;
	hadc2.Init.DMAContinuousRequests = ENABLE;
	hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc2.Init.LowPowerAutoWait = DISABLE;
	hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	HAL_ADC_Init(&hadc2);



	/**Configure Regular Channel
	 */
	sConfig.Channel = ANALOG_CHANNEL_ADC2[ADCChannel[1]];
	sConfig.Rank = 1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.SamplingTime = ADCSamplingTime;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);
	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
}

/* ADC3 init function */
void MX_ADC3_Init(void)
{

	ADC_ChannelConfTypeDef sConfig;

	/**Common config
	 */
	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc3.Init.Resolution = ADCResolution;
	hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T15_TRGO;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc3.Init.NbrOfConversion = 1;
	hadc3.Init.DMAContinuousRequests = ENABLE;
	hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc3.Init.LowPowerAutoWait = DISABLE;
	hadc3.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	HAL_ADC_Init(&hadc3);

	/**Configure Regular Channel
	 */
	sConfig.Channel = ANALOG_CHANNEL_ADC3[ADCChannel[2]];
	sConfig.Rank = 1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.SamplingTime = ADCSamplingTime;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);
	HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
}

/* ADC4 init function */
void MX_ADC4_Init(void)
{

	ADC_ChannelConfTypeDef sConfig;

	/**Common config
	 */
	hadc4.Instance = ADC4;
	hadc4.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc4.Init.Resolution = ADCResolution;
	hadc4.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc4.Init.ContinuousConvMode = DISABLE;
	hadc4.Init.DiscontinuousConvMode = DISABLE;
	hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T15_TRGO;
	hadc4.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc4.Init.NbrOfConversion = 1;
	hadc4.Init.DMAContinuousRequests = ENABLE;
	hadc4.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc4.Init.LowPowerAutoWait = DISABLE;
	hadc4.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	HAL_ADC_Init(&hadc4);

	/**Configure Regular Channel
	 */
	sConfig.Channel = ANALOG_CHANNEL_ADC4[ADCChannel[3]];
	sConfig.Rank = 1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.SamplingTime = ADCSamplingTime;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	HAL_ADC_ConfigChannel(&hadc4, &sConfig);
	HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);
}


void MX_ADC12_Interleaved_Init(){

	ADC_ChannelConfTypeDef sConfig;
	ADC_MultiModeTypeDef   mode;

	hadc2.Instance = ADC2;
	hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc2.Init.Resolution = ADCResolution;
	hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc2.Init.ContinuousConvMode = DISABLE;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T15_TRGO;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.NbrOfConversion =1;
	hadc2.Init.DMAContinuousRequests = ENABLE;
	hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc2.Init.LowPowerAutoWait = DISABLE;
	hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	HAL_ADC_Init(&hadc2);

	/**Configure Regular Channel
	 */
	sConfig.Channel = ANALOG_CHANNEL_ADC1[ADCChannel[0]];
	sConfig.Rank = 1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);

	/**Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc1.Init.Resolution = ADCResolution;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T15_TRGO;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	HAL_ADC_Init(&hadc1);

	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	mode.Mode = ADC_DUALMODE_INTERL;
	if(ADCResolution==ADC_RESOLUTION_8B || ADCResolution==ADC_RESOLUTION_6B){
		mode.DMAAccessMode = ADC_DMAACCESSMODE_8_6_BITS;
		mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_4CYCLES;
	}else{
		mode.DMAAccessMode = ADC_DMAACCESSMODE_12_10_BITS;
		mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_6CYCLES;
	}
	HAL_ADCEx_MultiModeConfigChannel(&hadc1, &mode);

	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);

}
void MX_ADC34_Interleaved_Init(){

	ADC_ChannelConfTypeDef sConfig;
	ADC_MultiModeTypeDef   mode;

	hadc4.Instance = ADC4;
	hadc4.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc4.Init.Resolution = ADCResolution;
	hadc4.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc4.Init.ContinuousConvMode = DISABLE;
	hadc4.Init.DiscontinuousConvMode = DISABLE;
	hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T15_TRGO;
	hadc4.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc4.Init.NbrOfConversion =1;
	hadc4.Init.DMAContinuousRequests = ENABLE;
	hadc4.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc4.Init.LowPowerAutoWait = DISABLE;
	hadc4.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	HAL_ADC_Init(&hadc4);

	/**Configure Regular Channel
	 */
	sConfig.Channel = ANALOG_CHANNEL_ADC3[ADCChannel[2]];
	sConfig.Rank = 1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	HAL_ADC_ConfigChannel(&hadc4, &sConfig);

	/**Common config
	 */
	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc3.Init.Resolution = ADCResolution;
	hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T15_TRGO;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc3.Init.NbrOfConversion = 1;
	hadc3.Init.DMAContinuousRequests = ENABLE;
	hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc3.Init.LowPowerAutoWait = DISABLE;
	hadc3.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	HAL_ADC_Init(&hadc3);

	HAL_ADC_ConfigChannel(&hadc3, &sConfig);

	mode.Mode = ADC_DUALMODE_INTERL;
	if(ADCResolution==ADC_RESOLUTION_8B || ADCResolution==ADC_RESOLUTION_6B){
		mode.DMAAccessMode = ADC_DMAACCESSMODE_8_6_BITS;
		mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_4CYCLES;
	}else{
		mode.DMAAccessMode = ADC_DMAACCESSMODE_12_10_BITS;
		mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_6CYCLES;
	}
	HAL_ADCEx_MultiModeConfigChannel(&hadc3, &mode);

	HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);

}



void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(hadc->Instance==ADC1)
	{
		/* USER CODE BEGIN ADC1_MspInit 0 */

		/* USER CODE END ADC1_MspInit 0 */
		/* Peripheral clock enable */
		ADC12_CLK_ENABLED++;
		if(ADC12_CLK_ENABLED==1){
			__HAL_RCC_ADC12_CLK_ENABLE();
			__HAL_RCC_ADC12_CONFIG(RCC_ADC12PLLCLK_DIV1);
		}

		/**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN6 
		 */
		if(ANALOG_PIN_ADC1[ADCChannel[0]]!=0){  //skip pin setting when pin is not set (when measuring Vref or Temp) --> otherwise full assert triggerd
			GPIO_InitStruct.Pin = ANALOG_PIN_ADC1[ADCChannel[0]];
			GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init((GPIO_TypeDef *)ANALOG_GPIO_ADC1[ADCChannel[0]], &GPIO_InitStruct);
		}

		/* Peripheral DMA init*/

		hdma_adc1.Instance = DMA1_Channel1;
		hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
		if(ADCInterleaved){
			if (ADCResolution==ADC_RESOLUTION_8B || ADCResolution==ADC_RESOLUTION_6B){
				hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
				hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
			}else{
				hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
				hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
			}
		}else{
			if (ADCResolution==ADC_RESOLUTION_8B || ADCResolution==ADC_RESOLUTION_6B){
				hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
				hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			}else{
				hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
				hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
			}
		}
		hdma_adc1.Init.Mode = DMA_CIRCULAR;
		hdma_adc1.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		HAL_DMA_Init(&hdma_adc1);

		__HAL_LINKDMA(hadc,DMA_Handle,hdma_adc1);

		/* USER CODE BEGIN ADC1_MspInit 1 */

		/* USER CODE END ADC1_MspInit 1 */
	}
	else if(hadc->Instance==ADC2)
	{
		/* USER CODE BEGIN ADC2_MspInit 0 */

		/* USER CODE END ADC2_MspInit 0 */
		/* Peripheral clock enable */
		ADC12_CLK_ENABLED++;
		if(ADC12_CLK_ENABLED==1){
			__HAL_RCC_ADC12_CLK_ENABLE();
			__HAL_RCC_ADC12_CONFIG(RCC_ADC12PLLCLK_DIV1);
		}

		/**ADC2 GPIO Configuration
    PC1     ------> ADC2_IN7 
		 */
		if(ANALOG_PIN_ADC2[ADCChannel[1]]!=0){  //skip pin setting when pin is not set (when measuring Vref or Temp) --> otherwise full assert triggerd
			GPIO_InitStruct.Pin = ANALOG_PIN_ADC2[ADCChannel[1]];
			GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init((GPIO_TypeDef *)ANALOG_GPIO_ADC2[ADCChannel[1]], &GPIO_InitStruct);
		}
		/* Peripheral DMA init*/

		hdma_adc2.Instance = DMA2_Channel1;
		hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
		if (ADCResolution==ADC_RESOLUTION_8B || ADCResolution==ADC_RESOLUTION_6B){
			hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	
			hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		}else{
			hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
			hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		}
		hdma_adc2.Init.Mode = DMA_CIRCULAR;
		hdma_adc2.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		HAL_DMA_Init(&hdma_adc2);

		__HAL_LINKDMA(hadc,DMA_Handle,hdma_adc2);

		/* USER CODE BEGIN ADC2_MspInit 1 */

		/* USER CODE END ADC2_MspInit 1 */
	}
	else if(hadc->Instance==ADC3)
	{
		/* USER CODE BEGIN ADC3_MspInit 0 */

		/* USER CODE END ADC3_MspInit 0 */
		/* Peripheral clock enable */
		ADC34_CLK_ENABLED++;
		if(ADC34_CLK_ENABLED==1){
			__HAL_RCC_ADC34_CLK_ENABLE();
			__HAL_RCC_ADC34_CONFIG(RCC_ADC34PLLCLK_DIV1);
		}

		/**ADC3 GPIO Configuration
    PB0     ------> ADC3_IN12 
		 */
		if(ANALOG_PIN_ADC3[ADCChannel[2]]!=0){  //skip pin setting when pin is not set (when measuring Vref or Temp) --> otherwise full assert triggerd
			GPIO_InitStruct.Pin = ANALOG_PIN_ADC3[ADCChannel[2]];
			GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init((GPIO_TypeDef *)ANALOG_GPIO_ADC3[ADCChannel[2]], &GPIO_InitStruct);
		}
		/* Peripheral DMA init*/

		hdma_adc3.Instance = DMA2_Channel5;
		hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
		if(ADCInterleaved){
			if (ADCResolution==ADC_RESOLUTION_8B || ADCResolution==ADC_RESOLUTION_6B){
				hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
				hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
			}else{
				hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
				hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
			}
		}else{
			if (ADCResolution==ADC_RESOLUTION_8B || ADCResolution==ADC_RESOLUTION_6B){
				hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
				hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			}else{
				hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
				hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
			}
		}
		hdma_adc3.Init.Mode = DMA_CIRCULAR;
		hdma_adc3.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_adc3);

		__HAL_LINKDMA(hadc,DMA_Handle,hdma_adc3);

		/* USER CODE BEGIN ADC3_MspInit 1 */

		/* USER CODE END ADC3_MspInit 1 */
	}
	else if(hadc->Instance==ADC4)
	{
		/* USER CODE BEGIN ADC4_MspInit 0 */

		/* USER CODE END ADC4_MspInit 0 */
		/* Peripheral clock enable */
		ADC34_CLK_ENABLED++;
		if(ADC34_CLK_ENABLED==1){
			__HAL_RCC_ADC34_CLK_ENABLE();
			__HAL_RCC_ADC34_CONFIG(RCC_ADC34PLLCLK_DIV1);
		}

		/**ADC4 GPIO Configuration
    PB14     ------> ADC4_IN4 
		 */
		if(ANALOG_PIN_ADC4[ADCChannel[3]]!=0){  //skip pin setting when pin is not set (when measuring Vref or Temp) --> otherwise full assert triggerd
			GPIO_InitStruct.Pin = ANALOG_PIN_ADC4[ADCChannel[3]];
			GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init((GPIO_TypeDef *)ANALOG_GPIO_ADC4[ADCChannel[3]], &GPIO_InitStruct);
		}
		/* Peripheral DMA init*/

		hdma_adc4.Instance = DMA2_Channel2;
		hdma_adc4.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc4.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc4.Init.MemInc = DMA_MINC_ENABLE;
		if (ADCResolution==ADC_RESOLUTION_8B || ADCResolution==ADC_RESOLUTION_6B){
			hdma_adc4.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	
			hdma_adc4.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		}else{
			hdma_adc4.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
			hdma_adc4.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		}
		hdma_adc4.Init.Mode = DMA_CIRCULAR;
		hdma_adc4.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_adc4);

		__HAL_LINKDMA(hadc,DMA_Handle,hdma_adc4);

		/* USER CODE BEGIN ADC4_MspInit 1 */

		/* USER CODE END ADC4_MspInit 1 */
	}

}

//void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
//{

//  if(hadc->Instance==ADC1)
//  {
//  /* USER CODE BEGIN ADC1_MspDeInit 0 */

//  /* USER CODE END ADC1_MspDeInit 0 */
//    /* Peripheral clock disable */
//    ADC12_CLK_ENABLED--;
//    if(ADC12_CLK_ENABLED==0){
//      __ADC12_CLK_DISABLE();
//    }
//  
//    /**ADC1 GPIO Configuration    
//    PC0     ------> ADC1_IN6 
//    */
//    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);

//    /* Peripheral DMA DeInit*/
//    HAL_DMA_DeInit(hadc->DMA_Handle);
//  /* USER CODE BEGIN ADC1_MspDeInit 1 */

//  /* USER CODE END ADC1_MspDeInit 1 */
//  }
//  else if(hadc->Instance==ADC2)
//  {
//  /* USER CODE BEGIN ADC2_MspDeInit 0 */

//  /* USER CODE END ADC2_MspDeInit 0 */
//    /* Peripheral clock disable */
//    ADC12_CLK_ENABLED--;
//    if(ADC12_CLK_ENABLED==0){
//      __ADC12_CLK_DISABLE();
//    }
//  
//    /**ADC2 GPIO Configuration    
//    PC1     ------> ADC2_IN7 
//    */
//    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1);

//    /* Peripheral DMA DeInit*/
//    HAL_DMA_DeInit(hadc->DMA_Handle);
//  /* USER CODE BEGIN ADC2_MspDeInit 1 */

//  /* USER CODE END ADC2_MspDeInit 1 */
//  }
//  else if(hadc->Instance==ADC3)
//  {
//  /* USER CODE BEGIN ADC3_MspDeInit 0 */

//  /* USER CODE END ADC3_MspDeInit 0 */
//    /* Peripheral clock disable */
//    ADC34_CLK_ENABLED--;
//    if(ADC34_CLK_ENABLED==0){
//      __ADC34_CLK_DISABLE();
//    }
//  
//    /**ADC3 GPIO Configuration    
//    PB0     ------> ADC3_IN12 
//    */
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);

//    /* Peripheral DMA DeInit*/
//    HAL_DMA_DeInit(hadc->DMA_Handle);
//  /* USER CODE BEGIN ADC3_MspDeInit 1 */

//  /* USER CODE END ADC3_MspDeInit 1 */
//  }
//  else if(hadc->Instance==ADC4)
//  {
//  /* USER CODE BEGIN ADC4_MspDeInit 0 */

//  /* USER CODE END ADC4_MspDeInit 0 */
//    /* Peripheral clock disable */
//    ADC34_CLK_ENABLED--;
//    if(ADC34_CLK_ENABLED==0){
//      __ADC34_CLK_DISABLE();
//    }
//  
//    /**ADC4 GPIO Configuration    
//    PB14     ------> ADC4_IN4 
//    */
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);

//    /* Peripheral DMA DeInit*/
//    HAL_DMA_DeInit(hadc->DMA_Handle);
//  /* USER CODE BEGIN ADC4_MspDeInit 1 */

//  /* USER CODE END ADC4_MspDeInit 1 */
//  }

//} 

/* USER CODE BEGIN 1 */
void ADC_DMA_Reconfig(uint8_t chan, uint32_t *buff, uint32_t len){
	ADC_HandleTypeDef adcHandler;
	switch(chan){
	case 0:
		adcHandler=hadc1;
		break;
	case 1:
		adcHandler=hadc2;
		break;
	case 2:
		adcHandler=hadc3;
		break;
	case 3:
		adcHandler=hadc4;
		break;
	}

	if(buff!=NULL && len!=0){
		HAL_ADC_Start_DMA(&adcHandler, buff, len);
	}
}

void ADC_DMA_Reconfig_Interleave(uint8_t chan, uint32_t *buff, uint32_t len){
	ADC_HandleTypeDef adcHandlerMaster;
	ADC_HandleTypeDef adcHandlerSlave;
	switch(chan){
	case 0:
		adcHandlerMaster=hadc1;
		adcHandlerSlave=hadc2;
		break;
	case 1:
		adcHandlerMaster=hadc3;
		adcHandlerSlave=hadc4;
		break;
	}

	if(buff!=NULL && len!=0){
		HAL_ADC_Start(&adcHandlerSlave);
		HAL_ADCEx_MultiModeStart_DMA(&adcHandlerMaster, buff,len);
	}

}

void ADC_DMA_Stop(void){
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_ADC_Stop_DMA(&hadc2);
	HAL_ADC_Stop_DMA(&hadc3);
	HAL_ADC_Stop_DMA(&hadc4);

	//CalibrateADC();
}

/**
 * @brief  Returns the number of remaining data units in the current DMAy Streamx transfer.
 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
 *          to 7 to select the DMA Stream.
 * @retval The number of remaining data units in the current DMAy Streamx transfer.
 */
uint16_t DMA_GetCurrDataCounter(uint8_t channel){
	/* Return the number of remaining data units for DMAy Streamx */
	ADC_HandleTypeDef adcHandler;
	switch(channel){
	case 1:
		adcHandler=hadc1;
		break;
	case 2:
		adcHandler=hadc2;
		break;
	case 3:
		adcHandler=hadc3;
		break;
	case 4:
		adcHandler=hadc4;
		break;
	}

	if(ADCInterleaved==1){
		return (adcHandler.DMA_Handle->Instance->CNDTR)*2;
	}else{
		return adcHandler.DMA_Handle->Instance->CNDTR;
	}


}

/**
 * @brief  This function will estimate maximum time to connect sampling capacitor to reduce equivalen current
 * @param  None
 * @retval None
 */
void ADC_set_sampling_time(uint32_t realfreq){
	uint8_t ADCRes = 12;
	uint32_t cyclesForConversion;
	switch(ADCResolution){
	case ADC_RESOLUTION_12B:
		ADCRes=12;
		break;
	case ADC_RESOLUTION_10B:
		ADCRes=10;
		break;
	case ADC_RESOLUTION_8B:
		ADCRes=8;
		break;
	case ADC_RESOLUTION_6B:
		ADCRes=6;
		break;
	}

	cyclesForConversion=HAL_RCC_GetPCLK2Freq()/realfreq-ADCRes-1;
	if(cyclesForConversion>=601){
		ADCSamplingTime=ADC_SAMPLETIME_601CYCLES_5;
	}else if(cyclesForConversion>=181){
		ADCSamplingTime=ADC_SAMPLETIME_181CYCLES_5;
	}else if(cyclesForConversion>=61){
		ADCSamplingTime=ADC_SAMPLETIME_61CYCLES_5;
	}else if(cyclesForConversion>=19){
		ADCSamplingTime=ADC_SAMPLETIME_19CYCLES_5;
	}else if(cyclesForConversion>=7){
		ADCSamplingTime=ADC_SAMPLETIME_7CYCLES_5;
	}else if(cyclesForConversion>=4){
		ADCSamplingTime=ADC_SAMPLETIME_4CYCLES_5;
	}else if(cyclesForConversion>=2){
		ADCSamplingTime=ADC_SAMPLETIME_2CYCLES_5;
	}else {
		ADCSamplingTime=ADC_SAMPLETIME_1CYCLE_5;
	}	
}

/**
 * @brief  Enable sampling
 * @param  None
 * @retval None
 */
void samplingEnable (void){
	TIMScopeEnable();
}

/**
 * @brief  Disable sampling
 * @param  None
 * @retval None
 */
void samplingDisable (void){
	TIMScopeDisable();
}

void adcSetInputChannel(uint8_t adc, uint8_t chann){
	ADCChannel[adc]=chann;
	samplingDisable();
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_ADC_Stop_DMA(&hadc2);
	HAL_ADC_Stop_DMA(&hadc3);
	HAL_ADC_Stop_DMA(&hadc4);

	HAL_ADC_DeInit(&hadc1);
	HAL_ADC_DeInit(&hadc2);
	HAL_ADC_DeInit(&hadc3);
	HAL_ADC_DeInit(&hadc4);

	HAL_DMA_DeInit(&hdma_adc1);
	HAL_DMA_DeInit(&hdma_adc2);
	HAL_DMA_DeInit(&hdma_adc3);
	HAL_DMA_DeInit(&hdma_adc4);
}

void adcSetDefaultInputs(void){
	uint8_t i;
	for(i=0;i<MAX_ADC_CHANNELS;i++){
		ADCChannel[i]=ANALOG_DEFAULT_INPUTS[i];
	}
}


void adcSetResolution (uint8_t res){
	samplingDisable();
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_ADC_Stop_DMA(&hadc2);
	HAL_ADC_Stop_DMA(&hadc3);
	HAL_ADC_Stop_DMA(&hadc4);

	switch(res){
	case 6:
		ADCResolution = ADC_RESOLUTION_6B;
		break;
	case 8:
		ADCResolution = ADC_RESOLUTION_8B;
		break;
	case 10:
		ADCResolution = ADC_RESOLUTION_10B;
		break;
	case 12:
		ADCResolution = ADC_RESOLUTION_12B;
		break;
	default:
		return;
	}

	HAL_ADC_DeInit(&hadc1);
	HAL_ADC_DeInit(&hadc2);
	HAL_ADC_DeInit(&hadc3);
	HAL_ADC_DeInit(&hadc4);

	HAL_DMA_DeInit(&hdma_adc1);
	HAL_DMA_DeInit(&hdma_adc2);
	HAL_DMA_DeInit(&hdma_adc3);
	HAL_DMA_DeInit(&hdma_adc4);

}

void CalibrateADC (void){
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);
}

void MeasureRealAVDD(void){
	uint32_t read=0;
	MX_Vref_Init();
	for (uint8_t i = 0;i<8;i++){
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 10)== HAL_OK){
			read += HAL_ADC_GetValue(&hadc1);
		}
	}
	VddValue_mV = VREF_INT * AVDD_DEFAULT * 8 / read ;
}

int32_t getRealAVDD(void){
	return VddValue_mV;
}

void ADCInitNormalMode(void){
	HAL_ADC_DeInit(&hadc1);
	HAL_ADC_DeInit(&hadc2);
	HAL_ADC_DeInit(&hadc3);
	HAL_ADC_DeInit(&hadc4);

	ADCInterleaved=0;
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_ADC3_Init();
	MX_ADC4_Init();
}

void ADCInitInterleavedMode(void){
	HAL_ADC_DeInit(&hadc1);
	HAL_ADC_DeInit(&hadc2);
	HAL_ADC_DeInit(&hadc3);
	HAL_ADC_DeInit(&hadc4);

	ADCInterleaved=1;
	MX_ADC12_Interleaved_Init();
	MX_ADC34_Interleaved_Init();

}

void ADCInitMultiMode(void){
	//todo
}


/* USER CODE END 1 */

/**
 * @}
 */

/**
 * @}
 */
#endif //USE_SCOPE


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
