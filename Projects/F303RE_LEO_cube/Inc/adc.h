/**
  ******************************************************************************
  * File Name          : ADC.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifdef USE_SCOPE
#ifndef __ADC_H
#define __ADC_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
	 
	 
 

void MX_Vref_Init(void);
void MX_ADC1_Init(void);
void MX_ADC2_Init(void);
void MX_ADC3_Init(void);
void MX_ADC4_Init(void);


void MX_ADC12_Interleaved_Init(void);
void MX_ADC34_Interleaved_Init(void);

uint16_t DMA_GetCurrDataCounter(uint8_t channel);
void ADC_DMA_Reconfig(uint8_t chan, uint32_t *buff, uint32_t len);
void ADC_DMA_Reconfig_Interleave(uint8_t chan, uint32_t *buff, uint32_t len);
void ADC_DMA_Stop(void);
void ADC_set_sampling_time(uint32_t realfreq);
void samplingEnable(void);
void samplingDisable(void);
void adcSetInputChannel(uint8_t adc, uint8_t chann);
void adcSetDefaultInputs(void);
void adcSetResolution (uint8_t res);
void CalibrateADC (void);
void MeasureRealAVDD(void);
int32_t getRealAVDD(void);
void ADCInitNormalMode(void);
void ADCInitInterleavedMode(void);
void ADCInitMultiMode(void);


#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */
#endif //USE_SCOPE


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
