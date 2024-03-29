/**
  ******************************************************************************
  * File Name          : ADC.c
  * Date               : 18/01/2015 10:00:30
  * Description        : This file provides code for the configuration
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
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;
DMA_HandleTypeDef hdma_adc3;

int32_t VddValue_mV = -1;

//uint16_t Data[3][32];
uint32_t ADCResolution=ADC_RESOLUTION12b;
uint32_t ADCSamplingTime=ADC_SAMPLETIME_3CYCLES;
uint8_t ADCInterleaved = 0;
uint8_t ADCChannel[MAX_ADC_CHANNELS]={0};


void MX_Vref_Init(void)
{

	ADC_ChannelConfTypeDef sConfig;

	/**Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	HAL_ADC_Init(&hadc1);

	/**Configure Regular Channel
	 */
	sConfig.Channel = ANALOG_CHANNEL_ADC1[ANALOG_VREF_INPUTS[0]];
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
	sConfig.Offset = 0;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
  hadc1.Init.Resolution = ADCResolution;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = EOC_SINGLE_CONV;
  HAL_ADC_Init(&hadc1);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ANALOG_CHANNEL_ADC1[ADCChannel[0]];
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADCSamplingTime;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

//  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
}

/* ADC2 init function */
void MX_ADC2_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
  hadc2.Init.Resolution = ADCResolution;
  hadc2.Init.ScanConvMode = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = EOC_SINGLE_CONV;
  HAL_ADC_Init(&hadc2);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
  sConfig.Channel = ANALOG_CHANNEL_ADC2[ADCChannel[1]];
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADCSamplingTime;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);
//  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
}

/* ADC3 init function */
void MX_ADC3_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
  hadc3.Init.Resolution = ADCResolution;
  hadc3.Init.ScanConvMode = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = EOC_SINGLE_CONV;
  HAL_ADC_Init(&hadc3);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
  sConfig.Channel = ANALOG_CHANNEL_ADC3[ADCChannel[2]];
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADCSamplingTime;
  HAL_ADC_ConfigChannel(&hadc3, &sConfig);
// HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
}


void MX_ADC123_Interleaved_Init(){

	  ADC_ChannelConfTypeDef sConfig;
	  ADC_MultiModeTypeDef   mode;

	/*  hadc3.Instance = ADC3;
	  hadc3.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	  hadc3.Init.Resolution = ADCResolution;
	  hadc3.Init.ScanConvMode = DISABLE;
	  hadc3.Init.ContinuousConvMode = ENABLE;
	  hadc3.Init.DiscontinuousConvMode = DISABLE;
	  hadc3.Init.NbrOfDiscConversion = 0;
	  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
	  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc3.Init.NbrOfConversion =1;
	  hadc3.Init.DMAContinuousRequests = DISABLE;
	  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  HAL_ADC_Init(&hadc3);*/

	  sConfig.Channel = ANALOG_CHANNEL_ADC1[ANALOG_DEFAULT_INPUTS[0]];
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	//  HAL_ADC_ConfigChannel(&hadc3, &sConfig);

	  hadc2.Instance = ADC2;
	  hadc2.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	  hadc2.Init.Resolution = ADCResolution;
	  hadc2.Init.ScanConvMode = ENABLE;
	  hadc2.Init.ContinuousConvMode = DISABLE;
	  hadc2.Init.DiscontinuousConvMode = DISABLE;
	  hadc2.Init.NbrOfDiscConversion = 0;
	  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
	  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc2.Init.NbrOfConversion =1;
	  hadc2.Init.DMAContinuousRequests = ENABLE;
	  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  HAL_ADC_Init(&hadc2);

	    /**Configure Regular Channel
	    */
	  HAL_ADC_ConfigChannel(&hadc2, &sConfig);

	    /**Common config
	    */
	  hadc1.Instance = ADC1;
	  hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	  hadc1.Init.Resolution = ADCResolution;
	  hadc1.Init.ScanConvMode = ENABLE;
	  hadc1.Init.ContinuousConvMode = DISABLE;
	  hadc1.Init.DiscontinuousConvMode = DISABLE;
	  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
	  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1.Init.NbrOfConversion = 1;
	  hadc1.Init.DMAContinuousRequests = ENABLE;
	  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  HAL_ADC_Init(&hadc1);

	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  mode.Mode = ADC_DUALMODE_INTERL;
	  if(ADCResolution==ADC_RESOLUTION_8B || ADCResolution==ADC_RESOLUTION_6B){
		  mode.DMAAccessMode = ADC_DMAACCESSMODE_3; //8bit
		  mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_6CYCLES;
	  }else{
		  mode.DMAAccessMode = ADC_DMAACCESSMODE_2; //12bit
		  mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_7CYCLES;
	  }
	  HAL_ADCEx_MultiModeConfigChannel(&hadc1, &mode);

//	  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
//	  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
//	  HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);

}


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* Peripheral clock enable */
    __ADC1_CLK_ENABLE();
  
    /**ADC1 GPIO Configuration    
    PC1     ------> ADC1_IN11 
    */
    if(ADCInterleaved == 0 && ANALOG_PIN_ADC1[ADCChannel[0]]!=0){
    	GPIO_InitStruct.Pin = ANALOG_PIN_ADC1[ADCChannel[0]];
    	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    	GPIO_InitStruct.Pull = GPIO_NOPULL;
    	HAL_GPIO_Init(ANALOG_GPIO_ADC1[ADCChannel[0]], &GPIO_InitStruct);
    }
    if(ADCInterleaved && ANALOG_PIN_ADC3[ADCChannel[0]]!=0){
    	GPIO_InitStruct.Pin = ANALOG_PIN_ADC3[ADCChannel[0]];
    	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    	GPIO_InitStruct.Pull = GPIO_NOPULL;
    	HAL_GPIO_Init(ANALOG_GPIO_ADC3[ADCChannel[0]], &GPIO_InitStruct);
    }
    /* Peripheral DMA init*/
  
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
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
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_adc1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_adc1.Init.PeriphBurst = DMA_PBURST_SINGLE;
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
	  __ADC2_CLK_ENABLE();

    /**ADC2 GPIO Configuration
    PC2     ------> ADC2_IN12
    */
	if(ANALOG_PIN_ADC2[ADCChannel[1]]!=0){
		GPIO_InitStruct.Pin = ANALOG_PIN_ADC2[ADCChannel[1]];
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ANALOG_GPIO_ADC2[ADCChannel[1]], &GPIO_InitStruct);
	}

    /* Peripheral DMA init*/

    hdma_adc2.Instance = DMA2_Stream2;
    hdma_adc2.Init.Channel = DMA_CHANNEL_1;
    hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
		if (ADCResolution==ADC_RESOLUTION8b || ADCResolution==ADC_RESOLUTION6b){
			hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
			hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		}else{
			hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
			hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		}
    hdma_adc2.Init.Mode = DMA_CIRCULAR;
    hdma_adc2.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adc2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_adc2.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_adc2.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_adc2.Init.PeriphBurst = DMA_PBURST_SINGLE;
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
	  __ADC3_CLK_ENABLE();

    /**ADC3 GPIO Configuration
    PC3     ------> ADC3_IN13
    */
	if(ANALOG_PIN_ADC3[ADCChannel[2]]!=0){
		GPIO_InitStruct.Pin = ANALOG_PIN_ADC3[ADCChannel[2]];
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ANALOG_GPIO_ADC3[ADCChannel[2]], &GPIO_InitStruct);
	}

    /* Peripheral DMA init*/

    hdma_adc3.Instance = DMA2_Stream1;
    hdma_adc3.Init.Channel = DMA_CHANNEL_2;
    hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
		if (ADCResolution==ADC_RESOLUTION8b || ADCResolution==ADC_RESOLUTION6b){
			hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
			hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		}else{
			hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
			hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		}
    hdma_adc3.Init.Mode = DMA_CIRCULAR;
    hdma_adc3.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adc3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_adc3.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_adc3.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_adc3.Init.PeriphBurst = DMA_PBURST_SINGLE;
    HAL_DMA_Init(&hdma_adc3);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc3);

  /* USER CODE BEGIN ADC3_MspInit 1 */

  /* USER CODE END ADC3_MspInit 1 */
  }
}

//void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
//{
//
//  if(hadc->Instance==ADC1)
//  {
//  /* USER CODE BEGIN ADC1_MspDeInit 0 */
//
//  /* USER CODE END ADC1_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __ADC1_CLK_DISABLE();
//
//    /**ADC1 GPIO Configuration
//    PC1     ------> ADC1_IN11
//    */
// //   HAL_GPIO_DeInit(ADC_GPIO_CH_1, ADC_PIN_CH_1);
//
//    /* Peripheral DMA DeInit*/
//    HAL_DMA_DeInit(hadc->DMA_Handle);
//  /* USER CODE BEGIN ADC1_MspDeInit 1 */
//
//  /* USER CODE END ADC1_MspDeInit 1 */
//  }
//  else if(hadc->Instance==ADC2)
//  {
//  /* USER CODE BEGIN ADC2_MspDeInit 0 */
//
//  /* USER CODE END ADC2_MspDeInit 0 */
//    /* Peripheral clock disable */
//	  __ADC2_CLK_DISABLE();
//
//	/**ADC2 GPIO Configuration
//    PC2     ------> ADC2_IN12
//    */
//
//    /* Peripheral DMA DeInit*/
//    HAL_DMA_DeInit(hadc->DMA_Handle);
//  /* USER CODE BEGIN ADC2_MspDeInit 1 */
//
//  /* USER CODE END ADC2_MspDeInit 1 */
//  }
//  else if(hadc->Instance==ADC3)
//  {
//  /* USER CODE BEGIN ADC3_MspDeInit 0 */
//
//  /* USER CODE END ADC3_MspDeInit 0 */
//    /* Peripheral clock disable */
//	  __ADC3_CLK_DISABLE();
//
//    /**ADC3 GPIO Configuration
//    PC3     ------> ADC3_IN13
//    */
//
//    /* Peripheral DMA DeInit*/
//    HAL_DMA_DeInit(hadc->DMA_Handle);
//  /* USER CODE BEGIN ADC3_MspDeInit 1 */
//
//  /* USER CODE END ADC3_MspDeInit 1 */
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
			adcHandler=hadc3;
		break;
		case 2:
			adcHandler=hadc2;
		break;
	}

	if(buff!=NULL && len!=0){
		HAL_ADC_Start_DMA(&adcHandler, buff, len);
	}
}

void ADC_DMA_Reconfig_Interleave(uint8_t chan, uint32_t *buff, uint32_t len){

	if(buff!=NULL && len!=0){
		HAL_ADC_Start(&hadc2);
		HAL_ADCEx_MultiModeStart_DMA(&hadc1, buff,len);
		//HAL_ADC_Start(&hadc1);
	}

}

void ADC_DMA_Stop(void){
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_ADC_Stop_DMA(&hadc2);
	HAL_ADC_Stop_DMA(&hadc3);
	//HAL_ADC_Stop_DMA(&hadc4);
	
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
			adcHandler=hadc3;
		break;
		case 3:
			adcHandler=hadc2;
		break;
	}

	if(ADCInterleaved==1){
		adcHandler=hadc1;
		return (adcHandler.DMA_Handle->Instance->NDTR)*2;
	}else{
		return adcHandler.DMA_Handle->Instance->NDTR;
	}
}

/**
  * @brief  This function will estimate maximum time to connect sampling capacitor to reduce equivalen current
  * @param  None
  * @retval None
  */
void ADC_set_sampling_time(uint32_t realfreq){
	uint8_t ADCRes;
	uint32_t cyclesForConversion;
	switch(ADCResolution){
		case ADC_RESOLUTION12b:
			ADCRes=12;
			break;
		case ADC_RESOLUTION10b:
			ADCRes=10;
			break;
		case ADC_RESOLUTION8b:
			ADCRes=8;
			break;
		case ADC_RESOLUTION6b:
			ADCRes=6;
			break;
	}
	
	cyclesForConversion=HAL_RCC_GetPCLK2Freq()/2/realfreq-ADCRes;
	if(cyclesForConversion>=481){
		ADCSamplingTime=ADC_SAMPLETIME_480CYCLES;
	}else if(cyclesForConversion>=145){
		ADCSamplingTime=ADC_SAMPLETIME_144CYCLES;
	}else if(cyclesForConversion>=113){
		ADCSamplingTime=ADC_SAMPLETIME_112CYCLES;
	}else if(cyclesForConversion>=85){
		ADCSamplingTime=ADC_SAMPLETIME_84CYCLES;
	}else if(cyclesForConversion>=57){
		ADCSamplingTime=ADC_SAMPLETIME_56CYCLES;
	}else if(cyclesForConversion>=29){
		ADCSamplingTime=ADC_SAMPLETIME_28CYCLES;
	}else if(cyclesForConversion>=16){
		ADCSamplingTime=ADC_SAMPLETIME_15CYCLES;
	}else {
		ADCSamplingTime=ADC_SAMPLETIME_3CYCLES;
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

	HAL_ADC_DeInit(&hadc1);
	HAL_ADC_DeInit(&hadc2);
	HAL_ADC_DeInit(&hadc3);

	HAL_DMA_DeInit(&hdma_adc1);
	HAL_DMA_DeInit(&hdma_adc2);
	HAL_DMA_DeInit(&hdma_adc3);

	//MX_ADC1_Init();
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
	if(res==8){
		ADCResolution	= ADC_RESOLUTION8b;
	}else if(res==12){
		ADCResolution	= ADC_RESOLUTION12b;
	}else{
		return;
	}
	
	HAL_ADC_DeInit(&hadc1);
	HAL_ADC_DeInit(&hadc2);
	HAL_ADC_DeInit(&hadc3);
	
	HAL_DMA_DeInit(&hdma_adc1);
	HAL_DMA_DeInit(&hdma_adc2);
	HAL_DMA_DeInit(&hdma_adc3);
}

void MeasureRealAVDD(void){
	uint32_t read=0;
	MX_Vref_Init();
	for (uint8_t i = 0;i<8;i++){
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 100)== HAL_OK){
			read += HAL_ADC_GetValue(&hadc1);
		}else{
			for(;;);
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

	ADCInterleaved=0;
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_ADC3_Init();
}

void ADCInitInterleavedMode(void){
	HAL_ADC_DeInit(&hadc1);
	HAL_ADC_DeInit(&hadc2);
	HAL_ADC_DeInit(&hadc3);

	ADCInterleaved=1;
	MX_ADC123_Interleaved_Init();
}
/* USER CODE END 1 */

void ADCInitMultiMode(void){
//todo
}
/**
  * @}
  */

/**
  * @}
  */
#endif //USE_SCOPE


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
