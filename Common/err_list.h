/*
  *****************************************************************************
  * @file    err_list.h
  * @author  Y3288231
  * @date    Sep, 2015
  * @brief   This file contains list of errors
  ***************************************************************************** 
*/ 
#ifndef ERR_LIST_H_
#define ERR_LIST_H_

// List of possible Errors ====================================================
#define ERROR_PREFIX 'E'


#define SCOPE_INVALID_FEATURE 50
#define SCOPE_TOO_MANY_CHANNELS 51
#define SCOPE_INVALID_FEATURE_PARAM 54
#define SCOPE_UNSUPPORTED_RESOLUTION 55
#define SCOPE_INVALID_TRIGGER_CHANNEL 56
#define SCOPE_INVALID_SAMPLING_FREQ 57
#define BUFFER_SIZE_ERR 58 // Buffer size exceeded located memory
#define SCOPE_TRIGGER_FORCED_TO_CH1 59

#define SCOPE_INVALID_ADC_CHANNEL 61

#define GEN_INVALID_FEATURE 100
#define GEN_INVALID_STATE 101
#define GEN_OUT_OF_MEMORY 102 //writing data for generating out of memory
#define GEN_BUFFER_SIZE_ERR 103
#define GEN_MISSING_DATA 104 //expected more data
#define GEN_TO_HIGH_FREQ 105
#define GEN_FREQ_IS_INACCURATE 107
#define GEN_FREQ_MISMATCH 108
#define GEN_INVALID_DATA 109
#define GEN_DAC_SET_ERROR 110
#define GEN_VOLT_INVALID_FEATURE 115

#define CNT_FEATURE_MODE_MISMATCH 130

#define COUNTER_INVALID_FEATURE_PARAM 150
#define SYNC_PWM_INVALID_FEATURE 151
#define LOG_ANLYS_INVALID_FEATURE 152

#define SYSTEM_INVALID_FEATURE 1
#define COMMS_INVALID_FEATURE 2

#define UNSUPORTED_FUNCTION_ERR_STR "E9" // Unsupported function
#define UNSUPORTED_FUNCTION_ERR 9
#define UNKNOW_ERROR 255


#endif /* ERR_LIST_H_ */

