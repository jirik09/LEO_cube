/**
  *****************************************************************************
  * @file    comms.c
  * @author  Y3288231
  * @date    Dec 15, 2014
  * @brief   This file contains functions for communication
  ***************************************************************************** 
*/ 

// Includes ===================================================================

#include "cmsis_os.h"
#include "mcu_config.h"
#include "comms.h"
#include "comms_hal.h"
#include "cmd_parser.h"
#include "scope.h"
#include "generator.h"
#include "commands.h"
#include "counter.h"
#include "logic_analyzer.h"
#include "usb_device.h"
#include "usart.h"
#include "gpio.h"

/** @defgroup Comms Comms
  * @{
  */

/** @defgroup Comms_Private_Variables Comms Private Variables
  * @{
  */
xQueueHandle messageQueue;
static xSemaphoreHandle commsMutex;
static uint8_t commBuffMem[COMM_BUFFER_SIZE];
//static uint8_t commTXBuffMem[COMM_TX_BUFFER_SIZE];
static commBuffer comm;
//commBuffer commTX;
char cntMessage[30];
uint8_t volatile testArray[100];	
/**
  * @}
  */
	
/** @defgroup Comms_Private_Prototypes Comms Private Function Prototypes
  * @{
  */	
void sendSystConf(void);
void sendCommsConf(void);
void sendScopeConf(void);
void sendCounterConf(void);
void sendScopeInputs(void);
void sendGenConf(void);
void sendGenPwmConf(void);
void sendSyncPwmConf(void);
void sendLogAnlysConf(void);
void sendShieldPresence(void);
void sendSystemVersion(void);
void assertPins(void);
/**
  * @}
  */

/** @defgroup Comms_Function_Definitions Comms Function Definitions
  * @{
  */
//portTASK_FUNCTION(vPrintTask, pvParameters) {
void LLCommTask(void const *argument){

	portBASE_TYPE xHigherPriorityTaskWoken;
	while(1){
		while (GetDMAIndex()!=comm.writePointer){
			xSemaphoreTakeRecursive(commsMutex, portMAX_DELAY);
			if(comm.memory[comm.writePointer]==';'){
				comm.state = BUFF_DATA;
				xQueueSendToBackFromISR(cmdParserMessageQueue, "1TryParseCmd", &xHigherPriorityTaskWoken);
			}
			comm.writePointer=(comm.writePointer+1)%COMM_BUFFER_SIZE;
			xSemaphoreGiveRecursive(commsMutex);
		}
		taskYIELD();
	}


}

int commSend = 0;
/**
  * @brief  Communication task function.
  * @param  Task handler, parameters pointer
  * @retval None
  */
//portTASK_FUNCTION(vPrintTask, pvParameters) {
void CommTask(void const *argument){
	
	//Build error on lines below? Lenght of Pin strings must be 4 chars long!!!
	CASSERT(sizeof(USART_TX_PIN_STR)==5);
	CASSERT(sizeof(USART_RX_PIN_STR)==5);
	#ifdef USE_USB
	CASSERT(sizeof(USB_DP_PIN_STR)==5);
	CASSERT(sizeof(USB_DM_PIN_STR)==5);
	#endif //USE_USB
	
	
	
	messageQueue = xQueueCreate(5, 30);
	commsMutex = xSemaphoreCreateRecursiveMutex();
	
	xSemaphoreTakeRecursive(commsMutex, portMAX_DELAY);
	commsInit();
	xSemaphoreGiveRecursive(commsMutex);
	
	char message[30];

	#ifdef USE_SCOPE
	uint8_t header[16]="OSC_yyyyxxxxCH0x";
	uint8_t *pointer;
	
	uint8_t k;
	uint32_t tmpToSend;
	uint32_t dataLength;
	uint32_t dataLenFirst;
	uint32_t dataLenSecond;
	uint16_t adcRes;
	uint16_t channels;
	uint32_t oneChanMemSize;
	#endif //USE_SCOPE

	#if defined(USE_GEN) || defined(USE_GEN_PWM)
	uint8_t header_gen[12]="GEN_xCH_Fxxx";
	#endif //USE_GEN || USE_GEN_PWM
	
	#if defined(USE_GEN) || defined(USE_SCOPE)
	uint8_t i;
	uint32_t j;
	#endif //USE_GEN || USE_SCOPE
	while(1){	
		xQueueReceive (messageQueue, message, portMAX_DELAY);
		///commsSendString("COMMS_Run\r\n");
		xSemaphoreTakeRecursive(commsMutex, portMAX_DELAY);
		
		//send IDN string
		if(message[0]=='0'){
			commsSendString(STR_ACK);
			commsSendString(IDN_STRING);
			#ifdef USE_SHIELD
			if(isScopeShieldConnected()==1){
				commsSendString(SHIELD_STRING);
			}else if(isScopeShieldConnected()==2){
				commsSendString(SHIELD_STRING_2);
			}
			#endif			
			
		//send data
		}else if(message[0]=='1'){
			#ifdef USE_SCOPE
			if(getScopeState() == SCOPE_DATA_SENDING){
				oneChanMemSize=getOneChanMemSize();
				dataLength = getSamples();
				adcRes = getADCRes();
				channels=GetNumOfChannels();
				
				j=scopeGetRealSmplFreq();
				header[4]=(uint8_t)(j>>24);
				header[5]=(uint8_t)(j>>16);
				header[6]=(uint8_t)(j>>8);
				header[7]=(uint8_t)(j);
				
				if(adcRes>8){
					j = ((getTriggerIndex() - ((getSamples() * getPretrigger()) >> 16 ))*2+oneChanMemSize)%oneChanMemSize;
					dataLength*=2;
				}else{
					j = ((getTriggerIndex() - ((getSamples() * getPretrigger()) >> 16 ))+oneChanMemSize)%oneChanMemSize;
				} 
				
				header[8]=(uint8_t)adcRes;	
				header[9]=(uint8_t)(dataLength >> 16);
				header[10]=(uint8_t)(dataLength >> 8);
				header[11]=(uint8_t)dataLength;
				header[15]=channels;
				
				if(j+dataLength>oneChanMemSize){
					dataLenFirst=oneChanMemSize-j;
					dataLenSecond=dataLength-dataLenFirst;
				}else{
					dataLenFirst=dataLength;
					dataLenSecond=0;
				}
				
				for(i=0;i<channels;i++){
				
					pointer = (uint8_t*)getDataPointer(i);
				
					//sending header
					header[14]=(i+1);
					
					commsSendBuff(header,16);
					
					if(dataLenFirst>COMMS_BULK_SIZE ){
						tmpToSend=dataLenFirst;
						k=0;
						while(tmpToSend>COMMS_BULK_SIZE){
							commsSendBuff(pointer + j+k*COMMS_BULK_SIZE, COMMS_BULK_SIZE);
							k++;
							tmpToSend-=COMMS_BULK_SIZE;
						}
						if(tmpToSend>0){
						commsSendBuff(pointer + j+k*COMMS_BULK_SIZE, tmpToSend);
						}
					}else if(dataLenFirst>0){
						commsSendBuff(pointer + j, dataLenFirst);
					}
					
					if(dataLenSecond>COMMS_BULK_SIZE ){
						tmpToSend=dataLenSecond;
						k=0;
						while(tmpToSend>COMMS_BULK_SIZE){
							commsSendBuff(pointer+k*COMMS_BULK_SIZE, COMMS_BULK_SIZE);
							k++;
							tmpToSend-=COMMS_BULK_SIZE;
						}
						if(tmpToSend>0){
						commsSendBuff(pointer+k*COMMS_BULK_SIZE, tmpToSend);
						}
					}else if(dataLenSecond>0){
						commsSendBuff(pointer, dataLenSecond);
					}
				}	
				///commsSendString("COMMS_DataSending\r\n");
				commsSendString(STR_SCOPE_OK);
				xQueueSendToBack(scopeMessageQueue, "2DataSent", portMAX_DELAY);
				
			}
			#endif //USE_SCOPE
		//send generating frequency	
		}else if(message[0]=='2'){
			#if defined(USE_GEN) || defined(USE_GEN_PWM)
			for(i = 0;i<MAX_DAC_CHANNELS;i++){
				header_gen[4]=i+1+
				48;
				j=genGetRealSmplFreq(i+1);
				header_gen[9]=(uint8_t)(j>>16);
				header_gen[10]=(uint8_t)(j>>8);
				header_gen[11]=(uint8_t)(j);
				commsSendBuff(header_gen,12);
			}
			#endif //USE_GEN || USE_GEN_PWM
			
		/* ---------------------------------------------------- */	
		/* COUNTER MEASURED DATA & IC BUFFER CORRECTION sending */
		/* ---------------------------------------------------- */
		}else if(message[0]=='G'){
			#ifdef USE_COUNTER		
			
			/* ETR mode configured */	
			if(counter.state==COUNTER_ETR){
				commsSendString(STR_CNT_ETR_DATA);
				sprintf(cntMessage, "%016.6f", counter.counterEtr.freq);
				commsSendString(cntMessage);		
				
			/* REF mode configured */	
			}else if(counter.state==COUNTER_REF){
				commsSendString(STR_CNT_REF_DATA);
				/* Here only the buffer is sent - PC app calculates frequency ratio as:
					 REF buffer / ETR buffer = arr * psc / buffer - where arr and psc is already 
					 known by PC app (user set) */										
				sprintf(cntMessage, "%010d", counter.counterEtr.buffer);
				commsSendString(cntMessage);										

			/* IC mode configured channel 1 */	
			}else if(counter.state==COUNTER_IC){		
				
				if(counter.icDutyCycle==DUTY_CYCLE_DISABLED){	
					
					if(counter.icChannel1==COUNTER_IRQ_IC){												
						commsSendString(STR_CNT_IC1_DATA);
						sprintf(cntMessage, "%016.6f", counter.counterIc.ic1freq);
						commsSendString(cntMessage);	
						counter.icChannel1=COUNTER_IRQ_IC_PASS;
					}	

					if(counter.icChannel2==COUNTER_IRQ_IC){							
						commsSendString(STR_CNT_IC2_DATA);	
						sprintf(cntMessage, "%016.6f", counter.counterIc.ic2freq);
						commsSendString(cntMessage);															
						counter.icChannel2=COUNTER_IRQ_IC_PASS;
					}						

				}else{		
										
					sprintf(cntMessage, "%06.3f", counter.counterIc.ic1freq);
					char cntMessage2[15];
					sprintf(cntMessage2, "%015.12f", counter.counterIc.ic2freq);
					
					commsSendString(STR_CNT_DUTY_CYCLE);
					commsSendString(cntMessage);					
					commsSendString(cntMessage2);		
				
//					commsSendString(STR_CNT_PULSE_WIDTH);
//					sprintf(cntMessage, "%015.12f", counter.counterIc.ic2freq);
//					commsSendString(cntMessage);	
				}					

			/* TI mode configured */		
			}else if(counter.state==COUNTER_TI){						
				switch(counter.tiState){
					case TIMEOUT:
						commsSendString(STR_CNT_TI_TIMEOUT);
						sprintf(message, "%02d", 2);						
						break;
					case SEND_TI_DATA:
						commsSendString(STR_CNT_TI_DATA);
						sprintf(cntMessage, "%016.12f", counter.counterIc.ic1freq);											
						break;
					case CLEAR:
						break;
				}
				commsSendString(cntMessage);					
				counter.tiState = CLEAR;
			}								
	
		}else if(message[0]=='O'){
			commsSendString(STR_CNT_REF_WARN);			
			sprintf(cntMessage, "%02d", 2);
			commsSendString(cntMessage);			
			
			#endif //USE_COUNTER			
		/* ---------------------------------------------------- */	
		/* ------------------ END OF COUNTER ------------------ */
		/* ---------------------------------------------------- */	
		/* Send LOGIC ANALYZER data */
		}else if(message[0]=='L'){
			#ifdef USE_LOG_ANLYS
			logAnlys.state = LOGA_DATA_SENDING;
			if(logAnlys.trigOccur == TRIG_OCCURRED){
				commsSendString(STR_LOG_ANLYS_TRIGGER_POINTER);	
				commsSendUint32(logAnlys.triggerPointer);
				logAnlys.trigOccur = TRIG_NOT_OCCURRED;
			}
			/* Send user trigger associated with data */
			/* 16-bit GPIO register by DMA to 16-bit array. Array send 8-bit by 8-bit to PC. samplesNumber countes with 16-bit array. */
//			commsSendString(STR_LOG_ANLYS_DATA_LENGTH);				
//			commsSendUint32(logAnlys.samplesNumber * 2);	
//			commsSendString(STR_LOG_ANLYS_DATA);	
//			commsSendBuff((uint8_t *)&logAnlys.samplesNumber,(logAnlys.samplesNumber * 2));					
			
			/* Send data */				
			commsSendString(STR_LOG_ANLYS_DATA_LENGTH);				
			commsSendUint32(logAnlys.samplesNumber * 2 + SCOPE_BUFFER_MARGIN * MAX_ADC_CHANNELS * 2);				
			commsSendString(STR_LOG_ANLYS_DATA);
			HAL_UART_Transmit(&huart2, (uint8_t *)logAnlys.bufferMemory, logAnlys.samplesNumber * 2 + SCOPE_BUFFER_MARGIN * MAX_ADC_CHANNELS * 2, 10000);			
			logAnlys.state = LOGA_DATA_SENT;
//			uint8_t blockNum = (logAnlys.samplesNumber * 2) / LOG_ANLYS_DEFAULT_DATA_LEN;		// be sure that the define is set to 100.
//			for(int k = 0; k < blockNum; k++)
//      {
//				//commsSendBuff((uint8_t *)&scopeBuffer[k*LOG_ANLYS_DEFAULT_DATA_LEN], LOG_ANLYS_DEFAULT_DATA_LEN);								
//				HAL_UART_Transmit(&huart2, (uint8_t *)&scopeBuffer[k*LOG_ANLYS_DEFAULT_DATA_LEN], LOG_ANLYS_DEFAULT_DATA_LEN, 5000);
//				taskYIELD();
//      }		
			#endif //USE_LOG_ANLYS
			
		// send system config
		}else if(message[0]=='3'){
			sendSystConf();
			
		// send comms config
		}else if(message[0]=='4'){
			sendCommsConf();
			
		// send scope config
		}else if(message[0]=='5'){
			#ifdef USE_SCOPE
			sendScopeConf();
			#endif //USE_SCOPE
			
		// send counter config
		}else if(message[0]=='D'){
			#ifdef USE_COUNTER
			sendCounterConf();
			#endif //USE_COUNTER
		// send scope inputs
		}else if(message[0]=='B'){
			#ifdef USE_SCOPE
			sendScopeInputs();
			#endif //USE_SCOPE
			
		// send shield presence
		}else if(message[0]=='C'){
			#ifdef USE_SHIELD
			sendShieldPresence();
			#endif //USE_SHIELD
			
		// send gen config
		}else if(message[0]=='6'){
			#ifdef USE_GEN
			sendGenConf();
			#endif //USE_GEN
			
		// send PWM gen config
		}else if(message[0]=='P'){
			#ifdef USE_GEN_PWM
			sendGenPwmConf();
			#endif //USE_GEN_PWM		

		// send Synchronized PWM gen config
		}else if(message[0]=='W'){
			#ifdef USE_SYNC_PWM
			sendSyncPwmConf();
			#endif //USE_GEN_PWM			
			
		// send Logic Analyzer config
		}else if(message[0]=='Y'){
			#ifdef USE_LOG_ANLYS
			sendLogAnlysConf();
			#endif //USE_LOG_ANLYS					
				
		// send gen next data block
		}else if(message[0]=='7'){
			#if defined(USE_GEN) || defined(USE_GEN_PWM)
			commsSendString(STR_GEN_NEXT);
			#endif //USE_GEN || USE_GEN_PWM
			
		// send gen ok status
		}else if(message[0]=='8'){
			#if defined(USE_GEN) || defined(USE_GEN_PWM)
			commsSendString(STR_GEN_OK);
			#endif //USE_GEN || USE_GEN_PWM
			
		}else if (message[0]=='9'){
			sendSystemVersion();
			
		}else if (message[0]=='Q'){
//			flushBuff(0);
			
		}else if (message[0] == 'I'){
			xQueueReceive(messageQueue, message, portMAX_DELAY);
			commsSendString(message);
			/////commsSendString("\r\n");
			
		//send ACK_	
		}else if (message[0]=='A'){
			commsSendString(STR_ACK);
			
		// not known message -> send it
		}else{
			commsSendString(message);
			/////commsSendString("\r\n");
		}
		//flushBuff(200);
		xSemaphoreGiveRecursive(commsMutex);		
	}
}

/**
  * @brief  Communication initialisation.
  * @param  None
  * @retval None
  */
void commsInit(void){
	#ifdef USE_USB
	MX_USB_DEVICE_Init();
	#endif //USE_USB
	MX_USART2_UART_Init();
	comm.memory = commBuffMem;
	comm.bufferSize = COMM_BUFFER_SIZE;
	comm.writePointer = 0;
	comm.readPointer = 0;
	comm.state = BUFF_EMPTY;
//	commTX.memory = commTXBuffMem;
//	commTX.bufferSize = COMM_TX_BUFFER_SIZE;
//	commTX.writePointer = 0;
//	commTX.readPointer = 0;
//	commTX.state = BUFF_EMPTY;
	//HAL_UART_Receive_DMA(&huart2,comm.memory,comm.bufferSize);
}

/**
  * @brief  Store incoming byte to buffer
  * @param  incoming byte
  * @retval 0 success, 1 error - buffer full
  */
uint8_t commBufferStoreByte(uint8_t chr){
	if(comm.state == BUFF_FULL){
		return 1;
	}else{
		*(comm.memory + comm.writePointer) = chr;
		comm.writePointer = (comm.writePointer + 1) % COMM_BUFFER_SIZE;
		if(comm.state == BUFF_EMPTY){
			comm.state = BUFF_DATA;
		}else if(comm.state == BUFF_DATA && comm.writePointer == comm.readPointer){
			comm.state = BUFF_FULL;
		}
		return 0;
	}
}

/**
  * @brief  Read byte from coms buffer
  * @param  pointer where byte will be written
  * @retval 0 success, 1 error - buffer empty
  */
uint8_t commBufferReadByte(uint8_t *ret){
	if(comm.state == BUFF_EMPTY){
		return 1;
	}else{
		*ret = *(comm.memory + comm.readPointer);
		comm.readPointer = (comm.readPointer + 1) % COMM_BUFFER_SIZE;
		if(comm.state == BUFF_FULL){
			comm.state = BUFF_DATA;
		}else if(comm.state == BUFF_DATA && comm.writePointer == comm.readPointer){
			comm.state = BUFF_EMPTY;
		}
		return 0;
	}
}

/**
  * @brief  Read N bytes from coms buffer
  * @param  pointer where bytes will be written and number of bytes to read
  * @retval Number of bytes read
  */
uint8_t commBufferReadNBytes(uint8_t *mem, uint16_t count){
	for(uint16_t i = 0; i < count; i++){
		if(commBufferReadByte(mem++) == 1){
			return i;
		}
	}
	return count;
}

/**
  * @brief  Read N bytes from coms buffer
  * @param  pointer where bytes will be written and number of bytes to read
  * @retval Number of bytes read
  */
uint16_t commBufferLookNewBytes(uint8_t *mem){
	uint16_t result = commBufferCounter();
	for(uint16_t i = 0;i<result;i++){
		*(mem++)=*(comm.memory+((comm.readPointer+i)%COMM_BUFFER_SIZE));
	}
	return result;
}



/**
  * @brief  Read N bytes from coms buffer
  * @param  pointer where bytes will be written and number of bytes to read
  * @retval Number of bytes read
  */
uint16_t commBufferCounter(void){
	if(comm.state == BUFF_FULL){
		return COMM_BUFFER_SIZE;
	}else{
		return (comm.writePointer+COMM_BUFFER_SIZE-comm.readPointer)%COMM_BUFFER_SIZE;
	}
}

/**
  * @brief  Processing of incoming byte
  * @param  incomming byte
  * @retval 0 success, 1 error - buffer full
  */
uint8_t commInputByte(uint8_t chr){
	portBASE_TYPE xHigherPriorityTaskWoken;
	uint8_t result=0;	
	if (chr==';'){
		result = commBufferStoreByte(chr);
		xQueueSendToBackFromISR(cmdParserMessageQueue, "1TryParseCmd", &xHigherPriorityTaskWoken);
		return result;
	}else{
		return commBufferStoreByte(chr);
	}
}

uint16_t getBytesAvailable(){
	uint16_t result; 
	if(comm.state==BUFF_FULL){
		return COMM_BUFFER_SIZE;
	}else if(comm.state==BUFF_EMPTY){
		return 0;
	}else{
		result = (comm.writePointer+COMM_BUFFER_SIZE-comm.readPointer)%COMM_BUFFER_SIZE;
		return result;
	}
}

/**
  * @brief  Send System configuration.
  * @param  None
  * @retval None
  */
void sendSystConf(){
	commsSendString("SYST");
	commsSendUint32(HAL_RCC_GetHCLKFreq());  //CCLK
	commsSendUint32(HAL_RCC_GetPCLK2Freq()); //PCLK
	commsSendString(MCU);
}

/**
  * @brief  Send Communication configuration.
  * @param  None
  * @retval None
  */
void sendCommsConf(){
	commsSendString("COMM");
	commsSendUint32(COMM_BUFFER_SIZE);
	commsSendUint32(UART_SPEED);
	commsSendString(USART_TX_PIN_STR);
	commsSendString(USART_RX_PIN_STR);
	#ifdef USE_USB
	commsSendString("USB_");
	commsSendString(USB_DP_PIN_STR);
	commsSendString(USB_DM_PIN_STR);
	#endif
}

/**
  * @brief  Send System version.
  * @param  None
  * @retval None
  */
void sendSystemVersion(){
	commsSendString("VER_");
	commsSendString("Instrulab FW"); 	//12
	commsSendString(FW_VERSION); 			//4
	commsSendString(BUILD);						//4
	commsSendString("FreeRTOS");			//8	
	commsSendString(tskKERNEL_VERSION_NUMBER);//6
	commsSendString("ST HAL");				//6
	commsSend('V');
	commsSend((HAL_GetHalVersion()>>24)+48);
	commsSend('.');
	commsSend((HAL_GetHalVersion()>>16)+48);
	commsSend('.');
	commsSend((HAL_GetHalVersion()>>8)+48); //6

}

#ifdef USE_SCOPE
/**
  * @brief  Send Scope configuration.
  * @param  None
  * @retval None
  */
void sendScopeConf(){
	uint8_t i;
	commsSendString("OSCP");
	commsSendUint32(MAX_SAMPLING_FREQ_12B);
	commsSendUint32(MAX_SCOPE_BUFF_SIZE);
	commsSendUint32(MAX_ADC_CHANNELS);
	for (i=0;i<MAX_ADC_CHANNELS;i++){
		switch(i){
			case 0:
				commsSendString(SCOPE_CH1_PIN_STR);
				break;
			case 1:
				commsSendString(SCOPE_CH2_PIN_STR);
				break;
			case 2:
				commsSendString(SCOPE_CH3_PIN_STR);
				break;
			case 3:
				commsSendString(SCOPE_CH4_PIN_STR);
				break;
		}
	}
	commsSendUint32(SCOPE_VREF);
	commsSendUint32(SCOPE_VREF_INT);
	commsSendBuff((uint8_t*)scopeGetRanges(&i),i);
}
#endif //USE_SCOPE


#ifdef USE_COUNTER
/**
  * @brief  Send Counter configuration.
  * @param  None
  * @retval None
  */
void sendCounterConf(){
	commsSendString("CNT_");
	commsSendUint32(CNT_COUNTER_PERIPH_CLOCK);
	commsSendUint32(CNT_GATE_PERIPH_CLOCK);
	commsSendString(COUNTER_MODES);
	commsSendString(CNT_ETR_PIN);
	commsSendString(CNT_IC_CH1_PIN);
	commsSendString(CNT_IC_CH2_PIN);
	commsSendString(CNT_REF1_PIN);
	commsSendString(CNT_REF2_PIN);
	/* Timer Interval pins (Events) */
	commsSendString(CNT_IC_CH1_PIN);
	commsSendString(CNT_IC_CH2_PIN);
	
	/* Scope Get Config is the last configuration demand - reconfig usart baud */
//	huart2.Init.BaudRate = 2000000;
//  HAL_UART_Init(&huart2);
}
#endif //USE_COUNTER


#ifdef USE_SCOPE
/**
  * @brief  Send Scope input channels.
  * @param  None
  * @retval None
  */
void sendScopeInputs(){
	uint8_t i,j;
	commsSendString("INP_");
	
	if(MAX_ADC_CHANNELS>=1){
		commsSend(ANALOG_DEFAULT_INPUTS[0]);
	}
	if(MAX_ADC_CHANNELS>=2){
		commsSend(ANALOG_DEFAULT_INPUTS[1]);
	}
	if(MAX_ADC_CHANNELS>=3){
		commsSend(ANALOG_DEFAULT_INPUTS[2]);
	}
	if(MAX_ADC_CHANNELS>=4){
		commsSend(ANALOG_DEFAULT_INPUTS[3]);
	}
	
	for (i=0;i<MAX_ADC_CHANNELS;i++){
		commsSendString("/");
		for (j=0;j<NUM_OF_ANALOG_INPUTS[i];j++){
			if(j>0){
				commsSendString(":");
			}
			switch(i){
			case 0:
				commsSendString((char *)ANALOG_CHANN_ADC1_NAME[j]);
				break;
			case 1:
				commsSendString((char *)ANALOG_CHANN_ADC2_NAME[j]);
				break;
			case 2:
				commsSendString((char *)ANALOG_CHANN_ADC3_NAME[j]);
				break;
			case 3:
				commsSendString((char *)ANALOG_CHANN_ADC4_NAME[j]);
				break;
			}
		}
	}
	commsSendString("/");
	commsSendString(";");
}
#endif //USE_SCOPE


#ifdef USE_GEN
/**
  * @brief  Send Arb. DAC Generator configuration.
  * @param  None
  * @retval None
  */
void sendGenConf(){
	uint8_t i;
	commsSendString("GEN_");
	commsSendUint32(MAX_GENERATING_FREQ);
	commsSendUint32(MAX_GENERATOR_BUFF_SIZE);
	commsSendUint32(DAC_DATA_DEPTH);
	commsSendUint32(MAX_DAC_CHANNELS);
	for (i=0;i<MAX_DAC_CHANNELS;i++){
		switch(i){
			case 0:
				commsSendString(GEN_CH1_PIN_STR);
				break;
			case 1:
				commsSendString(GEN_CH2_PIN_STR);
				break;
		}
	}
#ifdef USE_SHIELD
	if(isScopeShieldConnected()){
		commsSendInt32(SHIELD_GEN_LOW);
		commsSendUint32(SHIELD_GEN_HIGH); 
	}else{
		commsSendUint32(0);
		commsSendUint32(GEN_VREF);
	}
#else
	commsSendUint32(0);
	commsSendUint32(GEN_VREF);
#endif
	commsSendUint32(GEN_VDDA);
	commsSendUint32(GEN_VREF_INT);
}
#endif //USE_GEN


#ifdef USE_GEN_PWM
/**
  * @brief  Send Arb. PWM Generator configuration.
  * @param  None
  * @retval None
  */
void sendGenPwmConf(void){
	uint8_t i;
	commsSendString("GENP");		
	commsSendUint32(MAX_GEN_PWM_CHANNELS);
	for (i=0;i<MAX_DAC_CHANNELS;i++){
		switch(i){
			case 0:
				commsSendString(GEN_PWM_CH1_PIN);
				break;
			case 1:
				commsSendString(GEN_PWM_CH2_PIN);					
				break;
		}
	}
}
#endif //USE_GEN_PWM


#ifdef USE_SYNC_PWM
/**
  * @brief  Send Synch. PWM Generator configuration.
  * @param  None
  * @retval None
  */
void sendSyncPwmConf(void)
{
	uint8_t i;
	commsSendString("SYNP");		
	commsSendUint32(SYNC_PWM_TIM_PERIPH_CLOCK);
	commsSendUint32(MAX_SYNC_PWM_FREQ);
	commsSendUint32(MAX_SYNC_PWM_CHANNELS);
	for (i=0;i<MAX_SYNC_PWM_CHANNELS;i++){
		switch(i){
			case 0:
				commsSendString(SYNC_PWM_CH1_PIN);
				break;
			case 1:
				commsSendString(SYNC_PWM_CH2_PIN);
				break;			
			case 2:
				commsSendString(SYNC_PWM_CH3_PIN);	
				break;
			case 3:
				commsSendString(SYNC_PWM_CH4_PIN);	
				break;
		}
	}
}
#endif //USE_SYNC_PWM

#ifdef USE_LOG_ANLYS
void sendLogAnlysConf(void)
{
	uint8_t i;
	commsSendString("LOGA");
	commsSendUint32(LOG_ANLYS_POSTTRIG_PERIPH_CLOCK);
	commsSendUint32(LOG_ANLYS_TIMEBASE_PERIPH_CLOCK);
	commsSendUint32(LOG_ANLYS_SAMPLING_FREQ);
	commsSendUint32(LOG_ANLYS_BUFFER_LENGTH);
	commsSendUint32(LOG_ANLYS_CHANNELS_NUM);
	for (i=0;i<LOG_ANLYS_CHANNELS_NUM;i++){
		switch(i){
			case 0:
				commsSendString(LOG_ANLYS_PIN_CH1);
				break;
			case 1:
				commsSendString(LOG_ANLYS_PIN_CH2);
				break;			
			case 2:
				commsSendString(LOG_ANLYS_PIN_CH3);	
				break;
			case 3:
				commsSendString(LOG_ANLYS_PIN_CH4);	
				break;
			case 4:
				commsSendString(LOG_ANLYS_PIN_CH5);	
				break;
			case 5:
				commsSendString(LOG_ANLYS_PIN_CH6);	
				break;
			case 6:
				commsSendString(LOG_ANLYS_PIN_CH7);	
				break;
			case 7:
				commsSendString(LOG_ANLYS_PIN_CH8);	
				break;			
		}
	}	
}
#endif //USE_LOG_ANLYS


#ifdef USE_SHIELD
/**
  * @brief  Send info whether the shield is connected.
  * @param  None
  * @retval None
  */
void sendShieldPresence(void){
	if(isScopeShieldConnected()){
		commsSendString(STR_ACK);
	}else{
		commsSendString(STR_NACK);
	}
}

#endif //USE_SHIELD

/**
  * @}
  */	

/**
  * @}
  */

