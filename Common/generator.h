/**
  *****************************************************************************
  * @file    generator.h
  * @author  Y3288231
  * @date    Dec 15, 2014
  * @brief   This file contains definitions and prototypes of oscilloscope functions
  ***************************************************************************** 
*/ 
#if defined(USE_GEN) || defined(USE_GEN_PWM) || defined(USE_GEN_PATTERN)
#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "stdint.h"
#include "mcu_config.h"

/** @addtogroup Generator
  * @{
  */

/** @defgroup Generator_Defines Generator Defines
  * @{
  */
#define DEFAULT_GENERATING_FREQ 1000

/** @defgroup Generator_Common_Enums Generator Common Enums for DAC and PWM.
  * @{
  */
typedef enum{
	GEN_DAC = 0,
	GEN_PWM,
	GEN_PATTERN,
	GEN_VOLTSOURCE //this is actually special case of DAC mode
}generatorMode;

typedef enum{
	GENERATOR_DAC = 0,
	GENERATOR_PWM,
	GENERATOR_PATTERN
}generatorModeState;

typedef enum{
	DAC_GEN_MODE = 0,
	DAC_VOLTSOURCE_MODE
}generatorDACMode;

typedef enum{
	GENERATOR_IDLE = 0,
	GENERATOR_RUN
}generatorState;
/**
  * @}
  */

/** @defgroup Generator_Common_Struct_Definition Generator Common Structure Definitions
  * @{
  */
typedef struct{
	uint16_t *bufferMemory;		
	uint32_t generatingFrequency[MAX_DAC_CHANNELS];
	uint32_t realGenFrequency[MAX_DAC_CHANNELS];
	double realPwmFreqCh1;
	double realPwmFreqCh2;
	generatorState state;	
	generatorModeState modeState;
	generatorDACMode DACMode;
	uint8_t numOfChannles;
	uint16_t *pChanMem[MAX_DAC_CHANNELS];					// buffer itself
	uint16_t oneChanSamples[MAX_DAC_CHANNELS];		// buffer size
	uint32_t maxOneChanSamples;
	uint16_t DAC_res;
}generatorTypeDef;
/**
  * @}
  */

// Externs ===========================================================
extern volatile generatorTypeDef generator;

/** @defgroup Generator_DAC_Prototypes Generator DAC Function Prototypes
  * @{
  */
void GeneratorTask(void const *argument);
void generatorSetDefault(void);
void genInit(void);

void genPwmInit(void);
void genPwmSetFrequency(double freq, uint8_t channel);
double genPwmGetRealFreq(void);

uint8_t genSetData(uint16_t index,uint8_t length,uint8_t chan);
uint8_t genSetFrequency(uint32_t freq,uint8_t chan);
void genSendRealSamplingFreq(void);
void genDataOKSendNext(void);
void genStatusOK(void);
uint32_t genGetRealSmplFreq(uint8_t chan);
uint8_t genSetLength(uint32_t length,uint8_t chan);
uint8_t genSetNumOfChannels(uint8_t chan);
uint8_t genSetDAC(uint16_t chann1,uint16_t chann2);
void genSetOutputBuffer(void);
void genUnsetOutputBuffer(void);
void genStart(void);
void genStop(void);
void genStopVoltSource(void);
void genReset(void);
/**
  * @}
  */

/** @defgroup Generator_PWM_Prototypes Generator PWM Function Prototypes
  * @{
  */
void genSetMode(uint8_t mode);
void generator_deinit(void);
//void genSetPwmFrequencyPSC(uint32_t pscVal, uint8_t chan);
//void genSetPwmFrequencyARR(uint32_t arrVal, uint8_t chan);

/**
  * @}
  */

#endif /* GENERATOR_H_ */

#endif // USE_GEN || USE_GEN_PWM

/**
  * @}
  */

