/**
  *****************************************************************************
  * @file    cmd_parser.h
  * @author  Y3288231
  * @date    Dec 15, 2014
  * @brief   This file contains functions for parsing commands
  ***************************************************************************** 
*/ 
#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

/** @addtogroup Parser
  * @{
  */
	
// Constant definitions =======================================================
extern xQueueHandle cmdParserMessageQueue;
	
// Types definitions ==========================================================
// Exported variables =========================================================
// Exported functions =========================================================
/** @defgroup Parser_Prototypes Parser Function Prototypes
  * @{
  */
void CmdParserTask(void const *argument);
double makeDoubleOutOfTwo32bit(uint32_t word1, uint32_t word2);

static const uint8_t KOD[40] ={0x14, 0x93, 0xf4, 0xad, 0xce, 0xed, 0x69, 0x7b, 0x88, 0x76, 0xe5, 0x05, 0x20, 0x14, 0x0d, 0x1b, 0xd6, 0x1c, 0x81, 0xd0, 0x51, 0x1e, 0x79, 0xff, 0x23, 0x22, 0x04, 0xdd, 0x3a, 0x11, 0x47, 0xb3, 0x17, 0x71, 0x47, 0x97, 0x9b, 0x49, 0x9a, 0xc9};

/**
  * @}
  */

#endif /* CMD_PARSER_H_ */

/**
  * @}
  */
