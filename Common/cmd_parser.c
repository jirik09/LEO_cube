/**
 *****************************************************************************
 * @file    cmd_parser.c
 * @author  Y3288231
 * @date    Dec 16, 2014
 * @brief   This file contains functions for parsing commands
 *****************************************************************************
 */

// Includes ===================================================================
#include <string.h>
#include "cmsis_os.h"
#include "mcu_config.h"
#include "cmd_parser.h"
#include "commands.h"
#include "comms.h"
#include "scope.h"
#include "generator.h"
#include "adc.h"
#include "clock.h"
#include "counter.h"
#include "sync_pwm.h"
#include "logic_analyzer.h"
#include "messages.h"


/** @defgroup Parser Parser
 * @{
 */

/** @defgroup Parser_Private_Variables Parser Private Variables
 * @{
 */
xQueueHandle cmdParserMessageQueue;

/**
 * @}
 */

/** @defgroup Parser_Private_Prototypes Parser Private Function Prototypes
 * @{
 */
command parseSystemCmd(void);
command parseCommsCmd(void);
command parseScopeCmd(void);
command parseSyncPwmCmd(void);
command parseLogAnlysCmd(void);
command giveNextCmd(void);
command parseCounterCmd(void);

command parseGeneratorSignalCmd(void);
command parseGeneratorPwmCmd(void);
command parseGeneratorPatternCmd(void);
command parseVoltageSourceCmd(void);

command parseGenCommonCmd(command cmdIn);

double getDouble(command cmd);
void printErrResponse(command cmd);
/**
 * @}
 */

/** @defgroup Parser_Function_Definitions Parser Function Definitions
 * @{
 */
/**
 * @brief  Command parser task function.
 * @param  Task handler, parameters pointer
 * @retval None
 */
void CmdParserTask(void const *argument){

	//CASSERT(sizeof(IDN_STRING)<30); //IDN string is too long

	uint16_t message = 0xFFFF;
	cmdParserMessageQueue = xQueueCreate(32, sizeof(message)/sizeof(uint8_t));

	uint8_t cmdIn[5] = {0};
	uint8_t chr;
	uint8_t byteRead;
	command tempCmd;
	while(1){
		xQueueReceive(cmdParserMessageQueue, &message, portMAX_DELAY);

		if(message == MSG_COMMS_TRY_PARSE){//parsing of command
			do{
				cmdIn[0] = cmdIn[1];
				cmdIn[1] = cmdIn[2];
				cmdIn[2] = cmdIn[3];
				cmdIn[3] = chr;
				byteRead = commBufferReadByte(&chr);
			}while(byteRead==0 && chr != ':' && chr != ';');

			uint16_t passMsg;

			if(byteRead==0){
				switch (BUILD_CMD(cmdIn)){
				case CMD_IDN: //send IDN
					passMsg = MSG_DEVICE_IDN;
					xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
					break;
				case CMD_RESET_DEVICE:
					resetDevice();
					break;
				case CMD_IS_SHIELD:
					passMsg = MSG_SHIELD_AVAIL;
					xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
					break;
				case CMD_SYSTEM:
					tempCmd = parseSystemCmd();
					printErrResponse(tempCmd);
					break;
#ifdef USE_SCOPE
				case CMD_SCOPE: //parse scope command
					tempCmd = parseScopeCmd();
					printErrResponse(tempCmd);
					break;
#endif //USE_SCOPE

#ifdef USE_GEN_SIGNAL
				case CMD_GEN_SIGNAL: //parse generator command
					tempCmd = parseGeneratorSignalCmd();
					printErrResponse(tempCmd);
					break;
				case CMD_VOLATGE_SOURCE: //parse voltagesource command
					tempCmd = parseVoltageSourceCmd();
					printErrResponse(tempCmd);
					break;
#endif //USE_GEN_SIGNAL

#ifdef USE_GEN_PWM
				case CMD_GEN_PWM: //parse generator command
					tempCmd = parseGeneratorPwmCmd();
					printErrResponse(tempCmd);
					break;
#endif //USE_GEN_PWM

#ifdef USE_GEN_PATTERN
				case CMD_GEN_PATTERN: //parse generator command
					tempCmd = parseGeneratorPatternCmd();
					printErrResponse(tempCmd);
					break;
#endif //USE_GEN_PATTERN

#ifdef USE_COUNTER
				case CMD_COUNTER: //parse generator command
					tempCmd = parseCounterCmd();
					printErrResponse(tempCmd);
					break;
#endif //USE_COUNTER

#ifdef USE_SYNC_PWM
				case CMD_SYNC_PWM: //parse sync PWM command
					tempCmd = parseSyncPwmCmd();
					printErrResponse(tempCmd);
					break;
#endif //USE_SYNC_PWM

#ifdef USE_LOG_ANLYS
				case CMD_LOG_ANLYS: //parse logic analyzer command
					tempCmd = parseLogAnlysCmd();
					printErrResponse(tempCmd);
					break;
#endif //USE_LOG_ANLYS

				default:
					xQueueSendToBack(messageQueue, UNSUPORTED_FUNCTION_ERR_STR, portMAX_DELAY);
					while(commBufferReadByte(&chr)==0 && chr!=';');
				}	
			}
		}/*if(getBytesAvailable()>4){  //if there are still some bytes to read in the buffer then read invoke the parsing
			uint16_t passMsg = MSG_COMMS_TRY_PARSE;
			xQueueSendToBack(cmdParserMessageQueue, &passMsg, portMAX_DELAY);
		}*/
	}
}


/**
 * @brief  System command parse function
 * @param  None
 * @retval Command ACK or ERR
 */
command parseSystemCmd(void){
	command cmdIn=CMD_ERR;
	uint8_t error=0;
	//try to parse command while buffer is not empty 

	cmdIn = giveNextCmd();
	switch(cmdIn){
	uint16_t passMsg;
	case CMD_GET_CONFIG:
		passMsg = MSG_SYSTEM_SPEC;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;
	case CMD_END:break;
	default:
		error = SYSTEM_INVALID_FEATURE;
		cmdIn = CMD_ERR;
		break;
	}
	if(error>0){
		cmdIn=error;
	}else{
		cmdIn=CMD_END;
	}
	return cmdIn;
}


/**
 * @brief  Counter command parse function
 * @param  None
 * @retval Command ACK or ERR
 */
#ifdef USE_COUNTER
command parseCounterCmd(void)
{
	command cmdIn=CMD_ERR; 
	uint8_t error=0;
	uint16_t passMsg;

	cmdIn = giveNextCmd();
	switch(cmdIn){
	case CMD_CNT_MODE:
		cmdIn = giveNextCmd();
		if(isCounterMode(cmdIn)){
			if(cmdIn == CMD_MODE_ETR){
				counterSetMode(ETR);
			}else if(cmdIn == CMD_MODE_IC){
				counterSetMode(IC);
			}else if(cmdIn == CMD_MODE_RAT){
				counterSetMode(REF);
			}else if(cmdIn == CMD_MODE_TI){
				counterSetMode(TI);
			}
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_ETR_QUANTITY:
		cmdIn = giveNextCmd();
		if(isCounterQuantity(cmdIn)){
			if(cmdIn == CMD_CNT_QUANTITY_FREQUENCY){
				error = counterSetQuantity(QUANTITY_FREQUENCY);
			}else if(cmdIn == CMD_CNT_QUANTITY_PERIOD){
				error = counterSetQuantity(QUANTITY_PERIOD);
			}
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_IC_CHAN1_QUANTITY:
		cmdIn = giveNextCmd();
		if(isCounterQuantity(cmdIn)){
			if(cmdIn == CMD_CNT_QUANTITY_FREQUENCY){
				error = counterSetIc1Quantity(QUANTITY_FREQUENCY);
			}else if(cmdIn == CMD_CNT_QUANTITY_PERIOD){
				error = counterSetIc1Quantity(QUANTITY_PERIOD);
			}
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_IC_CHAN2_QUANTITY:
		cmdIn = giveNextCmd();
		if(isCounterQuantity(cmdIn)){
			if(cmdIn == CMD_CNT_QUANTITY_FREQUENCY){
				error = counterSetIc2Quantity(QUANTITY_FREQUENCY);
			}else if(cmdIn == CMD_CNT_QUANTITY_PERIOD){
				error = counterSetIc2Quantity(QUANTITY_PERIOD);
			}
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_GATE:
		cmdIn = giveNextCmd();
		if(isCounterEtrGate(cmdIn)){
			if(cmdIn == CMD_GATE_100m){
				error = counterSetEtrGate(100);
			}else if(cmdIn == CMD_GATE_500m){
				error = counterSetEtrGate(500);
			}else if(cmdIn == CMD_GATE_1s){
				error = counterSetEtrGate(1000);
			}else if(cmdIn == CMD_GATE_5s){
				error = counterSetEtrGate(5000);
			}else if(cmdIn == CMD_GATE_10s){
				error = counterSetEtrGate(10000);
			}					
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_EVENT:
		cmdIn = giveNextCmd();
		if(isCounterIcTiEvent(cmdIn)){
			if(cmdIn == CMD_EVENT_RF1){
				error = counterSetIcTi1_RisingFalling();
			}else if(cmdIn == CMD_EVENT_RF2){
				error = counterSetIcTi2_RisingFalling();
			}else if(cmdIn == CMD_EVENT_RO1){
				error = counterSetIcTi1_Rising();
			}else if(cmdIn == CMD_EVENT_RO2){
				error = counterSetIcTi2_Rising();
			}else if(cmdIn == CMD_EVENT_FO1){
				error = counterSetIcTi1_Falling();
			}else if(cmdIn == CMD_EVENT_FO2){
				error = counterSetIcTi2_Falling();
			}else if(cmdIn == CMD_EVENT_SEQ_AB){
				error = counterSetTiSequence_AB();
			}else if(cmdIn == CMD_EVENT_SEQ_BA){
				error = counterSetTiSequence_BA();
			}
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_DUTY_CYCLE:
		cmdIn = giveNextCmd();
		if(isCounterIcDutyCycle(cmdIn)){
			if(cmdIn == CMD_DUTY_CYCLE_ENABLE_CH1){
				error = counterIc1DutyCycleEnable();
			}else if(cmdIn == CMD_DUTY_CYCLE_ENABLE_CH2){
				error = counterIc2DutyCycleEnable();
			}else if(cmdIn == CMD_DUTY_CYCLE_DISABLE){
				error = counterIcDutyCycleDisable();
			}
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_TI_MODE:
		cmdIn = giveNextCmd();
		if(isCounterTiMode(cmdIn)){
			if(cmdIn == CMD_MODE_EVENT_SEQUENCE_DEP){
				error = counterSetTiMode_Dependent();
			}else if(cmdIn == CMD_MODE_EVENT_SEQUENCE_INDEP){
				error = counterSetTiMode_Independent();
			}
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_PRESC1:
		cmdIn = giveNextCmd();
		if(isCounterIcPresc(cmdIn)){
			if(cmdIn == CMD_PRESC_1x){
				error = counterSetIc1Prescaler(1);
			}else if(cmdIn == CMD_PRESC_2x){
				error = counterSetIc1Prescaler(2);
			}else if(cmdIn == CMD_PRESC_4x){
				error = counterSetIc1Prescaler(4);
			}else if(cmdIn == CMD_PRESC_8x){
				error = counterSetIc1Prescaler(8);
			}
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_PRESC2:
		cmdIn = giveNextCmd();
		if(isCounterIcPresc(cmdIn)){
			if(cmdIn == CMD_PRESC_1x){
				error = counterSetIc2Prescaler(1);
			}else if(cmdIn == CMD_PRESC_2x){
				error = counterSetIc2Prescaler(2);
			}else if(cmdIn == CMD_PRESC_4x){
				error = counterSetIc2Prescaler(4);
			}else if(cmdIn == CMD_PRESC_8x){
				error = counterSetIc2Prescaler(8);
			}else{
				cmdIn = CMD_ERR;
				error = COUNTER_INVALID_FEATURE_PARAM;
			}					
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_SAMPLE_COUNT1:
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			error = counterSetIc1SampleCount((uint16_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_SAMPLE_COUNT2:
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			error = counterSetIc2SampleCount((uint16_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_REF_SAMPLE_COUNT:
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			error = counterSetRefSampleCount((uint32_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
		/* TI set timout */
	case CMD_CNT_TIMEOUT_TIM:
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			error = counterSetTiTimeout((uint32_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = COUNTER_INVALID_FEATURE_PARAM;
		}
		break;
	case CMD_CNT_PAUSE:
		counterPause();
		break;
	case CMD_CNT_UNPAUSE:
		counterUnpause();
		break;
	case CMD_CNT_START:
		counterSendStart();
		break;
	case CMD_CNT_STOP:
		counterSendStop();
		break;
	case CMD_CNT_DEINIT:
		counterDeinit();
		break;
	case CMD_GET_CONFIG:
		passMsg = MSG_CNT_CONFIG;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;
	}	

	cmdIn = (error > 0) ? error : CMD_END;
	return cmdIn;
}
#endif // USE_COUNTER


/**
 * @brief  Scope command parse function
 * @param  None
 * @retval Command ACK or ERR
 */
#ifdef USE_SCOPE
command parseScopeCmd(void){
	command cmdIn=CMD_ERR;
	uint8_t error=0;
	uint16_t passMsg;
	//try to parse command while buffer is not empty 

	cmdIn = giveNextCmd();
	switch(cmdIn){
	case CMD_SCOPE_TRIG_MODE://set trigger mode
		cmdIn = giveNextCmd();
		if(isScopeTrigMode(cmdIn)){
			if(cmdIn == CMD_MODE_NORMAL){
				scopeSetTriggerMode(TRIG_NORMAL);
			}else if(cmdIn == CMD_MODE_AUTO){
				scopeSetTriggerMode(TRIG_AUTO);
			}else if(cmdIn == CMD_MODE_AUTO_FAST){
				scopeSetTriggerMode(TRIG_AUTO_FAST);
			}else if(cmdIn == CMD_MODE_SINGLE){
				scopeSetTriggerMode(TRIG_SINGLE);
			}
		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_TRIG_EDGE: //set trigger edge
		cmdIn = giveNextCmd();
		if(isScopeTrigEdge(cmdIn)){
			if(cmdIn == CMD_EDGE_RISING){
				scopeSetTriggerEdge(EDGE_RISING);
			}else if(cmdIn == CMD_EDGE_FALLING){
				scopeSetTriggerEdge(EDGE_FALLING);
			}
		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_CHANNELS: //set number of channels
		cmdIn = giveNextCmd();
		if(isChannel(cmdIn)){
			if(cmdIn == CMD_CHANNELS_1){
				error=scopeSetNumOfChannels(1);
			}else if(cmdIn == CMD_CHANNELS_2){
				error=scopeSetNumOfChannels(2);
			}else if(cmdIn == CMD_CHANNELS_3){
				error=scopeSetNumOfChannels(3);
			}else if(cmdIn == CMD_CHANNELS_4){
				error=scopeSetNumOfChannels(4);
			}
		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_TRIG_CHANNEL: //set trigger channel
		cmdIn = giveNextCmd();
		if(isChannel(cmdIn)){
			if(cmdIn == CMD_CHANNELS_1){
				error=scopeSetTrigChannel(1);
			}else if(cmdIn == CMD_CHANNELS_2){
				error=scopeSetTrigChannel(2);
			}else if(cmdIn == CMD_CHANNELS_3){
				error=scopeSetTrigChannel(3);
			}else if(cmdIn == CMD_CHANNELS_4){
				error=scopeSetTrigChannel(4);
			}
		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_ADC_CHANNEL_SET: //set actual ADC channel
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			error=scopeSetADCInputChannel((uint8_t)(cmdIn>>8),(uint8_t)(cmdIn));
			if(error!=0){
				cmdIn = CMD_ERR;
			}
		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_ADC_CHANNEL_SET_DEFAULT: //set actual ADC channel for default
		error=scopeSetADCInputChannelDefault();
		if(error!=0){
			cmdIn = CMD_ERR;
		}

		break;

	case CMD_SCOPE_ADC_CHANNEL_SET_VREF: //set actual ADC channel for Vref
		error=scopeSetADCInputChannelVref();
		if(error!=0){
			cmdIn = CMD_ERR;
		}
		break;


	case CMD_SCOPE_DATA_DEPTH: //set data bit depth
		cmdIn = giveNextCmd();
		if(isScopeDataDepth(cmdIn)){
			if(cmdIn == CMD_DATA_DEPTH_12B){
				error=scopeSetDataDepth(12);
			}else if(cmdIn == CMD_DATA_DEPTH_10B){
				error=scopeSetDataDepth(10);
			}else if(cmdIn == CMD_DATA_DEPTH_8B){
				error=scopeSetDataDepth(8);
			}else if(cmdIn == CMD_DATA_DEPTH_6B){
				error=scopeSetDataDepth(6);
			}
		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_SAMPLING_FREQ: //set sampling frequency
		cmdIn = giveNextCmd();

		if(isScopeFreq(cmdIn)){
			if(cmdIn == CMD_FREQ_1K){
				error=scopeSetSamplingFreq(1000);
			}else if(cmdIn == CMD_FREQ_2K){
				error=scopeSetSamplingFreq(2000);
			}else if(cmdIn == CMD_FREQ_5K){
				error=scopeSetSamplingFreq(5000);
			}else if(cmdIn == CMD_FREQ_10K){
				error=scopeSetSamplingFreq(10000);
			}else if(cmdIn == CMD_FREQ_20K){
				error=scopeSetSamplingFreq(20000);
			}else if(cmdIn == CMD_FREQ_50K){
				error=scopeSetSamplingFreq(50000);
			}else if(cmdIn == CMD_FREQ_100K){
				error=scopeSetSamplingFreq(100000);
			}else if(cmdIn == CMD_FREQ_200K){
				error=scopeSetSamplingFreq(200000);
			}else if(cmdIn == CMD_FREQ_500K){
				error=scopeSetSamplingFreq(500000);
			}else if(cmdIn == CMD_FREQ_1M){
				error=scopeSetSamplingFreq(1000000);
			}else if(cmdIn == CMD_FREQ_2M){
				error=scopeSetSamplingFreq(2000000);
			}else if(cmdIn == CMD_FREQ_5M){
				error=scopeSetSamplingFreq(5000000);
			}else if(cmdIn == CMD_FREQ_10M){
				error=scopeSetSamplingFreq(10000000);
			}else if(cmdIn == CMD_FREQ_MAX){
				error=scopeSetSamplingFreq(UINT32_MAX);
			}else if(cmdIn < 0x07FFFFFF){
				error=scopeSetSamplingFreq(cmdIn);
			}

		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_TRIG_LEVEL: //set trigger level
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			scopeSetTrigLevel((uint16_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_PRETRIGGER: //set prettriger
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			scopeSetPretrigger((uint16_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_DATA_LENGTH:
		cmdIn = giveNextCmd();
		if(isScopeNumOfSamples(cmdIn)){
			if(cmdIn == CMD_SAMPLES_100){
				error=scopeSetNumOfSamples(100);
			}else if(cmdIn == CMD_SAMPLES_200){
				error=scopeSetNumOfSamples(200);
			}else if(cmdIn == CMD_SAMPLES_500){
				error=scopeSetNumOfSamples(500);
			}else if(cmdIn == CMD_SAMPLES_1K){
				error=scopeSetNumOfSamples(1000);
			}else if(cmdIn == CMD_SAMPLES_2K){
				error=scopeSetNumOfSamples(2000);
			}else if(cmdIn == CMD_SAMPLES_5K){
				error=scopeSetNumOfSamples(5000);
			}else if(cmdIn == CMD_SAMPLES_10K){
				error=scopeSetNumOfSamples(10000);
			}else if(cmdIn == CMD_SAMPLES_20K){
				error=scopeSetNumOfSamples(20000);
			}else if(cmdIn == CMD_SAMPLES_50K){
				error=scopeSetNumOfSamples(50000);
			}else if(cmdIn == CMD_SAMPLES_100K){
				error=scopeSetNumOfSamples(100000);
			}else if(cmdIn < 0x07FFFFFF){
				error=scopeSetNumOfSamples(cmdIn);
			}
		}else{
			cmdIn = CMD_ERR;
			error = SCOPE_INVALID_FEATURE_PARAM;
		}
		break;

	case CMD_SCOPE_START: //start sampling
		scopeStart();
		break;

	case CMD_SCOPE_STOP: //stop sampling
		scopeStop();
		break;

	case CMD_SCOPE_NEXT: //restart sampling
		scopeRestart();

		break;
	case CMD_GET_CONFIG:
		passMsg = MSG_SCOPE_SPEC;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;
	case CMD_GET_INPUTS:
		passMsg = MSG_SCOPE_INPUTS;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;

	case CMD_END:break;
	default:
		error = SCOPE_INVALID_FEATURE;
		cmdIn = CMD_ERR;
		break;
	}
	if(error>0){
		cmdIn=error;
	}else{
		cmdIn=CMD_END;
	}
	return cmdIn;
}
#endif //USE_SCOPE


#ifdef USE_SYNC_PWM
/**
 * @brief  Synchronized PWM generator command parse function.
 * @param  None
 * @retval Command ACK or ERR
 */
command parseSyncPwmCmd(void){
	command cmdIn=CMD_ERR;
	uint8_t error=0;
	uint16_t passMsg;
	double temp, temp2;

	cmdIn = giveNextCmd();
	switch(cmdIn){
	case CMD_SYNC_PWM_COMMAND:
		cmdIn = giveNextCmd();
		if(isSyncPwm(cmdIn)){
			if(cmdIn == CMD_SYNC_PWM_INIT){
				syncPwmSendInit();
			}else if(cmdIn == CMD_SYNC_PWM_DEINIT){
				syncPwmSendDeinit();
			}else if(cmdIn == CMD_SYNC_PWM_START){
				syncPwmSendStart();
			}else if(cmdIn == CMD_SYNC_PWM_STOP){
				syncPwmSendStop();
			}
		}else{
			cmdIn = CMD_ERR;
			error = SYNC_PWM_INVALID_FEATURE;
		}
		break;
	case CMD_SYNC_PWM_STEP:
		cmdIn = giveNextCmd();
		if(isSyncPwmStepMode(cmdIn)){
			if(cmdIn == CMD_SYNC_PWM_STEP_ENABLE){
				syncPwmSetStepMode();
			}else if(cmdIn == CMD_SYNC_PWM_STEP_DISABLE){
				syncPwmResetStepMode();
			}	
		}else{
			cmdIn = CMD_ERR;
			error = SYNC_PWM_INVALID_FEATURE;
		}
		break;
	case CMD_SYNC_PWM_FREQ:
		temp = giveNextCmd() / (double)giveNextCmd();
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			syncPwmSetFreq(cmdIn, temp);
		}else{
			cmdIn = CMD_ERR;
			error = SYNC_PWM_INVALID_FEATURE;
		}
		break;
	case CMD_SYNC_PWM_DUTYPHASE_CONFIG:
		temp = giveNextCmd() / (double)giveNextCmd();
		temp2 = giveNextCmd() / (double)giveNextCmd();
		cmdIn = giveNextCmd();  		    	// channel number
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			syncPwmSetDutyAndPhase(cmdIn, temp, temp2);
		}else{
			cmdIn = CMD_ERR;
			error = SYNC_PWM_INVALID_FEATURE;
		}
		break;
	case CMD_SYNC_PWM_CHAN_STATE:
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			syncPwmSetChannelState(((cmdIn)&0xff00)>>8,(uint8_t)(cmdIn));
		}else{
			cmdIn = CMD_ERR;
			error = SYNC_PWM_INVALID_FEATURE;
		}
		break;
	case CMD_SYNC_PWM_CHAN_INVERT:
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			syncPwmSetChannelInvert(((cmdIn)&0xff00)>>8,(uint8_t)(cmdIn));
		}else{
			cmdIn = CMD_ERR;
			error = SYNC_PWM_INVALID_FEATURE;
		}
		break;
	case CMD_GET_CONFIG:
		passMsg = MSG_SYNCPWM_CONFIG;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;
	case CMD_END:
		break;
	default:
		error = SYNC_PWM_INVALID_FEATURE;
		cmdIn = CMD_ERR;
		break;
	}

	cmdIn = (error > 0) ? error : CMD_END;	
	return cmdIn;			
}
#endif // USE_SYNC_PWM


#ifdef USE_LOG_ANLYS
/**
 * @brief  Logic Analyzer command parse function.
 * @param  None
 * @retval Command ACK or ERR
 */
command parseLogAnlysCmd(void){
	command cmdIn=CMD_ERR;
	uint8_t error=0;
	uint16_t passMsg;

	cmdIn = giveNextCmd();
	while(logAnlys.state == LOGA_DATA_SENDING);
	/* In order to change any parameter, sampling has to be stopped. */
	if((logAnlys.state == LOGA_SAMPLING) && (cmdIn != CMD_LOG_ANLYS_STOP)){
		logAnlysStop(); 
	}

	switch (cmdIn)
	{
	case CMD_LOG_ANLYS_INIT:
		logAnlysSendInit();
		break;
	case CMD_LOG_ANLYS_DEINIT:
		logAnlysSendDeinit();
		break;
	case CMD_LOG_ANLYS_START:
		logAnlysSendStart();
		break;
	case CMD_LOG_ANLYS_STOP:
		logAnlysSendStop();
		break;
	case CMD_LOG_ANLYS_PRETRIG:
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			logAnlysSetPretrigger((uint32_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = LOG_ANLYS_INVALID_FEATURE;
		}
		break;
	case CMD_LOG_ANLYS_POSTTRIG:
		cmdIn = giveNextCmd();
		uint32_t secondHalfOfDouble = commBufferReadUInt32();
		double postTrigTime = makeDoubleOutOfTwo32bit(secondHalfOfDouble, cmdIn);
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			logAnlysSetPosttrigger(postTrigTime);
		}else{
			cmdIn = CMD_ERR;
			error = LOG_ANLYS_INVALID_FEATURE;
		}
		break;
	case CMD_LOG_ANLYS_SAMPLING_FREQ:
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			logAnlysSetSamplingFreq((uint32_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = LOG_ANLYS_INVALID_FEATURE;
		}
		break;
	case CMD_LOG_ANLYS_SAMPLES_NUM:		// data length
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			logAnlysSetSamplesNum((uint16_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = LOG_ANLYS_INVALID_FEATURE;
		}
		break;
	case CMD_LOG_ANLYS_TRIGGER_MODE:
		cmdIn = giveNextCmd();
		if(isLogAnlysTriggerMode(cmdIn)){
			if(cmdIn == CMD_TRIG_MODE_AUTO){
				logAnlys.triggerMode = LOGA_MODE_AUTO;
			}else if(cmdIn == CMD_TRIG_MODE_NORMAL){
				logAnlys.triggerMode = LOGA_MODE_NORMAL;
			}else if(cmdIn == CMD_TRIG_MODE_SINGLE){
				logAnlys.triggerMode = LOGA_MODE_SINGLE;  ////// myslim ze nestaci jenom zmenit mode ale musi se znova spustit vzorkovani nebo neco ne???????
			}	
		}
		break;
	case CMD_LOG_ANLYS_TRIGGER_CHANNEL:
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			logAnlysSetTriggerChannel((uint32_t)cmdIn);
		}else{
			cmdIn = CMD_ERR;
			error = LOG_ANLYS_INVALID_FEATURE;
		}
		break;
	case CMD_LOG_ANLYS_TRIGGER_EVENT:
		cmdIn = giveNextCmd();
		if(isLogAnlysTriggerEvent(cmdIn)){
			if(cmdIn == CMD_TRIG_EDGE_RISING){
				logAnlysSetTriggerRising();
			}else if(cmdIn == CMD_TRIG_EDGE_FALLING){
				logAnlysSetTriggerFalling();
			}
		}else{
			cmdIn = CMD_ERR;
		}
		break;
		//		case CMD_LOG_ANLYS_USER_TRIG:
		//			cmdIn = giveNextCmd();
		//			if(cmdIn != CMD_END && cmdIn != CMD_ERR){
		//				logAnlysUserTrigger((uint16_t)cmdIn);
		//			}else{
		//				cmdIn = CMD_ERR;
		//				error = LOG_ANLYS_INVALID_FEATURE;
		//			}
		//			break;
	case CMD_GET_CONFIG:
		passMsg = MSG_LOGAN_CONFIG;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;		
	default:
		error = LOG_ANLYS_INVALID_FEATURE;
		cmdIn = CMD_ERR;
		break;
	}

	if((logAnlys.state == LOGA_WAIT_FOR_RESTART) 
			|| (cmdIn == CMD_LOG_ANLYS_POSTTRIG)
			|| (cmdIn == CMD_LOG_ANLYS_TRIGGER_EVENT)
			|| (cmdIn == CMD_LOG_ANLYS_TRIGGER_CHANNEL)
			|| (cmdIn == CMD_LOG_ANLYS_TRIGGER_MODE))
	{
		logAnlysStart();
	}

	cmdIn = (error > 0) ? error : CMD_END;
	return cmdIn;			
}

#endif // USE_LOG_ANLYS		

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_GEN_SIGNAL
command parseGeneratorSignalCmd(void){
	command cmdIn=CMD_ERR;
	uint8_t error=0;
	uint16_t passMsg;

	cmdIn = giveNextCmd();

	switch (cmdIn) {
	case CMD_GET_CONFIG:
		passMsg = MSG_GEN_SIGNAL_CONFIG;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;
	case CMD_GEN_MODE:
		cmdIn = giveNextCmd();
		if(isGeneratorMode(cmdIn)){
			if(cmdIn == CMD_MODE_SIGNAL){
				genSetMode(GEN_SIGNAL);
			}
		}
		break;
	default:
		error = parseGenCommonCmd(cmdIn);
		break;
	}
	cmdIn = (error > 0) ? error : CMD_END;
	return cmdIn;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

command parseVoltageSourceCmd(void){
	command cmdIn=CMD_ERR;
	uint8_t error=0;
	uint16_t passMsg;

	cmdIn = giveNextCmd();

	switch (cmdIn) {
	case CMD_GET_CONFIG:
		passMsg = MSG_DAC_CONFIG;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;
	case CMD_DAC_VAL:
		cmdIn = giveNextCmd();
		error = genSetDAC((uint16_t) (cmdIn), (uint16_t) (cmdIn >> 16));
		break;
	case CMD_GEN_START:
		genSetMode(GEN_VOLTSOURCE);
		break;
	case CMD_GEN_STOP:
		genStopVoltSource();
		break;
	case CMD_END:
		break;
	default:
		error = GEN_VOLT_INVALID_FEATURE;
		cmdIn = CMD_ERR;
		break;
	}

	return (error > 0) ? error : CMD_END;
}
#endif //USE_GEN_SIGNAL

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_GEN_PWM
command parseGeneratorPwmCmd(void){
	command cmdIn=CMD_ERR;
	uint8_t error=0;
	uint16_t passMsg;
	uint32_t freq;

	cmdIn = giveNextCmd();

	switch(cmdIn){
	case CMD_GET_CONFIG:
		passMsg = MSG_GEN_PWM_CONFIG;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;
	case CMD_GEN_MODE:
		cmdIn = giveNextCmd();
		if(isGeneratorMode(cmdIn)){
			if(cmdIn == CMD_MODE_PWM){
				genSetMode(GEN_PWM);
			}
		}
		break;
	case CMD_GEN_PWM_FREQ_CH1:
		cmdIn = giveNextCmd();
		freq = cmdIn;
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			genPwmSetFrequency(freq, 0);
		}else{
			cmdIn = CMD_ERR;
		}
		break;
	case CMD_GEN_PWM_FREQ_CH2:
		cmdIn = giveNextCmd();
		freq = cmdIn;
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			genPwmSetFrequency(freq, 1);
		}else{
			cmdIn = CMD_ERR;
		}
		break;
	default:
		error = parseGenCommonCmd(cmdIn);
		break;
	}

	cmdIn = (error > 0) ? error : CMD_END;
	return cmdIn;
}
#endif //USE_GEN_PWM

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_GEN_PATTERN
command parseGeneratorPatternCmd(void){
	command cmdIn=CMD_ERR;
	uint8_t error=0;
	uint16_t passMsg;

	cmdIn = giveNextCmd();

	switch(cmdIn){
	case CMD_GET_CONFIG:
		passMsg = MSG_GEN_PATTERN_CONFIG;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
		break;
	case CMD_GEN_MODE:
		cmdIn = giveNextCmd();
		if(isGeneratorMode(cmdIn)){
			if(cmdIn == CMD_MODE_PATTERN){
				genSetMode(GEN_PATTERN);
			}
		}
		break;
	default:
		error = parseGenCommonCmd(cmdIn);
		break;
	}
	cmdIn = (error > 0) ? error : CMD_END;
	return cmdIn;
}
#endif //USE_GEN_PATTERN

/////////////////////////////////////////////////////////////////////////////////////////////////////////

command parseGenCommonCmd(command cmdIn){
	uint8_t error=0;
	uint16_t index;
	uint8_t length,chan;
	uint16_t watchDog=5000;

	switch(cmdIn){

	case CMD_GEN_DATA://set data
		cmdIn = giveNextCmd();
		index=SWAP_UINT16(cmdIn);
		length=cmdIn>>16;
		chan=cmdIn>>24;
		while(watchDog>0 && getBytesAvailable()<length*2+1){
			watchDog--;
			osDelay(1);
		}
		if(getBytesAvailable()<length*2+1){
			error=GEN_MISSING_DATA;
			while(commBufferReadByte(&chan)==0);
		}else{
			error=genSetData(index,length*2,chan);
			if (error){
				while(commBufferReadByte(&chan)==0);
			}else{
				genDataOKSendNext();
			}
		}
		break;

	case CMD_GEN_SAMPLING_FREQ: //set sampling freq
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			error=genSetFrequency(((cmdIn)&0xffffff00)>>8,(uint8_t)(cmdIn));
		}else{
			cmdIn = CMD_ERR;
		}
		break;	

	case CMD_GEN_DEINIT:
		generator_deinit();
		break;

	case CMD_GET_REAL_FREQ: //get sampling freq
		genSendRealSamplingFreq();
		break;	

	case CMD_GEN_DATA_LENGTH_CH1: //set data length
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			error=genSetLength(cmdIn, 1);
		}else{
			cmdIn = CMD_ERR;
		}
		break;	

	case CMD_GEN_DATA_LENGTH_CH2: //set data length
		cmdIn = giveNextCmd();
		if(cmdIn != CMD_END && cmdIn != CMD_ERR){
			error=genSetLength(cmdIn, 2);
		}else{
			cmdIn = CMD_ERR;
		}
		break;	

	case CMD_GEN_CHANNELS: //set number of channels
		cmdIn = giveNextCmd();
		if(isChannel(cmdIn)){
			if(cmdIn == CMD_CHANNELS_1){
				error=genSetNumOfChannels(1);
			}else if(cmdIn == CMD_CHANNELS_2){
				error=genSetNumOfChannels(2);
			}else if(cmdIn == CMD_CHANNELS_3){
				error=genSetNumOfChannels(3);
			}else if(cmdIn == CMD_CHANNELS_4){
				error=genSetNumOfChannels(4);
			}
		}else{
			cmdIn = CMD_ERR;
		}
		break;

	case CMD_GEN_OUTBUFF_ON: //buffer on
		genSetOutputBuffer();
		break;

	case CMD_GEN_OUTBUFF_OFF: //buffer off
		genUnsetOutputBuffer();
		break;	

	case CMD_GEN_START: //start generating
		genStart();
		genStatusOK();
		break;	

	case CMD_GEN_STOP: //stop generating
		genStop();
		break;	

	case CMD_GEN_RESET:
		genReset();
		break;

	case CMD_END:
		break;

	default:
		error = GEN_INVALID_FEATURE;
		cmdIn = CMD_ERR;
		break;
	}

	return error;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Read command from input buffer
 * @param  None
 * @retval Command
 */
command giveNextCmd(void){
	uint8_t cmdNext[5];
	uint8_t bytesRead = commBufferReadNBytes(cmdNext, 5);
	if(bytesRead >= 4){
		return BUILD_CMD(cmdNext);
	}else if(bytesRead == 0){
		return CMD_END;
	}else{
		return CMD_ERR;
	}
} 


/**
 * @brief  Printr error code
 * @param  Command
 * @retval None
 */
void printErrResponse(command cmd){
	uint8_t err[2];
	if(cmd == CMD_END){
		uint16_t passMsg = MSG_ACK;
		xQueueSendToBack(messageQueue, &passMsg, portMAX_DELAY);
	}else{
		err[0]=ERROR_PREFIX;
		err[1]=cmd;
		xQueueSendToBack(messageQueue, err, portMAX_DELAY);
	}
}

double makeDoubleOutOfTwo32bit(uint32_t word1, uint32_t word2){
	uint32_t makeArray[2];
	makeArray[0] = word1;
	makeArray[1] = word2;

	double doubleVal;
	memcpy(&doubleVal, makeArray, sizeof(doubleVal));
	return doubleVal;
}

double getDouble(command cmd){
	uint32_t secondHalfOfDouble = commBufferReadUInt32();
	return makeDoubleOutOfTwo32bit(secondHalfOfDouble, cmd);
}


/**
 * @}
 */

/**
 * @}
 */

