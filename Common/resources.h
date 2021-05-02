/**
  *****************************************************************************
  * @file    resources.h
  * @author  Jirik
  * @date    1.5.2021
  * @brief   This file contains definitions and prototypes of sync PWM generator 
						 functions.
  ***************************************************************************** 
*/ 


#ifndef RESOURCES_H_
#define RESOURCES_H_

/* Includes */
#include <stdint.h>
#include "cmsis_os.h"


void initResources(void);
uint8_t takeResources(uint32_t res);
void releaseResources(uint32_t res);
uint8_t isResouresAvailable(uint32_t res);

#endif /* RESOURCES_H_ */



