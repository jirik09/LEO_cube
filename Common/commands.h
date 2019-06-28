/**
  *****************************************************************************
  * @file    commands.h
  * @author  Y3288231
  * @date    Dec 15, 2014
  * @brief   This file contains functions for parsing commands
  ***************************************************************************** 
*/ 
#ifndef COMMANDS_H_
#define COMMANDS_H_
// Constant definitions =======================================================
// Types definitions ==========================================================
typedef uint32_t command;


#define STR_ACK "ACK_"
#define STR_NACK "NACK"
#define STR_ERR "ERR_"

#define STR_SCOPE_OK "S_OK"
#define STR_GEN_OK "G_OK"
#define STR_GEN_NEXT "G_NX"

#ifdef USE_COUNTER
#define STR_CNT_ETR_DATA "ETRD"		// data from ETR measurement
#define STR_CNT_ETR_BUFF "ETRB"		// buffer itself

#define STR_CNT_REF_DATA "REFD"		// data from REF measurement
#define STR_CNT_REF_WARN "WARN"		// reference counter sample count warning

#define STR_CNT_IC1_DATA "IC1D"		// data from IC1 channel meas.
#define STR_CNT_IC2_DATA "IC2D"		// data from IC2 channel meas.

#define STR_CNT_TI_DATA  "TIDA"		// data from TI
#define STR_CNT_TI_TIMEOUT "TMOT"	// Timeout occured

#define STR_CNT_DUTY_CYCLE  "DUT1"
#define STR_CNT_PULSE_WIDTH "PWD1"
#endif //USE_COUNTER

#ifdef USE_LOG_ANLYS
//#define STR_LOG_ANLYS_USER_TRIGGER "LAUT"
#define STR_LOG_ANLYS_TRIGGER_POINTER "LATP"
#define STR_LOG_ANLYS_DATA_LENGTH "LADL"
#define STR_LOG_ANLYS_DATA "LADT"
#endif //USE_LOG_ANLYS


// Macro definitions ==========================================================
#define STRINGIFY(str) #str
#define GETCHAR(s) (int)(s)
#define BUILD_CMD(a) ((a[3] << 24)|(a[2] << 16)|(a[1] << 8)|(a[0]))

//#define STRINGIFY(str) #str
//#define BUILD_CMD(a) ((a[3] << 24)|(a[2] << 16)|(a[1] << 8)|(a[0]))

#define SWAP_UINT16(x) (((x & 0xff00) >> 8) | ((x & 0x00ff) << 8))
#define SWAP_UINT32(x) ( (x&0xff000000)>>24 | (x&0x00ff0000)>>8 | (x&0x0000ff00)<<8 | (x&0x000000ff)<<24 )


//#define REGISTER_CMD(name,value) CMD_##name=(int)BUILD_CMD(STRINGIFY(value))

//Command definitions
//Common commands

//Query commands
#define CMD_IDN 	               SWAP_UINT32(0x49444e3f)
#define CMD_VERSION               SWAP_UINT32(0x5645523f)
#define CMD_GET_CONFIG 						SWAP_UINT32(0x4346473f)
#define CMD_GET_REAL_FREQ 						SWAP_UINT32(0x4652513f)
#define CMD_GET_INPUTS 						SWAP_UINT32(0x494e503f)
#define CMD_IS_SHIELD 						SWAP_UINT32(0x53485f3f)
#define CMD_RESET_DEVICE 						SWAP_UINT32(0x52455321)
#define CMD_SCOPE 						SWAP_UINT32(0x4f534350)
#define CMD_GENERATOR 						SWAP_UINT32(0x47454e5f)
#define CMD_SYNC_PWM 						SWAP_UINT32(0x53594e50)
#define CMD_LOG_ANLYS 						SWAP_UINT32(0x4c4f4741)
#define CMD_COUNTER 						SWAP_UINT32(0x434e545f)
#define CMD_COMMS 						SWAP_UINT32(0x434f4d53)
#define CMD_SYSTEM 						SWAP_UINT32(0x53595354)
#define CMD_ERR 						SWAP_UINT32(0x4552525f)
#define CMD_ACK 						SWAP_UINT32(0x41434b5f)
#define CMD_NACK 						SWAP_UINT32(0x4e41434b)
#define CMD_END 						SWAP_UINT32(0x454e445f)
#define CMD_SCOPE_TRIG_MODE 						SWAP_UINT32(0x54524947)
#define CMD_SCOPE_TRIG_EDGE 						SWAP_UINT32(0x45444745)
#define CMD_SCOPE_SAMPLING_FREQ 						SWAP_UINT32(0x46524551)
#define CMD_SCOPE_DATA_LENGTH  						SWAP_UINT32(0x4c454e47)
#define CMD_SCOPE_TRIG_LEVEL 						SWAP_UINT32(0x4c45564c)
#define CMD_SCOPE_TRIG_CHANNEL 						SWAP_UINT32(0x54524348)
#define CMD_SCOPE_DATA_DEPTH 						SWAP_UINT32(0x44415441)
#define CMD_SCOPE_CHANNELS 						SWAP_UINT32(0x4348414e)
#define CMD_SCOPE_PRETRIGGER 						SWAP_UINT32(0x50524554)
#define CMD_SCOPE_START 						SWAP_UINT32(0x53545254)
#define CMD_SCOPE_STOP 						SWAP_UINT32(0x53544f50)
#define CMD_SCOPE_NEXT 						SWAP_UINT32(0x4e455854)
#define CMD_SCOPE_ADC_CHANNEL_SET 						SWAP_UINT32(0x415f4348)
#define CMD_SCOPE_ADC_CHANNEL_SET_DEFAULT 						SWAP_UINT32(0x41444546)
#define CMD_SCOPE_ADC_CHANNEL_SET_VREF 						SWAP_UINT32(0x41524546)
#define CMD_GEN_MODE 						SWAP_UINT32(0x4d4f4445)
#define CMD_GEN_DATA 						SWAP_UINT32(0x44415441)
#define CMD_GEN_SAMPLING_FREQ 						SWAP_UINT32(0x46524551)
#define CMD_GEN_OUTBUFF_ON 						SWAP_UINT32(0x425f4f4e)
#define CMD_GEN_OUTBUFF_OFF 						SWAP_UINT32(0x425f4f46)
#define CMD_GEN_DATA_LENGTH_CH1 						SWAP_UINT32(0x4c434831)
#define CMD_GEN_DATA_LENGTH_CH2 						SWAP_UINT32(0x4c434832)
#define CMD_GEN_CHANNELS 						SWAP_UINT32(0x4348414e)
#define CMD_GEN_START 						SWAP_UINT32(0x53545254)
#define CMD_GEN_STOP 						SWAP_UINT32(0x53544f50)
#define CMD_GEN_RESET 						SWAP_UINT32(0x52534554)
#define CMD_GET_PWM_CONFIG 						SWAP_UINT32(0x5043463f)
#define CMD_GEN_PWM_DEINIT 	 					SWAP_UINT32(0x47504449)
#define CMD_GEN_PWM_FREQ_PSC 						SWAP_UINT32(0x46505750)
#define CMD_GEN_PWM_FREQ_ARR 						SWAP_UINT32(0x46505741)
#define CMD_GEN_DAC_VAL 						SWAP_UINT32(0x4441435f)
#define CMD_SYNC_PWM_COMMAND 						SWAP_UINT32(0x53434f4d)
#define CMD_SYNC_PWM_CHAN_CONFIG 						SWAP_UINT32(0x43434f4e)
#define CMD_SYNC_PWM_CHAN_NUM 						SWAP_UINT32(0x434e554d)
#define CMD_SYNC_PWM_FREQ 						SWAP_UINT32(0x53465251)
#define CMD_SYNC_PWM_CHAN_STATE 						SWAP_UINT32(0x53535441)
#define CMD_SYNC_PWM_STEP 						SWAP_UINT32(0x53544550)
#define CMD_LOG_ANLYS_START 						SWAP_UINT32(0x53545254)
#define CMD_LOG_ANLYS_STOP 						SWAP_UINT32(0x53544f50)
#define CMD_LOG_ANLYS_INIT 						SWAP_UINT32(0x494e4954)
#define CMD_LOG_ANLYS_DEINIT 						SWAP_UINT32(0x4445494e)
#define CMD_LOG_ANLYS_POSTTRIG 						SWAP_UINT32(0x504f5354)
#define CMD_LOG_ANLYS_PRETRIG 						SWAP_UINT32(0x50524554)
#define CMD_LOG_ANLYS_SAMPLING_FREQ 						SWAP_UINT32(0x534d5046)
#define CMD_LOG_ANLYS_SAMPLES_NUM 						SWAP_UINT32(0x534d504e)
#define CMD_LOG_ANLYS_TRIGGER_MODE 						SWAP_UINT32(0x5452474d)
#define CMD_LOG_ANLYS_TRIGGER_EVENT 						SWAP_UINT32(0x54524745)
#define CMD_LOG_ANLYS_TRIGGER_CHANNEL 						SWAP_UINT32(0x54524743)
#define CMD_CNT_MODE 						SWAP_UINT32(0x4d4f4445)
#define CMD_CNT_START 						SWAP_UINT32(0x53545254)
#define CMD_CNT_STOP 						SWAP_UINT32(0x53544f50)
#define CMD_CNT_DEINIT 						SWAP_UINT32(0x4445494e)
#define CMD_CNT_GATE 						SWAP_UINT32(0x47415445)
#define CMD_CNT_SAMPLE_COUNT1 						SWAP_UINT32(0x42554631)
#define CMD_CNT_SAMPLE_COUNT2 						SWAP_UINT32(0x42554632)
#define CMD_CNT_PRESC1 						SWAP_UINT32(0x50524531)
#define CMD_CNT_PRESC2 						SWAP_UINT32(0x50524532)
#define CMD_CNT_DUTY_CYCLE 						SWAP_UINT32(0x44554359)
#define CMD_CNT_EVENT 						SWAP_UINT32(0x45564e54)
#define CMD_CNT_TIMEOUT_TIM 						SWAP_UINT32(0x54494d4f)
#define CMD_CNT_TI_MODE 						SWAP_UINT32(0x54494d44)
#define CMD_CNT_MULT_PSC 						SWAP_UINT32(0x5053435f)
#define CMD_CNT_MULT_ARR 						SWAP_UINT32(0x4152525f)
#define CMD_MODE_ETR 						SWAP_UINT32(0x4554525f)
#define CMD_MODE_IC 						SWAP_UINT32(0x49435f5f)
#define CMD_MODE_REF 						SWAP_UINT32(0x5245465f)
#define CMD_MODE_TI 						SWAP_UINT32(0x54495f5f)
#define CMD_GATE_100m 						SWAP_UINT32(0x3130306d)
#define CMD_GATE_500m 						SWAP_UINT32(0x3530306d)
#define CMD_GATE_1s 						SWAP_UINT32(0x31735f5f)
#define CMD_GATE_5s 						SWAP_UINT32(0x35735f5f)
#define CMD_GATE_10s 						SWAP_UINT32(0x3130735f)
#define CMD_PRESC1_1x 						SWAP_UINT32(0x31785f5f)
#define CMD_PRESC1_2x 						SWAP_UINT32(0x32785f5f)
#define CMD_PRESC1_4x 						SWAP_UINT32(0x34785f5f)
#define CMD_PRESC1_8x 						SWAP_UINT32(0x38785f5f)
#define CMD_PRESC2_1x 						SWAP_UINT32(0x31785f5f)
#define CMD_PRESC2_2x 						SWAP_UINT32(0x32785f5f)
#define CMD_PRESC2_4x 						SWAP_UINT32(0x34785f5f)
#define CMD_PRESC2_8x 						SWAP_UINT32(0x38785f5f)
#define CMD_EVENT_RF1   						SWAP_UINT32(0x5246315f)
#define CMD_EVENT_RF2   						SWAP_UINT32(0x5246325f)
#define CMD_EVENT_RO1   						SWAP_UINT32(0x524f315f)
#define CMD_EVENT_RO2   						SWAP_UINT32(0x524f325f)
#define CMD_EVENT_FO1   						SWAP_UINT32(0x464f315f)
#define CMD_EVENT_FO2 	 					SWAP_UINT32(0x464f325f)
#define CMD_EVENT_SEQ_AB 						SWAP_UINT32(0x53514142)
#define CMD_EVENT_SEQ_BA 						SWAP_UINT32(0x53514241)
#define CMD_DUTY_CYCLE_INIT_CH1 						SWAP_UINT32(0x44434931)
#define CMD_DUTY_CYCLE_INIT_CH2 						SWAP_UINT32(0x44434932)
#define CMD_DUTY_CYCLE_DEINIT_CH1 						SWAP_UINT32(0x44434431)
#define CMD_DUTY_CYCLE_DEINIT_CH2 						SWAP_UINT32(0x44434432)
#define CMD_DUTY_CYCLE_ENABLE 						SWAP_UINT32(0x4443455f)
#define CMD_DUTY_CYCLE_DISABLE 						SWAP_UINT32(0x4443585f)
#define CMD_MODE_EVENT_SEQUENCE_DEP 						SWAP_UINT32(0x53455144)
#define CMD_MODE_EVENT_SEQUENCE_INDEP 						SWAP_UINT32(0x53455149)
#define CMD_MODE_PWM 						SWAP_UINT32(0x50574d5f)
#define CMD_MODE_DAC 						SWAP_UINT32(0x4441435f)
#define CMD_SYNC_PWM_INIT 						SWAP_UINT32(0x494e4954)
#define CMD_SYNC_PWM_DEINIT 						SWAP_UINT32(0x44494e49)
#define CMD_SYNC_PWM_START 						SWAP_UINT32(0x53545254)
#define CMD_SYNC_PWM_STOP 						SWAP_UINT32(0x53544f50)
#define CMD_SYNC_PWM_STEP_ENABLE 						SWAP_UINT32(0x53544545)
#define CMD_SYNC_PWM_STEP_DISABLE 						SWAP_UINT32(0x53544544)
#define CMD_TRIG_MODE_AUTO 						SWAP_UINT32(0x4155544f)
#define CMD_TRIG_MODE_NORMAL 						SWAP_UINT32(0x4e4f524d)
#define CMD_TRIG_MODE_SINGLE 						SWAP_UINT32(0x53494e47)
#define CMD_TRIG_EDGE_RISING 						SWAP_UINT32(0x52495345)
#define CMD_TRIG_EDGE_FALLING 						SWAP_UINT32(0x46414c4c)
#define CMD_MODE_NORMAL 						SWAP_UINT32(0x4e4f524d)
#define CMD_MODE_AUTO 						SWAP_UINT32(0x4155544f)
#define CMD_MODE_AUTO_FAST 						SWAP_UINT32(0x465f415f)
#define CMD_MODE_SINGLE 						SWAP_UINT32(0x53494e47)
#define CMD_EDGE_RISING 						SWAP_UINT32(0x52495345)
#define CMD_EDGE_FALLING 						SWAP_UINT32(0x46414c4c)
#define CMD_FREQ_1K 						SWAP_UINT32(0x314b5f5f)
#define CMD_FREQ_2K 						SWAP_UINT32(0x324b5f5f)
#define CMD_FREQ_5K 						SWAP_UINT32(0x354b5f5f)
#define CMD_FREQ_10K 						SWAP_UINT32(0x31304b5f)
#define CMD_FREQ_20K 						SWAP_UINT32(0x32304b5f)
#define CMD_FREQ_50K 						SWAP_UINT32(0x35304b5f)
#define CMD_FREQ_100K 						SWAP_UINT32(0x3130304b)
#define CMD_FREQ_200K 						SWAP_UINT32(0x3230304b)
#define CMD_FREQ_500K 						SWAP_UINT32(0x3530304b)
#define CMD_FREQ_1M 						SWAP_UINT32(0x314d5f5f)
#define CMD_FREQ_2M 						SWAP_UINT32(0x324d5f5f)
#define CMD_FREQ_5M 						SWAP_UINT32(0x354d5f5f)
#define CMD_FREQ_10M 						SWAP_UINT32(0x31304d5f)
#define CMD_FREQ_MAX 						SWAP_UINT32(0x4d41585f)
#define CMD_SAMPLES_100 						SWAP_UINT32(0x3130305f)
#define CMD_SAMPLES_200 						SWAP_UINT32(0x3230305f)
#define CMD_SAMPLES_500 						SWAP_UINT32(0x3530305f)
#define CMD_SAMPLES_1K 						SWAP_UINT32(0x314b5f5f)
#define CMD_SAMPLES_2K 						SWAP_UINT32(0x324b5f5f)
#define CMD_SAMPLES_5K 						SWAP_UINT32(0x354b5f5f)
#define CMD_SAMPLES_10K 						SWAP_UINT32(0x31304b5f)
#define CMD_SAMPLES_20K 						SWAP_UINT32(0x32304b5f)
#define CMD_SAMPLES_50K 						SWAP_UINT32(0x35304b5f)
#define CMD_SAMPLES_100K 						SWAP_UINT32(0x3130304b)
#define CMD_DATA_DEPTH_12B 						SWAP_UINT32(0x3132425f)
#define CMD_DATA_DEPTH_10B 						SWAP_UINT32(0x3130425f)
#define CMD_DATA_DEPTH_8B 						SWAP_UINT32(0x38425f5f)
#define CMD_DATA_DEPTH_6B 						SWAP_UINT32(0x36425f5f)
#define CMD_CHANNELS_1 						SWAP_UINT32(0x3143485f)
#define CMD_CHANNELS_2 						SWAP_UINT32(0x3243485f)
#define CMD_CHANNELS_3 						SWAP_UINT32(0x3343485f)
#define CMD_CHANNELS_4 						SWAP_UINT32(0x3443485f)


//Counter modes



#define isCounterMode(CMD) (((CMD) == CMD_MODE_ETR) || \
                           ((CMD) == CMD_MODE_IC) || \
													 ((CMD) == CMD_MODE_REF) || \
													 ((CMD) == CMD_MODE_TI))

//Counter ETR sampling times


#define isCounterEtrGate(CMD) (((CMD) == CMD_GATE_100m) || \
															((CMD) == CMD_GATE_500m) || \
															((CMD) == CMD_GATE_1s) || \
															((CMD) == CMD_GATE_5s) || \
															((CMD) == CMD_GATE_10s))

//Counter IC prescaler 1




#define isCounterIcPresc1(CMD) (((CMD) == CMD_PRESC1_1x) || \
																((CMD) == CMD_PRESC1_2x) || \
																((CMD) == CMD_PRESC1_4x) || \
																((CMD) == CMD_PRESC1_8x))		

//Counter IC prescaler 2



#define isCounterIcPresc2(CMD) (((CMD) == CMD_PRESC2_1x) || \
																((CMD) == CMD_PRESC2_2x) || \
																((CMD) == CMD_PRESC2_4x) || \
																((CMD) == CMD_PRESC2_8x))		

//Counter IC pulse mode change configuration + TI (time interval)




#define isCounterIcTiEvent(CMD) (((CMD) == CMD_EVENT_RF1) || \
																((CMD) == CMD_EVENT_RF2) || \
																((CMD) == CMD_EVENT_RO1) || \
																((CMD) == CMD_EVENT_RO2)	|| \
																((CMD) == CMD_EVENT_FO1)	|| \
																((CMD) == CMD_EVENT_FO2)	|| \
																((CMD) == CMD_EVENT_SEQ_AB)	|| \
																((CMD) == CMD_EVENT_SEQ_BA))

//Counter IC duty cycle init/deinit



#define isCounterIcDutyCycle(CMD) (((CMD) == CMD_DUTY_CYCLE_INIT_CH1) || \
																	((CMD) == CMD_DUTY_CYCLE_INIT_CH2) || \
																	((CMD) == CMD_DUTY_CYCLE_DEINIT_CH1) || \
																	((CMD) == CMD_DUTY_CYCLE_DEINIT_CH2) || \
																	((CMD) == CMD_DUTY_CYCLE_ENABLE) || \
																	((CMD) == CMD_DUTY_CYCLE_DISABLE))

//Counter TI mode




#define isCounterTiMode(CMD) (((CMD) == CMD_MODE_EVENT_SEQUENCE_DEP) || \
															((CMD) == CMD_MODE_EVENT_SEQUENCE_INDEP))		


//Generator modes (NORMAL - DAC BUILD_CMD(STRINGIFY( ABNORMAL - PWM)




#define isGeneratorMode(CMD) (((CMD) == CMD_MODE_PWM) || \
															((CMD) == CMD_MODE_DAC))

//Sync PWM general commands



#define isSyncPwm(CMD) (((CMD) == CMD_SYNC_PWM_INIT) || \
												((CMD) == CMD_SYNC_PWM_DEINIT) || \
												((CMD) == CMD_SYNC_PWM_START) || \
												((CMD) == CMD_SYNC_PWM_STOP))						

//Sync PWM general commands


#define isSyncPwmStepMode(CMD) (((CMD) == CMD_SYNC_PWM_STEP_ENABLE) || \
																((CMD) == CMD_SYNC_PWM_STEP_DISABLE))		

//Logic analyzer trigger event.




#define isLogAnlysTriggerMode(CMD) (((CMD) == CMD_TRIG_MODE_AUTO) || \
																		((CMD) == CMD_TRIG_MODE_NORMAL) || \
																		((CMD) == CMD_TRIG_MODE_SINGLE))

//Logic analyzer trigger event.



#define isLogAnlysTriggerEvent(CMD) (((CMD) == CMD_TRIG_EDGE_RISING) || \
																		((CMD) == CMD_TRIG_EDGE_FALLING))		
	

//Scope tigger modes



#define isScopeTrigMode(CMD) (((CMD) == CMD_MODE_NORMAL) || \
                              ((CMD) == CMD_MODE_AUTO) || \
															((CMD) == CMD_MODE_AUTO_FAST) || \
                              ((CMD) == CMD_MODE_SINGLE))	

//Scope trigger edges




#define isScopeTrigEdge(CMD) (((CMD) == CMD_EDGE_RISING) || \
                              ((CMD) == CMD_EDGE_FALLING))	

//Scope sampling frequencies



#define isScopeFreq(CMD) (((CMD) == CMD_FREQ_1K) || \
                          ((CMD) == CMD_FREQ_2K) || \
                          ((CMD) == CMD_FREQ_5K) || \
                          ((CMD) == CMD_FREQ_10K) || \
                          ((CMD) == CMD_FREQ_20K) || \
                          ((CMD) == CMD_FREQ_50K) || \
                          ((CMD) == CMD_FREQ_100K) || \
                          ((CMD) == CMD_FREQ_200K) || \
                          ((CMD) == CMD_FREQ_500K) || \
                          ((CMD) == CMD_FREQ_1M) || \
                          ((CMD) == CMD_FREQ_2M) || \
                          ((CMD) == CMD_FREQ_5M) || \
													((CMD) == CMD_FREQ_10M) || \
                          ((CMD) == CMD_FREQ_MAX))	
													
//Scope data lengths


#define isScopeNumOfSamples(CMD) (((CMD) == CMD_SAMPLES_100) || \
																((CMD) == CMD_SAMPLES_200) || \
                                ((CMD) == CMD_SAMPLES_500) || \
                                ((CMD) == CMD_SAMPLES_1K) || \
                                ((CMD) == CMD_SAMPLES_2K) || \
                                ((CMD) == CMD_SAMPLES_5K) || \
                                ((CMD) == CMD_SAMPLES_10K) || \
                                ((CMD) == CMD_SAMPLES_20K) || \
																((CMD) == CMD_SAMPLES_50K) || \
																((CMD) == CMD_SAMPLES_100K))	


//Scope Data depths


#define isScopeDataDepth(CMD) (((CMD) == CMD_DATA_DEPTH_12B) || \
                               ((CMD) == CMD_DATA_DEPTH_10B) || \
                               ((CMD) == CMD_DATA_DEPTH_8B) || \
                               ((CMD) == CMD_DATA_DEPTH_6B))	
															 
//Number of channels



#define isChannel(CMD) (((CMD) == CMD_CHANNELS_1) || \
                              ((CMD) == CMD_CHANNELS_2) || \
															((CMD) == CMD_CHANNELS_3) || \
                              ((CMD) == CMD_CHANNELS_4))		

// Exported variables =========================================================
// Exported functions =========================================================



#endif /* COMMANDS_H_ */
