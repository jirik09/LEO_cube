/**
  *****************************************************************************
  * @file    comms_hal.h
  * @author  Y3288231
  * @date    jan 15, 2014
  * @brief   Hardware abstraction for communication
  ***************************************************************************** 
*/ 
#ifndef COMMS_HAL_H_
#define COMMS_HAL_H_

void commHalInit(void);
void commsSend(uint8_t chr);
void commsSendInt32(int32_t num);
void commsSendUint32(uint32_t num);
void commsSendBuff(uint8_t *buff, uint16_t len);
void commsSendString(char *chr);
void commsSendDouble(double value);
//void commsSendDMA(uint8_t chr);
//void commsSendInt32DMA(int32_t num);
//void commsSendUint32DMA(uint32_t num);
//void commsSendBuffDMA(uint8_t *buff, uint16_t len);
//void commsSendStringDMA(char *chr);
//void flushBuff(uint16_t threshold);
//void XferComplete(DMA_HandleTypeDef *hdma);
void commsRecieveUSB(uint8_t chr);
void commsRecieveUART(uint8_t chr);

#endif /* COMMS_HAL_H_ */
