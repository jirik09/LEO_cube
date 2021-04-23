/**
  ******************************************************************************
  * File Name          : DAC.h
  * Date               : 18/01/2015 10:00:30
  * Description        : This file provides code for the configuration
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifdef USE_GEN
#ifndef __DAC_H
#define __DAC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

//extern DAC_HandleTypeDef hdac;

void MX_DAC_Init(void);
	 
void DAC_DMA_Reconfig(uint8_t chan, uint32_t *buff, uint32_t len);
void DACsetOutput(uint8_t chann, uint16_t val);
void DACDisableOutput(void);
void DACEnableOutput(void);
void DACSetOutputBuffer(void);
void DACUnsetOutputBuffer(void);
void GeneratingEnable (void);
void GeneratingDisable (void);

void DACSetModeGenerator(void);
void DACSetModeVoltageSource(void);

void GEN_DAC_DMA_deinit(void);
void GEN_DAC_deinit(void);
	 
extern DMA_HandleTypeDef hdma_dac1_ch1;
extern DMA_HandleTypeDef hdma_dac1_ch2;
	 
#ifdef __cplusplus
}
#endif
#endif /*__DAC_H */

/**
  * @}
  */

/**
  * @}
  */
#endif //USE_GEN
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
