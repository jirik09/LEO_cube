/**
  *****************************************************************************
  * @file    counter.h
  * @author  HeyBirdie
  * @date    June 3, 2017
  * @brief   This file contains definitions and prototypes of counter functions
  ***************************************************************************** 
*/ 

#ifdef USE_COUNTER

#ifndef COUNTER_H_
#define COUNTER_H_

/** @addtogroup Counter
  * @{
  */

/* the buffer size of input capture mode has to be set at least to number 2 (two edges captured) */
/** @defgroup Counter_Defines Counter Defines.
  * @{
  */
#define IC12_BUFFER_SIZE	110
/**
  * @}
  */
	
/* Enums */
/** @defgroup Counter_Enums Counter Enums.
  * @{
  */
	
	/**
  * @brief  Counter modes 1. 
  */
typedef enum{
	ETR = 0,
	IC,
	TI,
	REF
}counterMode;

	/**
  * @brief  Counter modes 2. 
  */
typedef enum{
	COUNTER_IDLE = 0,
	COUNTER_ETR,
	COUNTER_IC,
	COUNTER_TI,
	COUNTER_REF		
}counterState;

/**
* @brief  Counter paused; prevents starting the counter by all matters.
*/
typedef enum{
	NO = 0,
	YES
}counterPaused;

/**
* @brief  Counter ETR mode Quantity selection.
*/
typedef enum{
	QUANTITY_FREQUENCY = 0,
	QUANTITY_PERIOD
}counterQuantity;

	/**
  * @brief  Reciprocal mode ISR pass through flag. 
  */
typedef enum{
	COUNTER_IRQ_IC = 3,
	COUNTER_IRQ_IC_PASS = 4
}counterIcChannel;

	/**
  * @brief  Reciprocal mode DC ch1/ch2 enabled/disabled flag. 
  */
typedef enum{
	DUTY_CYCLE_DISABLED = 0,
	DUTY_CYCLE_CH1_ENABLED,
	DUTY_CYCLE_CH2_ENABLED
}counterIcDutyCycle;

	/**
  * @brief  Direct mode (ETR) sample count change request from host flag. 
  */
typedef enum{	
	SAMPLE_COUNT_CHANGED = 0,
	SAMPLE_COUNT_NOT_CHANGED
}counterRefSmplCntChange;

/**
* @brief  Reciprocal mode ISR pass through flag.
*/
typedef enum{
	COUNTER_REF_SEND_DATA = 0,
	COUNTER_WARNING_FIRED
}counterRefWarning;

	/**
  * @brief  Time interval (TI) measurement edge rising falling selection. 
  */
typedef enum{	
	EVENT_RISING = 0,
	EVENT_FALLING
}counterEventTypeDef;

	/**
  * @brief  Time interval (TI) measurement flag. 
  */
typedef enum{
	CLEAR = 0,
	TIMEOUT,
	SEND_TI_DATA
}counterTiStateTypeDef;

	/**
  * @brief  Time interval (TI) measurement FAST event mode selection. 
  */
typedef enum{
	TI_MODE_EVENT_SEQUENCE_INDEP = 0,
	TI_MODE_FAST_EVENT_SEQUENCE_DEP
}counterTiModeTypeDef;

	/**
  * @brief  BIN semaphore used for IC and TI modes due to UART communication troubles. 
  */
typedef enum{
	BIN0 = 0,
	BIN1
}counterBin;

/**
  * @}
  */

/** @defgroup Counter_Struct_Definition Counter Structure Definitions
  * @{
  */
	/**
  * @brief  Direct (ETR) and reference measurement (REF) mode common struct.
	* The ETR struct (High frequency meas.) is also used for REF (precise meas.) mode as only 
	* the difference is in clock feeding of timer 4 and in the data sent to PC app.
  */
typedef struct{
	uint16_t arr;		// TIM4 ARR
	uint16_t psc;		// TIM4 PSC
	uint16_t arrTemp;
	uint16_t pscTemp;
	counterQuantity quantity;	// Frequency/Period

	uint8_t etrp;		// TIM2 ETRP
	uint32_t buffer;
	uint32_t refBuffer;
	uint16_t gateTime;	
	double freq;
}counterEtrTypeDef;

	/**
  * @brief  Reciprocal (IC) and time measurement (TI) mode common struct. 
	* IC mode (Low frequency)
  */
typedef struct{
	uint32_t arr;		// TIM2 ARR
	uint16_t psc;		// TIM2 PSC
	uint16_t ic1BufferSize;
	uint16_t ic2BufferSize;
	uint16_t ic1BufferSizeTemp;
	uint16_t ic2BufferSizeTemp;	
	uint32_t ic1buffer[IC12_BUFFER_SIZE];
	uint32_t ic2buffer[IC12_BUFFER_SIZE];
	double ic1freq;
	double ic2freq;
	double duty;
	double pulseWidth;
	uint8_t ic1psc;
	uint8_t ic2psc;
	uint8_t ic1pscTemp;
	uint8_t ic2pscTemp;
	counterQuantity quantityChan1;
	counterQuantity quantityChan2;

	uint32_t tiTimeout; // TI timeout part of IC struct
}counterIcTypeDef;

	/**
  * @brief  Counter common struct. 
  */
typedef struct{
	counterIcTypeDef counterIc;
	counterEtrTypeDef counterEtr;	
//  counterRefTypeDef counterRef;	-> ETR structure used for REF	counter
	counterState state;
	uint32_t tim4PrphClk;
	uint32_t tim2PrphClk;
	double qError;		// quantization error
	double tbError;		// time base error
	double qError2;		// quantization error
	double tbError2;		// time base error
	counterPaused paused;
	
	counterRefSmplCntChange sampleCntChange;
	counterRefWarning refWarning;
	counterIcChannel icChannel1;
	counterIcChannel icChannel2;
	counterIcDutyCycle icDutyCycle;
	counterTiStateTypeDef tiState;	
	counterTiModeTypeDef tiMode;
	counterEventTypeDef eventChan1;
	counterEventTypeDef eventChan2;
	counterBin bin;
	counterBin abba; 		// TI mode events sequence t_AB or t_BA
}counterTypeDef;
/**
  * @}
  */

/** @defgroup Counter_Prototypes Function Prototypes
  * @{
  */
void CounterTask(void const *argument);

/* Counter general functions */
void counterInitETR(void);
void counterInitIC(void);
void counterInitREF(void);
void counterInitTI(void);
void counter_deinit(void);
void counterSendStart(void);
void counterSendStop(void);
void counterPause(void);
void counterUnpause(void);
void counterStart(void);
void counterStop(void);
void counterDeinit(void);
void counterEtrRefSetDefault(void);
void counterIcTiSetDefault(void);
void counterSetMode(uint8_t mode);
uint8_t counterSetQuantity(uint8_t quant);
uint8_t counterSetIc1Quantity(uint8_t quant);
uint8_t counterSetIc2Quantity(uint8_t quant);

/* ETR mode functions */
uint8_t counterSetEtrGate(uint16_t gateTime);
void counterGateConfig(uint16_t gateTime);
void counterSetQuantityFreq(void);
void counterSetQuantityPer(void);
void counterSetIc1QuantityFreq(void);
void counterSetIc1QuantityPer(void);
void counterSetIc2QuantityFreq(void);
void counterSetIc2QuantityPer(void);

/* ETR error calculations */
double counterEtrCalculateQuantError(float gateFreq);
double counterEtrCalculateTimeBaseError(void);
double counterIcCalculateQuantError(int icChannel);
double counterIcCalculateTimeBaseError(int icChannel);

/* IC mode functions */
uint8_t counterSetIc1SampleCount(uint16_t buffer);
uint8_t counterSetIc2SampleCount(uint16_t buffer);
uint8_t counterSetIc1Prescaler(uint16_t presc);
uint8_t counterSetIc2Prescaler(uint16_t presc);
uint8_t counterSetIcTi1_RisingFalling(void);
uint8_t counterSetIcTi2_RisingFalling(void);
uint8_t counterSetIcTi1_Rising(void);
uint8_t counterSetIcTi2_Rising(void);
uint8_t counterSetIcTi1_Falling(void);
uint8_t counterSetIcTi2_Falling(void);
uint8_t counterSetTiSequence_AB(void);
uint8_t counterSetTiSequence_BA(void);
void counterIcRestartMeasCh1(void);
void counterIcRestartMeasCh2(void);
void counterIc1BufferConfig(uint16_t ic1buffSize);
void counterIc2BufferConfig(uint16_t ic2buffSize);
void counterIcProcess(void);
void counterIcDutyCycleProcess(void);

uint8_t counterIc1DutyCycleEnable(void);
uint8_t counterIc2DutyCycleEnable(void);
uint8_t counterIcDutyCycleDisable(void);

/* TI mode functions */
void counterTiProcess(void);
uint8_t counterSetTiTimeout(uint32_t timeout);
uint8_t counterSetTiMode_Independent(void);
uint8_t counterSetTiMode_Dependent(void);

/* REF mode functions */
void counterSetRefPsc(uint16_t psc);
void counterSetRefArr(uint16_t arr);
uint8_t counterSetRefSampleCount(uint32_t sampleCount);

void COUNTER_IC_TIM_Elapse(void);
void counterPeriodElapsedCallback(TIM_HandleTypeDef *htim);

extern volatile counterTypeDef counter;

extern DMA_HandleTypeDef hdma_tim2_up;
extern DMA_HandleTypeDef hdma_tim2_ch1;
extern DMA_HandleTypeDef hdma_tim2_ch2_ch4;

/**
  * @}
  */
	
#endif /* COUNTER_H_ */

#endif //USE_COUNTER

/**
  * @}
  */
	
