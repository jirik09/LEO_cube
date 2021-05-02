/**
 *****************************************************************************
 * @file    resources.c
 * @author  Jirik
 * @date    1.5.2021
 * @brief   This file contains sync PWM generator functions.
 *****************************************************************************
 */

// Includes ===================================================================
#include "resources.h"
#include "err_list.h"

uint32_t resourcesInUse;
static xSemaphoreHandle resourceMutex;

void initResources(void){
	resourceMutex = xSemaphoreCreateRecursiveMutex();
	resourcesInUse = 0;
}

// 0 - success, 1 - error
uint8_t takeResources(uint32_t res){
    if(resourcesInUse & res){
    	return RESOURCES_ERROR;
    }
    xSemaphoreTakeRecursive(resourceMutex, portMAX_DELAY);
    resourcesInUse = resourcesInUse | res;
    xSemaphoreGiveRecursive(resourceMutex);
    return 0;
}

void releaseResources(uint32_t res){
	xSemaphoreTakeRecursive(resourceMutex, portMAX_DELAY);
	resourcesInUse = (resourcesInUse ^ res) & resourcesInUse;
	xSemaphoreGiveRecursive(resourceMutex);
}


//0 - not available, 1 - yes resources available
uint8_t isResouresAvailable(uint32_t res){
    if(resourcesInUse & res){
    	return 0;
    }
    return 1;
}



/****************************** END MY FRIEND ******************************/
