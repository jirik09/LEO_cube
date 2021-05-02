/**
 *****************************************************************************
 * @file    generator.c
 * @author  Y3288231
 * @date    Dec 15, 2014
 * @brief   This file contains oscilloscope functions
 *****************************************************************************
 */

// Includes ===================================================================
#if defined(USE_GEN_SIGNAL) || defined(USE_GEN_PWM) || defined(USE_GEN_PATTERN)
#include "cmsis_os.h"
#include "mcu_config.h"
#include "comms.h"
#include "generator.h"
#include "dac.h"
#include "tim.h"
#include "gpio.h"
#include "resources.h"
#include "messages.h"
#include "commands.h"

xQueueHandle generatorMessageQueue;
//uint8_t validateGenBuffUsage(void);
void clearGenBuffer(void);

volatile generatorTypeDef generator;
uint16_t blindValue=0;
uint16_t generatorBuffer[MAX_GENERATOR_BUFF_SIZE/2];

/**
 * @brief  Generator task function.
 * task is getting messages from other tasks and takes care about generator functions
 * @param  Task handler, parameters pointer
 * @retval None
 */
//portTASK_FUNCTION(vScopeTask, pvParameters){	
void GeneratorTask(void const *argument){

	//Build error on lines below? Lenght of Pin strings must be 4 chars long!!!
	CASSERT(sizeof(GEN_CH1_PIN_STR)==5);
	CASSERT(sizeof(GEN_CH2_PIN_STR)==5);

	uint16_t message = 0xFFFF;
	generatorMessageQueue = xQueueCreate(30, sizeof(message)/sizeof(uint8_t));
	//MX_DAC_Init();
	//MX_TIM6_Init();
	//MX_TIM7_Init();
	generatorSetDefault();

	while(1){

		xQueueReceive(generatorMessageQueue, &message, portMAX_DELAY);

		switch(message){

		case MSG_INVALIDATE:
			if(generator.state==GENERATOR_IDLE){}
			break;

		case MSG_GEN_START:
			if(generator.state==GENERATOR_IDLE){
				if(generator.modeState==GENERATOR_SIGNAL){
					genSignalInit();
					genSignalGeneratingEnable();
				}else if(generator.modeState==GENERATOR_PWM){
					genPwmInit();
					genPwmGeneratingEnable();
				}else if(generator.modeState==GENERATOR_PATTERN){
					genPatternInit();
					genPatternGeneratingEnable();
				}
				generator.state=GENERATOR_RUN;
			}
			break;

		case MSG_GEN_STOP:
			if(generator.state==GENERATOR_RUN){
				if(generator.modeState==GENERATOR_SIGNAL){
					genSignalGeneratingDisable();
				}else if(generator.modeState==GENERATOR_PWM){
					genPwmGeneratingDisable();
				}else if(generator.modeState==GENERATOR_PATTERN){
					genPatternGeneratingDisable();
				}
				generator.state=GENERATOR_IDLE;
			}
			break;

		case MSG_GEN_STOP_VOLTSOURCE:
			DAC_Deinit();
			break;

		case MSG_GEN_SIGNAL_MODE:  /* Set DAC mode */
			generator.DACMode = DAC_GEN_MODE;
			generator.modeState = GENERATOR_SIGNAL;
			generator.genTypeMessage = STR_GEN_SIGNAL;
			DAC_SetMode_SignalGenerator();
			TIM_GenSignal_Init();
			break;

		case MSG_GEN_VOLTSOURCE_MODE:  /* Set Voltage source mode / actually special case of DAC mode */
			generator.DACMode = DAC_VOLTSOURCE_MODE;
			DAC_SetMode_VoltageSource();
			break;

		case MSG_GEN_PWM_MODE:
			generator.modeState = GENERATOR_PWM;
			generator.genTypeMessage = STR_GEN_PWM;
			TIM_GenPwm_Init();
			break;

		case MSG_GEN_PATTERN_MODE:
			generator.modeState = GENERATOR_PATTERN;
			generator.genTypeMessage = STR_GEN_PATTERN;
			TIM_GenPattern_Init();
			break;

		case MSG_GEN_DEINIT:
			generator_deinit();
			break;

		default:
			break;
		}
	}
}

void genSetMode(uint8_t mode)
{
	uint16_t passMsg;

	switch(mode){
	case GEN_PWM:
		passMsg = MSG_GEN_PWM_MODE;
		xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
		break;
	case GEN_SIGNAL:
		passMsg = MSG_GEN_SIGNAL_MODE;
		xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
		break;
	case GEN_VOLTSOURCE:
		passMsg = MSG_GEN_VOLTSOURCE_MODE;
		xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
		break;
	case GEN_PATTERN:
		passMsg = MSG_GEN_PATTERN_MODE;
		xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
		break;
	default:
		break;
	}
}

void generator_deinit(void){
	switch(generator.modeState){
	case GENERATOR_PWM:
		releaseResources(GEN_PWM_RESOURCES);
		TIM_GenPwm_Deinit();
		break;
	case GENERATOR_SIGNAL:
		releaseResources(GEN_SIGNAL_RESOURCES);
		TIM_GenSignal_Deinit();
		DAC_Deinit();
		break;
	case GENERATOR_PATTERN:
		releaseResources(GEN_PATTERN_RESOURCES);
		TIM_GenPattern_Deinit();
		break;
	case NONE:
		break;
	}
}

void generatorSetDefault(void){
	generator.bufferMemory=generatorBuffer;
	for(uint8_t i = 0;i<MAX_DAC_CHANNELS;i++){
		generator.generatingFrequency[i]=DEFAULT_GENERATING_FREQ;
		generator.realGenFrequency[i]=DEFAULT_GENERATING_FREQ;
	}

	generator.numOfChannles=1;
	generator.maxOneChanSamples=MAX_GENERATOR_BUFF_SIZE/2;
	generator.oneChanSamples[0]=MAX_GENERATOR_BUFF_SIZE/2;
	generator.pChanMem[0]=generatorBuffer;
	generator.state=GENERATOR_IDLE;
	generator.DAC_res=DAC_DATA_DEPTH;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_GEN_SIGNAL

void genSignalInit(void){
	for(uint8_t i = 0;i<MAX_DAC_CHANNELS;i++){
		TIM_DataTransfer_FreqReconfig(generator.generatingFrequency[i],i,0);
		if(generator.numOfChannles>i){
			DAC_DmaReconfig(i,(uint32_t *)generator.pChanMem[i], generator.oneChanSamples[i]);
		}/*else{
			DAC_DmaReconfig(i,NULL,0);
		}*/
	}	
}

void genSignalGeneratingEnable(void){
	DAC_Output_Enable();
	TIM_GenSignal_Start();
}

void genSignalGeneratingDisable(void){
	TIM_GenSignal_Stop();
	DAC_Disable();
	DAC_Output_Disable();
}

#endif //USE_GEN_SIGNAL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_GEN_PWM

void genPwmInit(void){
	for(uint8_t i = 0;i<MAX_DAC_CHANNELS;i++){
		TIM_DataTransfer_FreqReconfig(generator.generatingFrequency[i],i,0);
		if(generator.numOfChannles>i){
			TIM_GenPwm_DmaReconfig(i);
		}
	}
}

void genPwmSetFrequency(double freq, uint8_t channel){
	double realPwmFrq;
	realPwmFrq = TIM_GenPwm_FreqReconfig(freq, channel);

	uint16_t passMsg;
	if(channel == 0){
		passMsg = MSG_GEN_PWM_REAL_FREQ_CH1;
		generator.realPwmFreqCh1 = realPwmFrq;
	}else if(channel == 1){
		passMsg = MSG_GEN_PWM_REAL_FREQ_CH2;
		generator.realPwmFreqCh2 = realPwmFrq;
	}
	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
}

/* not used yet */
double genPwmGetRealFreqCh1(void){
	return generator.realPwmFreqCh1;
}

void genPwmGeneratingEnable(void){
	TIM_GenPwm_Start();
}

void genPwmGeneratingDisable(void){
	TIM_GenPwm_Stop();
}

#endif //USE_GEN_PWM
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_GEN_PATTERN

void genPatternInit(void){
	TIM_DataTransfer_FreqReconfig(generator.generatingFrequency[0], 0, 0);
	TIM_GenPattern_DmaReconfig();
}

void genPatternGeneratingEnable(void){
	TIM_GenPattern_Start();
}

void genPatternGeneratingDisable(void){
	TIM_GenPattern_Stop();
}

#endif //USE_GEN_PATTERN
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Common Generator set data length function */
uint8_t genSetData(uint16_t index,uint8_t length,uint8_t chan){
	uint8_t result = GEN_INVALID_STATE;
	if(generator.state==GENERATOR_IDLE ){
		if ((index*2+length)/2<=generator.oneChanSamples[chan-1] && generator.numOfChannles>=chan){
			if(commBufferReadNBytes((uint8_t *)generator.pChanMem[chan-1]+index*2,length)==length && commBufferReadByte(&result)==0 && result==';'){
				result = 0;
				uint16_t passMsg = MSG_INVALIDATE;
				xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
			}else{
				result = GEN_INVALID_DATA;
			}
		}else{
			result = GEN_OUT_OF_MEMORY;
		}
	}
	return result;
}

/**
 * @brief  Arb. DAC Generator set frequency function.
 * @param  Freq: required generating frequency
 * @param  chan: channel number 1 or 2 (100 to set all)
 * @retval None
 */
uint8_t genSetFrequency(uint32_t freq,uint8_t chan){
	uint8_t result = GEN_TO_HIGH_FREQ;
	uint32_t realFreq;
	if(freq<=MAX_GENERATING_FREQ){
		if(chan == 100){
			result = TIM_DataTransfer_FreqReconfigAll(freq,&realFreq);
			for(uint8_t i=0;i<MAX_DAC_CHANNELS;i++){
				generator.generatingFrequency[i] = freq;
				generator.realGenFrequency[i] = realFreq;
			}
		}else{
			generator.generatingFrequency[chan-1] = freq;
			result = TIM_DataTransfer_FreqReconfig(generator.generatingFrequency[chan-1],chan-1,&realFreq);
			generator.realGenFrequency[chan-1] = realFreq;
		}
	}
	return result;
}

/* Common function for sending real sampling frequency */
void genSendRealSamplingFreq(void){
	uint16_t passMsg = MSG_GEN_SIGNAL_REAL_SAMPLING_FREQ_CH1;
	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
	passMsg = MSG_GEN_SIGNAL_REAL_SAMPLING_FREQ_CH2;
	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
}

void genDataOKSendNext(void){
	uint16_t passMsg = MSG_GEN_NEXT;
	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
}

void genStatusOK(void){
	uint16_t passMsg = MSG_GEN_OK;
	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
}

uint32_t genGetRealSmplFreq(uint8_t chan){
	return generator.realGenFrequency[chan-1];
}

uint8_t genSetLength(uint32_t length,uint8_t chan){
	uint8_t result=GEN_INVALID_STATE;
	if(generator.state==GENERATOR_IDLE){
		//uint32_t smpTmp=generator.maxOneChanSamples;
		if(length<=generator.maxOneChanSamples){
			generator.oneChanSamples[chan-1]=length;
			clearGenBuffer();
			result=0;
		}else{
			result = GEN_BUFFER_SIZE_ERR;
		}
		uint16_t passMsg = MSG_INVALIDATE;
		xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
	}
	return result;
}

uint8_t genSetNumOfChannels(uint8_t chan){
	uint8_t result=GEN_INVALID_STATE;
	uint8_t chanTmp=generator.numOfChannles;
	if(generator.state==GENERATOR_IDLE){
		if(chan<=MAX_DAC_CHANNELS){
			while(chanTmp>0){
				if(generator.oneChanSamples[--chanTmp]>MAX_GENERATOR_BUFF_SIZE/2/chan){
					return GEN_BUFFER_SIZE_ERR;
				}
			}
			generator.numOfChannles=chan;
			generator.maxOneChanSamples=MAX_GENERATOR_BUFF_SIZE/2/chan;
			for(uint8_t i=0;i<chan;i++){
				generator.pChanMem[i]=(uint16_t *)&generatorBuffer[i*generator.maxOneChanSamples];
			}
			result=0;
			uint16_t passMsg = MSG_INVALIDATE;
			xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
		}
	}
	return result;
}

void clearGenBuffer(void){
	for(uint32_t i=0;i<MAX_GENERATOR_BUFF_SIZE/2;i++){
		generatorBuffer[i]=0;
	}
}

void genSetOutputBuffer(void){
	DAC_OutputBuffer_Enable();
}

void genUnsetOutputBuffer(void){
	DAC_OutputBuffer_Disable();
}

uint8_t genSetDAC(uint16_t chann1,uint16_t chann2){
	//setModeVoltageSource();
	uint8_t result=0;
	DAC_SetOutput(0,chann1);
	DAC_SetOutput(1,chann2);
	return result;
}

/* Start generator terminator skynet */
void genStart(void){
	uint16_t passMsg = MSG_GEN_START;
	xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
}

void genStop(void){
	uint16_t passMsg = MSG_GEN_STOP;
	xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
}

void genStopVoltSource(void){
	uint16_t passMsg = MSG_GEN_STOP_VOLTSOURCE;
	xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
}

/* Disable peripheral by reseting it */
void genReset(void){
	uint16_t passMsg = MSG_GEN_DEINIT;
	xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
}

#endif // USE_GEN_SIGNAL || USE_GEN_PWM || USE_GEN_PATTERN
