/**
 *****************************************************************************
 * @file    generator.c
 * @author  Y3288231
 * @date    Dec 15, 2014
 * @brief   This file contains oscilloscope functions
 *****************************************************************************
 */

// Includes ===================================================================
#if defined(USE_GEN) || defined(USE_GEN_PWM)
#include "cmsis_os.h"
#include "mcu_config.h"
#include "comms.h"
#include "generator.h"
#include "dac.h"
#include "tim.h"
#include "messages.h"

/** @defgroup Generator Generator
 * @{
 */

/** @defgroup Generator_Private_Variables Generator Private Variables
 * @{
 */
xQueueHandle generatorMessageQueue;
//uint8_t validateGenBuffUsage(void);
void clearGenBuffer(void);

volatile generatorTypeDef generator;
uint16_t blindValue=0;

uint16_t generatorBuffer[MAX_GENERATOR_BUFF_SIZE/2];
/**
 * @}
 */

/** @defgroup Generator_Function_Definitions Generator Function Definitions
 * @{
 */
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
	generatorSetDefault();

	while(1){

		xQueueReceive(generatorMessageQueue, &message, portMAX_DELAY);

		switch(message){
		case MSG_INVALIDATE:
			if(generator.state==GENERATOR_IDLE){}
			break;
		case MSG_GEN_START:
			if(generator.state==GENERATOR_IDLE){
				if(generator.modeState==GENERATOR_DAC){
					genInit();
					GeneratingEnable();
				}else if(generator.modeState==GENERATOR_PWM){
#ifdef USE_GEN_PWM
					genPwmInit();
					PWMGeneratingEnable();
#endif //USE_GEN_PWM
				}
				generator.state=GENERATOR_RUN;
			}
			break;
		case MSG_GEN_STOP:
			if(generator.state==GENERATOR_RUN){
				if(generator.modeState==GENERATOR_DAC){
					GeneratingDisable();
				}else if(generator.modeState==GENERATOR_PWM){
#ifdef USE_GEN_PWM
					PWMGeneratingDisable();
#endif //USE_GEN_PWM
				}
				generator.state=GENERATOR_IDLE;
			}
			break;
		case MSG_GEN_PWM_MODE: /* Set PWM mode */
#ifdef USE_GEN_PWM
			generatorSetModePWM();
			TIMGenPwmInit();
#endif //USE_GEN_PWM
			break;
		case MSG_GEN_DAC_MODE:  /* Set DAC mode */
			generatorSetModeDAC();
			TIMGenInit();
			break;
		case MSG_GEN_DEINIT:
			if(generator.modeState==GENERATOR_DAC){				
				TIMGenDacDeinit();
			}else if(generator.modeState==GENERATOR_PWM){
#ifdef USE_GEN_PWM
				TIMGenPwmDeinit();

#endif //USE_GEN_PWM
			}
			break;
		default:
			break;
		}
	}
}

/**
 * @brief  Sets arb. generator mode.
 * @param  mode: GEN_DAC or GEN_PWM
 * @retval None
 */
void genSetMode(uint8_t mode)
{
	uint16_t passMsg;

	switch(mode){
	case GEN_PWM:
		passMsg = MSG_GEN_PWM_MODE;
		xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
		break;
	case GEN_DAC:
		passMsg = MSG_GEN_DAC_MODE;
		xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
		break;
	default:
		break;
	}
}

/**
 * @brief  Sets generator mode to PWM.
 * @param  None
 * @retval None
 */
void generatorSetModePWM(void){
	//generator_deinit();
	generator.modeState = GENERATOR_PWM;
}

/**
 * @brief  Sets generator mode to DAC.
 * @param  None
 * @retval None
 */
void generatorSetModeDAC(void){
	//generator_deinit();
	//TIMGenPwmDeinit();	
	generator.modeState = GENERATOR_DAC;
}

/**
 * @brief  Generator deinitialization.
 * @param  None
 * @retval None
 */
void generator_deinit(void){
	switch(generator.modeState){
	case GENERATOR_PWM:
#ifdef USE_GEN_PWM
		TIMGenPwmDeinit();
#endif //USE_GEN_PWM
		break;
	case GENERATOR_DAC:
		TIMGenDacDeinit();
		break;
	}
}

//#ifdef USE_GEN_PWM
///**
// * @brief  Arb. PWM Generator frequency configurarion function.
// * @param  pscVal:	value of PSC register sent by host
// * @param  chan: channel number 1 or 2
// * @retval None
// */
//void genSetPwmFrequencyPSC(uint32_t pscVal, uint8_t chan){
//	TIM_GEN_PWM_PSC_Config(pscVal, chan);		// -1 subtraction made in PC app
//}

///**
// * @brief  Arb. PWM Generator frequency configurarion function.
// * @param  pscVal:	value of ARR register sent by host
// * @param  chan: channel number 1 or 2
// * @retval None
// */
//void genSetPwmFrequencyARR(uint32_t arrVal, uint8_t chan){
//	TIM_GEN_PWM_ARR_Config(arrVal, chan);		// -1 subtraction made in PC app
//}
//#endif //USE_GEN_PWM

/**
 * @brief  Generator set Default values
 * @param  None
 * @retval None
 */
void generatorSetDefault(void)
{
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

/**
 * @brief  Arb. DAC Generator initialization function.
 * @param  None
 * @retval None
 */
void genInit(void)
{	
	for(uint8_t i = 0;i<MAX_DAC_CHANNELS;i++){
		TIM_Reconfig_gen(generator.generatingFrequency[i],i,0);
		if(generator.numOfChannles>i){
			DAC_DMA_Reconfig(i,(uint32_t *)generator.pChanMem[i], generator.oneChanSamples[i]);
		}else{
			DAC_DMA_Reconfig(i,NULL,0);
		}
	}	
}

#ifdef USE_GEN_PWM
/**
 * @brief  Arb. PWM Generator initialization function.
 * @param  None
 * @retval None
 */
void genPwmInit(void)
{	
	for(uint8_t i = 0;i<MAX_DAC_CHANNELS;i++){
		TIM_Reconfig_gen(generator.generatingFrequency[i],i,0);
		if(generator.numOfChannles>i){
			TIM_DMA_Reconfig(i);			
		}
	}
}

void genPwmSetFrequency(double freq, uint8_t channel){
	double realPwmFrq;
	realPwmFrq = TIM_Reconfig_GenPwm(freq, channel);

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


#endif //USE_GEN_PWM	

/**
 * @brief  Common Generator set data length function.
 * @param
 * @retval None
 */
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
			result = TIM_Reconfig_gen_all(freq,&realFreq);
			for(uint8_t i=0;i<MAX_DAC_CHANNELS;i++){
				generator.generatingFrequency[i] = freq;
				generator.realGenFrequency[i] = realFreq;
			}
		}else{
			generator.generatingFrequency[chan-1] = freq;
			result = TIM_Reconfig_gen(generator.generatingFrequency[chan-1],chan-1,&realFreq);
			generator.realGenFrequency[chan-1] = realFreq;
		}
	}
	return result;
}



/**
 * @brief  Common function for sending real sampling frequency.
 * @param  None
 * @retval None
 */
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


/**
 * @brief 	Checks if scope settings doesn't exceed memory
 * @param  None
 * @retval err/ok
 */
//uint8_t validateGenBuffUsage(){
//	uint8_t result=1;
//	uint32_t data_len=generator.maxOneChanSamples;
//	if(generator.DAC_res>8){
//		data_len=data_len*2;
//	}
//	data_len=data_len*generator.numOfChannles;
//	if(data_len<=MAX_GENERATOR_BUFF_SIZE){
//		result=0;
//	}
//	return result;
//}

/**
 * @brief 	Clears generator buffer
 * @param  None
 * @retval None
 */
void clearGenBuffer(void){
	for(uint32_t i=0;i<MAX_GENERATOR_BUFF_SIZE/2;i++){
		generatorBuffer[i]=0;
	}
}

void genSetOutputBuffer(void){
	DACSetOutputBuffer();
}

void genUnsetOutputBuffer(void){
	DACUnsetOutputBuffer();
}

uint8_t genSetDAC(uint16_t chann1,uint16_t chann2){
	uint8_t result=0;
	if(generator.state==GENERATOR_IDLE){
		for(uint8_t i = 0;i<MAX_DAC_CHANNELS;i++){
			result+=genSetLength(1,i+1);
		}
		result+=genSetNumOfChannels(MAX_DAC_CHANNELS);
	}
	if(MAX_DAC_CHANNELS>0){
		*generator.pChanMem[0]=chann1;
		result+=genSetFrequency(100,1);
	}
	if(MAX_DAC_CHANNELS>1){
		*generator.pChanMem[1]=chann2;
		result+=genSetFrequency(100,2);
	}
	genStart();	


	return result;
}
/**
 * @brief  Start generator terminator skynet
 * @param  None
 * @retval None
 */
void genStart(void){
	uint16_t passMsg = MSG_GEN_START;
	xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
}

/**
 * @brief  Stop generator
 * @param  None
 * @retval None
 */
void genStop(void){
	uint16_t passMsg = MSG_GEN_STOP;
	xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
}

/**
 * @brief  Disable peripheral by reseting it.
 * @param  None
 * @retval None
 */
void genReset(void){
	uint16_t passMsg = MSG_GEN_DEINIT;
	xQueueSendToBack(generatorMessageQueue, &passMsg, portMAX_DELAY);
}

/**
 * @}
 */

#endif // USE_GEN || USE_GEN_PWM

/**
 * @}
 */
