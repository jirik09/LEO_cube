/**
 *****************************************************************************
 * @file    scope.c
 * @author  Y3288231
 * @date    Dec 15, 2014
 * @brief   This file contains oscilloscope functions
 *****************************************************************************
 */

// Includes ===================================================================
#ifdef USE_SCOPE

#include "cmsis_os.h"
#include "mcu_config.h"
#include "adc_channels.h"
#include "comms.h"
#include "scope.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "messages.h"

/** @defgroup Scope Scope
 * @{
 */

const int16_t RANGES[8] = {RANGE_1_LOW,RANGE_1_HI,RANGE_2_LOW,RANGE_2_HI,RANGE_3_LOW,RANGE_3_HI,RANGE_4_LOW,RANGE_4_HI};
#ifdef USE_SHIELD
const int16_t SHIELD_RANGES[8] = {SHIELD_RANGE_1_LOW,SHIELD_RANGE_1_HI,SHIELD_RANGE_2_LOW,SHIELD_RANGE_2_HI,SHIELD_RANGE_3_LOW,SHIELD_RANGE_3_HI,SHIELD_RANGE_4_LOW,SHIELD_RANGE_4_HI};
#endif

/** @defgroup Scope_Private_Variables Scope Private Variables
 * @{
 */
xQueueHandle scopeMessageQueue;

uint8_t scopeBuffer[MAX_SCOPE_BUFF_SIZE+MAX_ADC_CHANNELS*SCOPE_BUFFER_MARGIN]; 
uint8_t blindBuffer[MAX_ADC_CHANNELS];
static uint32_t triggerIndex;
static int16_t triggerShift;
static uint16_t triggerLevel;
static uint32_t samplesToStop=0;
static uint32_t samplesToStart=0;
static uint8_t autoTrigged=0;

static xSemaphoreHandle scopeMutex ;
static uint32_t writingIndex=0;
static uint32_t lastWritingIndex=0;
static volatile scopeTypeDef scope;

uint32_t actualIndex = 0;
uint16_t data = 0;
uint32_t samplesTaken = 0;
uint32_t totalSmpTaken = 0;
uint32_t SmpBeforeTrig=0;
uint32_t SmpAfterTrig=0;
/**
 * @}
 */

/** @defgroup Scope_Functions_Prototypes Scope Functions Prototypes
 * @{
 */
uint16_t samplesPassed(uint16_t dataRemain, uint16_t lastDataRemain);
uint8_t validateBuffUsage(void);
/**
 * @}
 */


/** @defgroup Scope_Function_Definitions Scope Function Definitions
 * @{
 */

/**
 * @brief  Oscilloscope task function.
 * 				task is getting messages from other tasks and takes care about oscilloscope functions
 * @param  Task handler, parameters pointer
 * @retval None
 */

void ScopeTask(void const *argument){

	//Build error on lines below? Lenght of Pin strings must be 4 chars long!!!
	CASSERT(sizeof(SCOPE_CH1_PIN_STR)==5);
	CASSERT(sizeof(SCOPE_CH2_PIN_STR)==5);
	CASSERT(sizeof(SCOPE_CH3_PIN_STR)==5);
	CASSERT(sizeof(SCOPE_CH4_PIN_STR)==5);

	uint16_t message = 0xFFFF;
	scopeMessageQueue = xQueueCreate(30, sizeof(message)/sizeof(uint8_t));

	scopeMutex = xSemaphoreCreateRecursiveMutex();
	scopeSetDefault();

	while(1){
		xQueueReceive(scopeMessageQueue, &message, portMAX_DELAY);
		xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);

		if(message == MSG_SCOPE_DATA_SENT && scope.state != SCOPE_IDLE){ //Data was sent. Actualisation of scope sxtate and/or rerun
			scope.state = SCOPE_WAIT_FOR_RESTART;
		}else if(message == MSG_INVALIDATE){  //Settings has been changed
			if(scope.state == SCOPE_DONE || scope.state == SCOPE_IDLE){
			}else{
				samplingDisable();
				scopeInit();
				if(scope.state!=SCOPE_WAIT_FOR_RESTART && scope.state!=SCOPE_DATA_SENDING){
					scope.state=SCOPE_SAMPLING_WAITING;
					samplesTaken=0;
					samplingEnable();
				}
			}	
		}else if (message == MSG_SCOPE_START && scope.state != SCOPE_SAMPLING_WAITING && scope.state != SCOPE_SAMPLING_TRIGGER_WAIT && scope.state != SCOPE_SAMPLING && scope.state != SCOPE_DATA_SENDING){ //Enable sampling
			scopeInit();
			scope.state=SCOPE_SAMPLING_WAITING;
			samplingEnable();
		//	uint16_t passMsg = MSG_SCOPE_SMPL_STARTED;
		//	xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		}else if (message == MSG_SCOPE_STOP){//Disable sampling
			samplingDisable();
			scope.state = SCOPE_IDLE;
		}else if (message == MSG_SCOPE_RESTART && scope.state==SCOPE_WAIT_FOR_RESTART ){ //Rerun sampling
			//scopeInit();
			samplingEnable();
			scope.state=SCOPE_SAMPLING_WAITING;
		}
		xSemaphoreGiveRecursive(scopeMutex);
	}
}

/**
 * @brief  Oscilloscope trigger finding task function.
 * 				Task is finding trigger edge when oscilloscope is sampling.
 * @param  Task handler, parameters pointer
 * @retval None
 */
//portTASK_FUNCTION(vScopeTriggerTask, pvParameters) {
void ScopeTriggerTask(void const *argument) {

	while(1){
		if(scope.state==SCOPE_SAMPLING_WAITING || scope.state==SCOPE_SAMPLING_TRIGGER_WAIT || scope.state==SCOPE_SAMPLING){
			xSemaphoreTakeRecursive ( scopeMutex , portMAX_DELAY );
			lastWritingIndex = writingIndex;
			writingIndex = scope.oneChanSamples - DMA_GetCurrDataCounter(scope.triggerChannel);
			actualIndex = (scope.oneChanSamples + writingIndex - 1) % scope.oneChanSamples;
			uint16_t passMsg;

			//wait for right level before finding trigger (lower level then trigger level for rising edge, higher level for falling edge)
			if(scope.state == SCOPE_SAMPLING_WAITING){ 
				if(scope.settings.adcRes<=8){
					data=*(scope.pChanMem[scope.triggerChannel-1]+actualIndex/2);
					data = data & 0x00ff;
				}else{
					data=*(scope.pChanMem[scope.triggerChannel-1]+actualIndex);
				}

				updateTrigger();
				samplesTaken += samplesPassed(writingIndex,lastWritingIndex);	
				//start finding right level before trigger (cannot start to find it earlier because pretrigger was not taken yet)
				if (samplesTaken > samplesToStart){
					if((scope.settings.triggerEdge == EDGE_RISING && data + NOISE_REDUCTION < triggerLevel) 
							|| (scope.settings.triggerEdge == EDGE_FALLING && data - NOISE_REDUCTION > triggerLevel) ){ //skip waiting for trigger in case of TRIG_AUTO
						autoTrigged=0;
						scope.state = SCOPE_SAMPLING_TRIGGER_WAIT;
						passMsg = MSG_SCOPE_SMPL_STARTED;
						xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
					}else if((scope.settings.triggerMode == TRIG_AUTO && samplesTaken > (scope.settings.samplesToSend * AUTO_TRIG_WAIT_NORMAL))
							|| (scope.settings.triggerMode == TRIG_AUTO_FAST && samplesTaken > (scope.settings.samplesToSend * AUTO_TRIG_WAIT_FAST))  ){ //skip waiting for trigger in case of TRIG_AUTO
						autoTrigged=1;
						scope.state = SCOPE_SAMPLING_TRIGGER_WAIT;
						passMsg = MSG_SCOPE_SMPL_STARTED;
						xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
					}
				}
				//looking for trigger
			}else if(scope.state == SCOPE_SAMPLING_TRIGGER_WAIT){
				samplesTaken += samplesPassed(writingIndex,lastWritingIndex);	
				if(scope.settings.adcRes<=8){
					data=*(scope.pChanMem[scope.triggerChannel-1]+actualIndex/2);
					data = data & 0x00ff;
				}else{
					data=*(scope.pChanMem[scope.triggerChannel-1]+actualIndex);
				}
				updateTrigger();
				if((scope.settings.triggerEdge == EDGE_RISING && data > triggerLevel) 
						|| (scope.settings.triggerEdge == EDGE_FALLING && data < triggerLevel) ){
					totalSmpTaken = samplesTaken;
					samplesTaken = 0;
					scope.state = SCOPE_SAMPLING;
					triggerIndex = actualIndex;
					triggerShift = samplesPassed(writingIndex, lastWritingIndex);
					passMsg = MSG_SCOPE_TRIGGER;
					xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
				}else if((scope.settings.triggerMode == TRIG_AUTO && samplesTaken > (scope.settings.samplesToSend * AUTO_TRIG_WAIT_NORMAL))
						|| (scope.settings.triggerMode == TRIG_AUTO_FAST && samplesTaken > (scope.settings.samplesToSend * AUTO_TRIG_WAIT_FAST)) ){
					totalSmpTaken = samplesTaken;
					samplesTaken = 0;
					scope.state = SCOPE_SAMPLING;
					triggerIndex = actualIndex;
					passMsg = MSG_SCOPE_TRIGGER;
					xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
					autoTrigged=1;
				}
				//sampling after trigger event
			}else if(scope.state == SCOPE_SAMPLING){
				samplesTaken += samplesPassed(writingIndex, lastWritingIndex);	


				//sampling is done
				if(samplesTaken >= samplesToStop){
					samplingDisable();

					//finding exact trigger position because not every samples are chcecked 
					if (autoTrigged==0){//scope.settings.triggerMode != TRIG_AUTO && scope.settings.triggerMode != TRIG_AUTO_FAST){
						triggerShift += samplesTaken-samplesToStop + samplesPassed(writingIndex, lastWritingIndex) - (scope.oneChanSamples-scope.settings.samplesToSend);
						if(scope.settings.adcRes>8){
							if(scope.settings.triggerEdge == EDGE_RISING){
								while(*(scope.pChanMem[scope.triggerChannel-1]+triggerIndex) > triggerLevel && triggerShift < SCOPE_BUFFER_MARGIN){
									triggerIndex--;
									triggerShift++;
								}
							}else{
								while(*(scope.pChanMem[scope.triggerChannel-1]+triggerIndex) < triggerLevel && triggerShift < SCOPE_BUFFER_MARGIN){
									triggerIndex--;
									triggerShift++;
								}
							}
						}else{							
							if(scope.settings.triggerEdge == EDGE_RISING){
								while(*((uint8_t *)scope.pChanMem[scope.triggerChannel-1]+triggerIndex) > triggerLevel && triggerShift < SCOPE_BUFFER_MARGIN){
									triggerIndex--;
									triggerShift++;
								}
							}else{
								while(*((uint8_t *)scope.pChanMem[scope.triggerChannel-1]+triggerIndex) < triggerLevel && triggerShift < SCOPE_BUFFER_MARGIN){
									triggerIndex--;
									triggerShift++;
								}
							}
						}
					}

					scope.triggerIndex = triggerIndex;
					scope.state = SCOPE_DATA_SENDING;

					samplesTaken = 0;
					totalSmpTaken = 0;
					passMsg = MSG_SCOPE_DATA_READY;
					xQueueSendToBack (messageQueue, &passMsg, portMAX_DELAY);
				}
			}
			xSemaphoreGiveRecursive(scopeMutex);
		}else{
			taskYIELD();
		}
	}
}

/**
 * @brief 	Returns number of samples between indexes.
 * @param  actual index, last index
 * @retval None
 */
uint16_t samplesPassed(uint16_t index, uint16_t lastIndex){
	uint16_t result=0;
	if(index < lastIndex){
		result = index + scope.oneChanSamples - lastIndex;
	}else{
		result = index - lastIndex;
	}
	return result;
}

/**
 * @brief 	Checks if scope settings doesn't exceed memory
 * @param  None
 * @retval 1=err / 0=ok
 */
uint8_t validateBuffUsage(){
	uint8_t result=1;
	uint32_t data_len=scope.settings.samplesToSend;
	if(scope.settings.adcRes>8){
		data_len=data_len*2;
	}
	data_len=data_len*scope.numOfChannles;
	if(data_len<=MAX_SCOPE_BUFF_SIZE){
		result=0;
	}
	return result;
}

/**
 * @brief  Oscilloscope initialisation.
 *					Configurstion of DMA and TIM time base
 * @param  None
 * @retval None
 */
void scopeInit(void){
	writingIndex = 0;
	uint32_t realfreq=0;

	ADC_DMA_Stop();

	if(scope.settings.AdvMode == SCOPE_INTERLEAVE_MODE){
		scope.settings.interleaved=2;
		TIM_Reconfig_scope(scope.settings.samplingFrequency/2,&realfreq);
		ADC_set_sampling_time(realfreq);
		scopeInitADCMode(scope.settings.AdvMode);

		for(uint8_t i = 0;i<MAX_ADC_CHANNELS/2;i++){
			if(scope.numOfChannles>i){
				ADC_DMA_Reconfig_Interleave(i,(uint32_t *)scope.pChanMem[i], scope.oneChanSamples/2);
			}
		}
		scope.settings.ADCSamplingFreq=realfreq;


	}else if(scope.settings.AdvMode == SCOPE_MULTI_MODE){
		scope.settings.ADCmux=2;


	}else { //normal sampling
		scope.settings.AdvMode = SCOPE_NORMAL_MODE;
		scope.settings.interleaved=1;
		TIM_Reconfig_scope(scope.settings.samplingFrequency,&realfreq);
		ADC_set_sampling_time(realfreq);
		scopeInitADCMode(scope.settings.AdvMode);

		for(uint8_t i = 0;i<MAX_ADC_CHANNELS;i++){
			if(scope.numOfChannles>i){
				ADC_DMA_Reconfig(i,(uint32_t *)scope.pChanMem[i], scope.oneChanSamples);
			}
		}
		scope.settings.ADCSamplingFreq=realfreq;
	}
}

/**
 * @brief  Update trigger level and pretriger values
 *					Ccan be changed on the fly
 * @param  None
 * @retval None
 */
void updateTrigger(void){
	triggerLevel = (scope.settings.triggerLevel * scope.settings.adcLevels) >> 16;
	samplesToStop = ((scope.settings.samplesToSend * (UINT16_MAX - scope.settings.pretrigger)) >> 16)+1;
	samplesToStart = (scope.settings.samplesToSend * (scope.settings.pretrigger)) >> 16;
}

/**
 * @brief  Oscilloscope set Default values
 * @param  None
 * @retval None
 */
void scopeSetDefault(void){
	scope.bufferMemory = scopeBuffer;
	scope.settings.samplingFrequency = SCOPE_DEFAULT_SAMPLING_FREQ;
	scope.settings.triggerEdge = SCOPE_DEFAULT_TRIG_EDGE;
	scope.settings.triggerMode = SCOPE_DEFAULT_TRIGGER;
	scope.settings.triggerLevel = SCOPE_DEFAULT_TRIGGER_LEVEL;
	scope.settings.pretrigger = SCOPE_DEFAULT_PRETRIGGER;
	scope.settings.adcRes = SCOPE_DEFAULT_ADC_RES;
	scope.settings.adcLevels=pow(2,SCOPE_DEFAULT_ADC_RES);
	scope.settings.samplesToSend = SCOPE_DEFAULT_DATA_LEN;
	scope.pChanMem[0] = (uint16_t*)scopeBuffer;
	scope.oneChanMemSize = MAX_SCOPE_BUFF_SIZE+SCOPE_BUFFER_MARGIN;
	if(scope.settings.adcRes>8){
		scope.oneChanSamples = scope.oneChanMemSize/2;
	}else{
		scope.oneChanSamples = scope.oneChanMemSize;
	}
	scope.numOfChannles = 1;
	scope.triggerChannel = 1;
	scopeSetADCInputChannelDefault();
}

/**
 * @brief  Getter function number of scope channels being used
 * @param  None
 * @retval Number of channels
 */
uint8_t GetNumOfChannels (void){
	return scope.numOfChannles;
}

/**
 * @brief  Getter function of pointer for data buffer.
 * @param  None
 * @retval Pointer to buffer
 */
uint16_t *getDataPointer(uint8_t chan){
	return scope.pChanMem[chan];
}

/**
 * @brief  Getter function of one channel memory size.
 * @param  None
 * @retval One channel memory size
 */
uint32_t getOneChanMemSize(){
	return scope.oneChanMemSize;
}

/**
 * @brief  Getter function of one channel samples.
 * @param  None
 * @retval One channel number of samples
 */
uint32_t getOneChanMemSamples(){
	return scope.oneChanSamples;
}

/**
 * @brief  Getter function of trigger index.
 * @param  None
 * @retval Pointer to sample where trigger occured
 */
uint32_t getTriggerIndex(void){
	return triggerIndex;
}

/**
 * @brief  Getter function of data length.
 * @param  None
 * @retval Data length
 */
uint32_t getSamples(void){
	return scope.settings.samplesToSend;
}

/**
 * @brief  Getter function of ADC resolution.
 * @param  None
 * @retval ADC resolution
 */
uint16_t getADCRes(void){
	return scope.settings.adcRes;
}

/**
 * @brief  Getter function of pretrigger.
 * @param  None
 * @retval Pretrigger value
 */
uint16_t getPretrigger(void){
	return scope.settings.pretrigger;
}

/**
 * @brief  Getter for oscilloscope state.
 * @param  None
 * @retval Scope state
 */
scopeState getScopeState(void){
	return scope.state;
}

/**
 * @brief  Setter for trigger mode
 * @param  Scope Trigger mode
 * @retval None
 */
void scopeSetTriggerMode(scopeTriggerMode mode){
	xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
	scope.settings.triggerMode = mode;
	xSemaphoreGiveRecursive(scopeMutex);
}

/**
 * @brief  Setter for trigger edge
 * @param  Scope Trigger edge
 * @retval None
 */
void scopeSetTriggerEdge(scopeTriggerEdge edge){
	xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
	scope.settings.triggerEdge = edge;
	xSemaphoreGiveRecursive(scopeMutex);
	uint16_t passMsg = MSG_INVALIDATE;
	xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY); //cannot change this property on the on the fly (scope must re-init)
}

/**
 * @brief  Setter for ADC resolution
 * @param  ADC resolution 2^N where N is number of bits
 * @retval success=0/error
 */
uint8_t scopeSetDataDepth(uint16_t res){
	uint8_t result=BUFFER_SIZE_ERR;
	uint8_t resTmp=scope.settings.adcRes;
	xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
	scope.settings.adcRes = res;
	if(validateBuffUsage()){
		scope.settings.adcRes = resTmp;
	}else{
		scope.settings.adcLevels=pow(2,scope.settings.adcRes);
		if(scope.settings.adcRes>8){
			scope.oneChanSamples=scope.oneChanMemSize/2;
		}else{
			scope.oneChanSamples=scope.oneChanMemSize;
		}
		adcSetResolution(res);
		scopeInitADCMode(scope.settings.AdvMode);
		result = scopeSetSamplingFreq(scope.settings.samplingFrequency);
	}
	xSemaphoreGiveRecursive(scopeMutex);
	uint16_t passMsg = MSG_INVALIDATE;
	xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);

	return result;
}

/**
 * @brief  Setter for sampling frequency
 * @param  Samples per second
 * @retval success=0/error
 */
uint8_t scopeSetSamplingFreq(uint32_t freq){
	uint8_t result=SCOPE_INVALID_SAMPLING_FREQ;
	xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);

	if (freq <= getMaxScopeSamplingFreq(scope.settings.adcRes)){
		scope.settings.samplingFrequency = freq;
		scope.settings.AdvMode = SCOPE_NORMAL_MODE;
		result=0;
	}else{
		if(scope.numOfChannles<=MAX_INTERLEAVED_CHANNELS){
			scope.settings.samplingFrequency=getMaxScopeSamplingFreqInterleaved(scope.settings.adcRes);
			scope.settings.AdvMode = SCOPE_INTERLEAVE_MODE;
		}else{
			scope.settings.samplingFrequency=getMaxScopeSamplingFreq(scope.settings.adcRes);
			scope.settings.AdvMode = SCOPE_NORMAL_MODE;
		}
	}

	xSemaphoreGiveRecursive(scopeMutex);
	uint16_t passMsg = MSG_INVALIDATE;
	xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);

	return result;
}

/**
 * @brief  Setter for trigger level
 * @param  Signal level to trigger (0xFFFF is 100%)
 * @retval None
 */
void scopeSetTrigLevel(uint16_t level){
	xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
	scope.settings.triggerLevel = level;
	xSemaphoreGiveRecursive(scopeMutex);
}

/**
 * @brief  Setter for pretrigger
 * @param  Fraction of buffer before trigger event (0xFFFF is 100%)
 * @retval None
 */
void scopeSetPretrigger(uint16_t pretrig){
	xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
	scope.settings.pretrigger = pretrig;
	xSemaphoreGiveRecursive(scopeMutex);
}

/**
 * @brief  Setter for number of samples
 * @param  Numbr of samples
 * @retval success=0/error
 */
uint8_t scopeSetNumOfSamples(uint32_t smp){
	uint8_t result=BUFFER_SIZE_ERR;
	//uint32_t smpTmp=scope.settings.samplesToSend;
	xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
	if(smp<scope.oneChanSamples){
		scope.settings.samplesToSend = smp;
		result=0;
	}
	xSemaphoreGiveRecursive(scopeMutex);
	uint16_t passMsg = MSG_INVALIDATE;
	xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);
	return result;
}

/**
 * @brief  Setter for number of channels
 * @param  Number of channels
 * @retval success=0/error
 */
uint8_t scopeSetNumOfChannels(uint8_t chan){
	uint8_t result=BUFFER_SIZE_ERR;
	uint8_t chanTmp=scope.numOfChannles;

	if(chan<=MAX_ADC_CHANNELS){
		xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
		 //workaround to exit interleave mode when more channels needed
		if(chan>1 && scope.settings.AdvMode!=SCOPE_NORMAL_MODE){
			scope.settings.AdvMode=SCOPE_NORMAL_MODE;
			scope.settings.samplingFrequency=getMaxScopeSamplingFreq(scope.settings.adcRes);
		}
		//workaround to enter interleave mode again when one channel selected
		if(chan==1 && scope.settings.samplingFrequency==getMaxScopeSamplingFreq(scope.settings.adcRes)){
			scope.settings.AdvMode=SCOPE_INTERLEAVE_MODE;
			scope.settings.samplingFrequency=getMaxScopeSamplingFreqInterleaved(scope.settings.adcRes);
		}

		scope.numOfChannles=chan;
		if(validateBuffUsage()){
			scope.numOfChannles = chanTmp;
		}else{
			scope.oneChanMemSize=MAX_SCOPE_BUFF_SIZE/chan+SCOPE_BUFFER_MARGIN-(MAX_SCOPE_BUFF_SIZE/chan+SCOPE_BUFFER_MARGIN)%2;
			if(scope.settings.adcRes>8){
				scope.oneChanSamples=scope.oneChanMemSize/2;
			}else{
				scope.oneChanSamples=scope.oneChanMemSize;
			}
			for(uint8_t i=0;i<chan;i++){
				scope.pChanMem[i]=(uint16_t *)(&scopeBuffer[i*scope.oneChanMemSize]+(uint32_t)(&scopeBuffer[i*scope.oneChanMemSize])%2);
			}
			result=0;
		}

		if(chan < scope.triggerChannel && result == 0){
			scopeSetTrigChannel(1);
			result = SCOPE_TRIGGER_FORCED_TO_CH1;
		}
		xSemaphoreGiveRecursive(scopeMutex);
		uint16_t passMsg = MSG_INVALIDATE;
		xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);
	}else{
		result = SCOPE_TOO_MANY_CHANNELS;
	}
	return result;
}

/**
 * @brief  Setter for trigger channel
 * @param  Trigger channel
 * @retval success=0/error
 */
uint8_t scopeSetTrigChannel(uint8_t chan){
	uint8_t result=SCOPE_INVALID_TRIGGER_CHANNEL;
	if(chan<=MAX_ADC_CHANNELS && chan <=scope.numOfChannles){
		xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
		scope.triggerChannel=chan;
		result=0;
		xSemaphoreGiveRecursive(scopeMutex);
		uint16_t passMsg = MSG_INVALIDATE;
		xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);
	}
	return result;
}

uint32_t scopeGetRealSmplFreq(){
	return scope.settings.ADCSamplingFreq * scope.settings.interleaved;
}


/**
 * @brief  ADC channel selections
 *					Selects ADC channel to be sampled. Possible selection defined in mcu_config.h
 * @param  ADC number, Channel number
 * @retval success=0/error
 */
uint8_t scopeSetADCInputChannel(uint8_t adc, uint8_t chann){
	uint8_t result = SCOPE_INVALID_ADC_CHANNEL;
	if(adc < MAX_ADC_CHANNELS && chann < NUM_OF_ANALOG_INPUTS[adc]){
		xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
		scope.adcChannel[adc] = chann;

		adcSetInputChannel(adc, chann);
		scopeInitADCMode(scope.settings.AdvMode);

		result = 0;
		xSemaphoreGiveRecursive(scopeMutex);
		uint16_t passMsg = MSG_INVALIDATE;
		xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);
	}
	return result;
}

/**
 * @brief  Set ADC channels to default input
 * @param  ADC number, Channel number
 * @retval success=0/error
 */
uint8_t scopeSetADCInputChannelDefault(){
	uint8_t result = SCOPE_INVALID_ADC_CHANNEL;
	xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
	for(uint8_t i = 0;i<MAX_ADC_CHANNELS;i++){
		scope.adcChannel[i] = ANALOG_DEFAULT_INPUTS[i];

		adcSetInputChannel(i, ANALOG_DEFAULT_INPUTS[i]);
		scopeInitADCMode(scope.settings.AdvMode);

		result = 0;
	}
	xSemaphoreGiveRecursive(scopeMutex);
	uint16_t passMsg = MSG_INVALIDATE;
	xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);
	return result;
}

/**
 * @brief  Set all ADC channels to sense intenral Vref value
 * @param  None
 * @retval success=0/error
 */
uint8_t scopeSetADCInputChannelVref(){
	uint8_t result = SCOPE_INVALID_ADC_CHANNEL;
	xSemaphoreTakeRecursive(scopeMutex, portMAX_DELAY);
	for(uint8_t i = 0;i<MAX_ADC_CHANNELS;i++){
		scope.adcChannel[i] = ANALOG_VREF_INPUTS[i];

		adcSetInputChannel(i, ANALOG_VREF_INPUTS[i]);
		scopeInitADCMode(scope.settings.AdvMode);

		result = 0;
	}
	xSemaphoreGiveRecursive(scopeMutex);
	uint16_t passMsg = MSG_INVALIDATE;
	xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);
	return result;
}


uint8_t scopeInitADCMode(scopeMode mode){
	switch (mode){
	case SCOPE_NORMAL_MODE:
		ADCInitNormalMode();
		break;
	case SCOPE_INTERLEAVE_MODE:
		ADCInitInterleavedMode();
		break;
	case SCOPE_MULTI_MODE:
		ADCInitMultiMode();
		break;
	}
	return 0;
}


/**
 * @brief  getter of pointer to dafinition of input voltage ranges
 * @param  uint8 pointer - Number of ranges
 * @retval uint16 pointer - Pointer to ranges
 */
const int16_t* scopeGetRanges(uint8_t * len){
#ifdef USE_SHIELD
	if(isScopeShieldConnected()){
		*len=sizeof(SHIELD_RANGES);
		return SHIELD_RANGES;
	}else{
		*len=sizeof(RANGES);
		return RANGES;
	}
#else
	*len=sizeof(RANGES);
	return RANGES;	
#endif


}

/**
 * @brief  Restart scope sampling
 * @param  None
 * @retval None
 */
void scopeRestart(void){
	uint16_t passMsg = MSG_SCOPE_RESTART;
	xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);
}

/**
 * @brief  Start scope sampling
 * @param  None
 * @retval None
 */
void scopeStart(void){
	uint16_t passMsg = MSG_SCOPE_START;
	xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);
}

/**
 * @brief  Stop scope sampling
 * @param  None
 * @retval None
 */
void scopeStop(void){
	uint16_t passMsg = MSG_SCOPE_STOP;
	xQueueSendToBack(scopeMessageQueue, &passMsg, portMAX_DELAY);
}

/**
 * @}
 */

/**
 * @}
 */

#endif //USE_SCOPE



