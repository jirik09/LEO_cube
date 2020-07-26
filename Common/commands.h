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

#define STR_SYSTEM "SYST"
#define STR_SCOPE "OSCP"
#define STR_COUNTER "CNT_"
#define STR_GENERATOR "GEN_"
#define STR_LOGIC_ANLYS "LAN_"
#define STR_SYNC_PWM "SPWM"
#define STR_DELIMITER 0xCAFEFADE

#define STR_CONFIG "CFG_"

#define STR_ACK "ACK_"
#define STR_NACK "NACK"
#define STR_ERR "ERR_"
#define STR_UNKNOWN_MSG "XMSG"

#define STR_SCOPE_OK "S_OK"
#define STR_GEN_OK "G_OK"
#define STR_GEN_NEXT "G_NX"

#ifdef USE_SCOPE
#define STR_SCOPE_TRIG "TRIG"
#define STR_SCOPE_SMPL "SMPL"
#endif //USE_SCOPE

#ifdef USE_GEN_PWM
#define STR_GEN_PWM_REAL_FREQ_CH1 "GPR1"
#define STR_GEN_PWM_REAL_FREQ_CH2 "GPR2"
#define STR_GEN_SIGNAL_REAL_SAMPLING_FREQ_CH1 "GSR1"
#define STR_GEN_SIGNAL_REAL_SAMPLING_FREQ_CH2 "GSR2"
#endif //USE_GEN_PWM

#ifdef USE_SYNC_PWM
#define STR_SYNC_PWM_REAL_FREQ "SPRF"
#endif // USE_SYNC_PWM

#ifdef USE_COUNTER
#define STR_CNT_ETR_DATA "ETRD"		// data from ETR measurement
#define STR_CNT_ETR_BUFF "ETRB"		// buffer itself
/*
#define STR_CNT_ETR_QUANT_ERR "QERR"		// quantization error
#define STR_CNT_ETR_TIMEBASE_ERR "TERR"		// time base error
*/

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

#define REGISTER_CMD(name,value) CMD_##name=(int)BUILD_CMD(STRINGIFY(value))

//Command definitions
//Common commands

//Query commands
#define CMD_IDN 	               SWAP_UINT32(0x49444e3f)								//	IDN?
#define CMD_VERSION               SWAP_UINT32(0x5645523f)									//	VER?
#define CMD_GET_CONFIG 						SWAP_UINT32(0x4346473f)			//	CFG?
#define CMD_GET_REAL_FREQ 						SWAP_UINT32(0x4652513f)			//	FRQ?
#define CMD_GET_INPUTS 						SWAP_UINT32(0x494e503f)			//	INP?
#define CMD_IS_SHIELD 						SWAP_UINT32(0x53485f3f)			//	SH_?
#define CMD_RESET_DEVICE 						SWAP_UINT32(0x52455321)			//	RES!
#define CMD_SCOPE 						SWAP_UINT32(0x4f534350)			//	OSCP
#define CMD_GENERATOR 						SWAP_UINT32(0x47454e5f)			//	GEN_
#define CMD_SYNC_PWM 						SWAP_UINT32(0x53594e50)			//	SYNP
#define CMD_LOG_ANLYS 						SWAP_UINT32(0x4c4f4741)			//	LOGA
#define CMD_COUNTER 						SWAP_UINT32(0x434e545f)			//	CNT_
#define CMD_COMMS 						SWAP_UINT32(0x434f4d53)			//	COMS
#define CMD_SYSTEM 						SWAP_UINT32(0x53595354)			//	SYST
#define CMD_ERR 						SWAP_UINT32(0x4552525f)			//	ERR_
#define CMD_ACK 						SWAP_UINT32(0x41434b5f)			//	ACK_
#define CMD_NACK 						SWAP_UINT32(0x4e41434b)			//	NACK
#define CMD_END 						SWAP_UINT32(0x454e445f)			//	END_
#define CMD_SCOPE_TRIG_MODE 						SWAP_UINT32(0x54524947)			//	TRIG
#define CMD_SCOPE_TRIG_EDGE 						SWAP_UINT32(0x45444745)			//	EDGE
#define CMD_SCOPE_SAMPLING_FREQ 						SWAP_UINT32(0x46524551)			//	FREQ
#define CMD_SCOPE_DATA_LENGTH  						SWAP_UINT32(0x4c454e47)			//	LENG
#define CMD_SCOPE_TRIG_LEVEL 						SWAP_UINT32(0x4c45564c)			//	LEVL
#define CMD_SCOPE_TRIG_CHANNEL 						SWAP_UINT32(0x54524348)			//	TRCH
#define CMD_SCOPE_DATA_DEPTH 						SWAP_UINT32(0x44415441)			//	DATA
#define CMD_SCOPE_CHANNELS 						SWAP_UINT32(0x4348414e)			//	CHAN
#define CMD_SCOPE_PRETRIGGER 						SWAP_UINT32(0x50524554)			//	PRET
#define CMD_SCOPE_START 						SWAP_UINT32(0x53545254)			//	STRT
#define CMD_SCOPE_STOP 						SWAP_UINT32(0x53544f50)			//	STOP
#define CMD_SCOPE_NEXT 						SWAP_UINT32(0x4e455854)			//	NEXT
#define CMD_SCOPE_ADC_CHANNEL_SET 						SWAP_UINT32(0x415f4348)			//	A_CH
#define CMD_SCOPE_ADC_CHANNEL_SET_DEFAULT 						SWAP_UINT32(0x41444546)			//	ADEF
#define CMD_SCOPE_ADC_CHANNEL_SET_VREF 						SWAP_UINT32(0x41524546)			//	AREF
#define CMD_GEN_MODE 						SWAP_UINT32(0x4d4f4445)			//	MODE
#define CMD_GEN_DATA 						SWAP_UINT32(0x44415441)			//	DATA
#define CMD_GEN_SAMPLING_FREQ 						SWAP_UINT32(0x46524551)			//	FREQ
#define CMD_GEN_OUTBUFF_ON 						SWAP_UINT32(0x425f4f4e)			//	B_ON
#define CMD_GEN_OUTBUFF_OFF 						SWAP_UINT32(0x425f4f46)			//	B_OF
#define CMD_GEN_DATA_LENGTH_CH1 						SWAP_UINT32(0x4c434831)			//	LCH1
#define CMD_GEN_DATA_LENGTH_CH2 						SWAP_UINT32(0x4c434832)			//	LCH2
#define CMD_GEN_CHANNELS 						SWAP_UINT32(0x4348414e)			//	CHAN
#define CMD_GEN_START 						SWAP_UINT32(0x53545254)			//	STRT
#define CMD_GEN_STOP 						SWAP_UINT32(0x53544f50)			//	STOP
#define CMD_GEN_RESET 						SWAP_UINT32(0x52534554)			//	RSET
#define CMD_GET_PWM_CONFIG 						SWAP_UINT32(0x5043463f)			//	PCF?
#define CMD_GEN_PWM_DEINIT 	 					SWAP_UINT32(0x47504449)			//	GPDI
//#define CMD_GEN_PWM_FREQ_PSC 						SWAP_UINT32(0x46505750)			//	FPWP
//#define CMD_GEN_PWM_FREQ_ARR 						SWAP_UINT32(0x46505741)			//	FPWA
#define CMD_GEN_PWM_FREQ_CH1 					SWAP_UINT32(0x47465231)				//	GFR1
#define CMD_GEN_PWM_FREQ_CH2					SWAP_UINT32(0x47465232)				//	GFR2
#define CMD_GEN_DAC_VAL 						SWAP_UINT32(0x4441435f)			//	DAC_
#define CMD_SYNC_PWM_COMMAND 						SWAP_UINT32(0x53434f4d)			//	SCOM
#define CMD_SYNC_PWM_CHAN_CONFIG 						SWAP_UINT32(0x43434f4e)			//	CCON
#define CMD_SYNC_PWM_CHAN_NUM 						SWAP_UINT32(0x434e554d)			//	CNUM
#define CMD_SYNC_PWM_FREQ 						SWAP_UINT32(0x53465251)			//	SFRQ
#define CMD_SYNC_PWM_CHAN_STATE 						SWAP_UINT32(0x53535441)			//	SSTA
#define CMD_SYNC_PWM_STEP 						SWAP_UINT32(0x53544550)			//	STEP
#define CMD_LOG_ANLYS_START 						SWAP_UINT32(0x53545254)			//	STRT
#define CMD_LOG_ANLYS_STOP 						SWAP_UINT32(0x53544f50)			//	STOP
#define CMD_LOG_ANLYS_INIT 						SWAP_UINT32(0x494e4954)			//	INIT
#define CMD_LOG_ANLYS_DEINIT 						SWAP_UINT32(0x4445494e)			//	DEIN
#define CMD_LOG_ANLYS_POSTTRIG 						SWAP_UINT32(0x504f5354)			//	POST
#define CMD_LOG_ANLYS_PRETRIG 						SWAP_UINT32(0x50524554)			//	PRET
#define CMD_LOG_ANLYS_SAMPLING_FREQ 						SWAP_UINT32(0x534d5046)			//	SMPF
#define CMD_LOG_ANLYS_SAMPLES_NUM 						SWAP_UINT32(0x534d504e)			//	SMPN
#define CMD_LOG_ANLYS_TRIGGER_MODE 						SWAP_UINT32(0x5452474d)			//	TRGM
#define CMD_LOG_ANLYS_TRIGGER_EVENT 						SWAP_UINT32(0x54524745)			//	TRGE
#define CMD_LOG_ANLYS_TRIGGER_CHANNEL 						SWAP_UINT32(0x54524743)			//	TRGC
#define CMD_CNT_MODE 						SWAP_UINT32(0x4d4f4445)			//	MODE
#define CMD_CNT_START 						SWAP_UINT32(0x53545254)			//	STRT
#define CMD_CNT_STOP 						SWAP_UINT32(0x53544f50)			//	STOP
#define CMD_CNT_DEINIT 						SWAP_UINT32(0x4445494e)			//	DEIN
#define CMD_CNT_GATE 						SWAP_UINT32(0x47415445)			//	GATE
#define CMD_CNT_SAMPLE_COUNT1 						SWAP_UINT32(0x42554631)			//	BUF1
#define CMD_CNT_SAMPLE_COUNT2 						SWAP_UINT32(0x42554632)			//	BUF2
#define CMD_CNT_PRESC1 						SWAP_UINT32(0x50524531)			//	PRE1
#define CMD_CNT_PRESC2 						SWAP_UINT32(0x50524532)			//	PRE2
#define CMD_CNT_DUTY_CYCLE 						SWAP_UINT32(0x44554359)			//	DUCY
#define CMD_CNT_EVENT 						SWAP_UINT32(0x45564e54)			//	EVNT
#define CMD_CNT_TIMEOUT_TIM 						SWAP_UINT32(0x54494d4f)			//	TIMO
#define CMD_CNT_TI_MODE 						SWAP_UINT32(0x54494d44)			//	TIMD
//#define CMD_CNT_MULT_PSC 						SWAP_UINT32(0x5053435f)			//	PSC_
//#define CMD_CNT_MULT_ARR 						SWAP_UINT32(0x4152525f)			//	ARR_
#define CMD_CNT_REF_SAMPLE_COUNT 				SWAP_UINT32(0x5253435f)					//	RSC_
#define CMD_CNT_QUANTITY						SWAP_UINT32(0x5155414e)			// QUAN
#define CMD_CNT_QUANTITY_FREQUENCY				SWAP_UINT32(0x51465245)			//	QFRE
#define CMD_CNT_QUANTITY_PERIOD					SWAP_UINT32(0x51504552)			//	QPER
#define CMD_MODE_ETR 						SWAP_UINT32(0x4554525f)			//	ETR_
#define CMD_MODE_IC 						SWAP_UINT32(0x49435f5f)			//	IC__
#define CMD_MODE_REF 						SWAP_UINT32(0x5245465f)			//	REF_
#define CMD_MODE_TI 						SWAP_UINT32(0x54495f5f)			//	TI__
#define CMD_GATE_100m 						SWAP_UINT32(0x3130306d)			//	100m
#define CMD_GATE_500m 						SWAP_UINT32(0x3530306d)			//	500m
#define CMD_GATE_1s 						SWAP_UINT32(0x31735f5f)			//	1s__
#define CMD_GATE_5s 						SWAP_UINT32(0x35735f5f)			//	5s__
#define CMD_GATE_10s 						SWAP_UINT32(0x3130735f)			//	10s_
#define CMD_PRESC1_1x 						SWAP_UINT32(0x31785f5f)			//	1x__
#define CMD_PRESC1_2x 						SWAP_UINT32(0x32785f5f)			//	2x__
#define CMD_PRESC1_4x 						SWAP_UINT32(0x34785f5f)			//	4x__
#define CMD_PRESC1_8x 						SWAP_UINT32(0x38785f5f)			//	8x__
#define CMD_PRESC2_1x 						SWAP_UINT32(0x31785f5f)			//	1x__
#define CMD_PRESC2_2x 						SWAP_UINT32(0x32785f5f)			//	2x__
#define CMD_PRESC2_4x 						SWAP_UINT32(0x34785f5f)			//	4x__
#define CMD_PRESC2_8x 						SWAP_UINT32(0x38785f5f)			//	8x__
#define CMD_EVENT_RF1   						SWAP_UINT32(0x5246315f)			//	RF1_
#define CMD_EVENT_RF2   						SWAP_UINT32(0x5246325f)			//	RF2_
#define CMD_EVENT_RO1   						SWAP_UINT32(0x524f315f)			//	RO1_
#define CMD_EVENT_RO2   						SWAP_UINT32(0x524f325f)			//	RO2_
#define CMD_EVENT_FO1   						SWAP_UINT32(0x464f315f)			//	FO1_
#define CMD_EVENT_FO2 	 					SWAP_UINT32(0x464f325f)			//	FO2_
#define CMD_EVENT_SEQ_AB 						SWAP_UINT32(0x53514142)			//	SQAB
#define CMD_EVENT_SEQ_BA 						SWAP_UINT32(0x53514241)			//	SQBA
#define CMD_DUTY_CYCLE_INIT_CH1 						SWAP_UINT32(0x44434931)			//	DCI1
#define CMD_DUTY_CYCLE_INIT_CH2 						SWAP_UINT32(0x44434932)			//	DCI2
#define CMD_DUTY_CYCLE_DEINIT_CH1 						SWAP_UINT32(0x44434431)			//	DCD1
#define CMD_DUTY_CYCLE_DEINIT_CH2 						SWAP_UINT32(0x44434432)			//	DCD2
#define CMD_DUTY_CYCLE_ENABLE 						SWAP_UINT32(0x4443455f)			//	DCE_
#define CMD_DUTY_CYCLE_DISABLE 						SWAP_UINT32(0x4443585f)			//	DCX_
#define CMD_MODE_EVENT_SEQUENCE_DEP 						SWAP_UINT32(0x53455144)			//	SEQD
#define CMD_MODE_EVENT_SEQUENCE_INDEP 						SWAP_UINT32(0x53455149)			//	SEQI
#define CMD_MODE_PWM 						SWAP_UINT32(0x50574d5f)			//	PWM_
#define CMD_MODE_DAC 						SWAP_UINT32(0x4441435f)			//	DAC_
#define CMD_SYNC_PWM_INIT 						SWAP_UINT32(0x494e4954)			//	INIT
#define CMD_SYNC_PWM_DEINIT 						SWAP_UINT32(0x44494e49)			//	DINI
#define CMD_SYNC_PWM_START 						SWAP_UINT32(0x53545254)			//	STRT
#define CMD_SYNC_PWM_STOP 						SWAP_UINT32(0x53544f50)			//	STOP
#define CMD_SYNC_PWM_STEP_ENABLE 						SWAP_UINT32(0x53544545)			//	STEE
#define CMD_SYNC_PWM_STEP_DISABLE 						SWAP_UINT32(0x53544544)			//	STED
#define CMD_TRIG_MODE_AUTO 						SWAP_UINT32(0x4155544f)			//	AUTO
#define CMD_TRIG_MODE_NORMAL 						SWAP_UINT32(0x4e4f524d)			//	NORM
#define CMD_TRIG_MODE_SINGLE 						SWAP_UINT32(0x53494e47)			//	SING
#define CMD_TRIG_EDGE_RISING 						SWAP_UINT32(0x52495345)			//	RISE
#define CMD_TRIG_EDGE_FALLING 						SWAP_UINT32(0x46414c4c)			//	FALL
#define CMD_MODE_NORMAL 						SWAP_UINT32(0x4e4f524d)			//	NORM
#define CMD_MODE_AUTO 						SWAP_UINT32(0x4155544f)			//	AUTO
#define CMD_MODE_AUTO_FAST 						SWAP_UINT32(0x465f415f)			//	F_A_
#define CMD_MODE_SINGLE 						SWAP_UINT32(0x53494e47)			//	SING
#define CMD_EDGE_RISING 						SWAP_UINT32(0x52495345)			//	RISE
#define CMD_EDGE_FALLING 						SWAP_UINT32(0x46414c4c)			//	FALL
#define CMD_FREQ_1K 						SWAP_UINT32(0x314b5f5f)			//	1K__
#define CMD_FREQ_2K 						SWAP_UINT32(0x324b5f5f)			//	2K__
#define CMD_FREQ_5K 						SWAP_UINT32(0x354b5f5f)			//	5K__
#define CMD_FREQ_10K 						SWAP_UINT32(0x31304b5f)			//	10K_
#define CMD_FREQ_20K 						SWAP_UINT32(0x32304b5f)			//	20K_
#define CMD_FREQ_50K 						SWAP_UINT32(0x35304b5f)			//	50K_
#define CMD_FREQ_100K 						SWAP_UINT32(0x3130304b)			//	100K
#define CMD_FREQ_200K 						SWAP_UINT32(0x3230304b)			//	200K
#define CMD_FREQ_500K 						SWAP_UINT32(0x3530304b)			//	500K
#define CMD_FREQ_1M 						SWAP_UINT32(0x314d5f5f)			//	1M__
#define CMD_FREQ_2M 						SWAP_UINT32(0x324d5f5f)			//	2M__
#define CMD_FREQ_5M 						SWAP_UINT32(0x354d5f5f)			//	5M__
#define CMD_FREQ_10M 						SWAP_UINT32(0x31304d5f)			//	10M_
#define CMD_FREQ_MAX 						SWAP_UINT32(0x4d41585f)			//	MAX_
#define CMD_SAMPLES_100 						SWAP_UINT32(0x3130305f)			//	100_
#define CMD_SAMPLES_200 						SWAP_UINT32(0x3230305f)			//	200_
#define CMD_SAMPLES_500 						SWAP_UINT32(0x3530305f)			//	500_
#define CMD_SAMPLES_1K 						SWAP_UINT32(0x314b5f5f)			//	1K__
#define CMD_SAMPLES_2K 						SWAP_UINT32(0x324b5f5f)			//	2K__
#define CMD_SAMPLES_5K 						SWAP_UINT32(0x354b5f5f)			//	5K__
#define CMD_SAMPLES_10K 						SWAP_UINT32(0x31304b5f)			//	10K_
#define CMD_SAMPLES_20K 						SWAP_UINT32(0x32304b5f)			//	20K_
#define CMD_SAMPLES_50K 						SWAP_UINT32(0x35304b5f)			//	50K_
#define CMD_SAMPLES_100K 						SWAP_UINT32(0x3130304b)			//	100K
#define CMD_DATA_DEPTH_12B 						SWAP_UINT32(0x3132425f)			//	12B_
#define CMD_DATA_DEPTH_10B 						SWAP_UINT32(0x3130425f)			//	10B_
#define CMD_DATA_DEPTH_8B 						SWAP_UINT32(0x38425f5f)			//	8B__
#define CMD_DATA_DEPTH_6B 						SWAP_UINT32(0x36425f5f)			//	6B__
#define CMD_CHANNELS_1 						SWAP_UINT32(0x3143485f)			//	1CH_
#define CMD_CHANNELS_2 						SWAP_UINT32(0x3243485f)			//	2CH_
#define CMD_CHANNELS_3 						SWAP_UINT32(0x3343485f)			//	3CH_
#define CMD_CHANNELS_4 						SWAP_UINT32(0x3443485f)			//	4CH_

//Counter modes

#define isCounterMode(CMD) (((CMD) == CMD_MODE_ETR) || \
		((CMD) == CMD_MODE_IC) || \
		((CMD) == CMD_MODE_REF) || \
		((CMD) == CMD_MODE_TI))

//Counter ETR quantity

#define isCounterQuantity(CMD) (((CMD) == CMD_CNT_QUANTITY_FREQUENCY) || \
		((CMD) == CMD_CNT_QUANTITY_PERIOD))

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
		((CMD) <  0x07FFFFFF) || \
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
