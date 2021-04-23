/**
  *****************************************************************************
  * @file    messages.h
  * @author  HeyBirdie
  * @date    Jul 12, 2019
  * @brief   This file contains functions for parsing commands
  *****************************************************************************
*/
#ifndef MESSAGES_H_
#define MESSAGES_H_

enum messages{
	MSG_DEVICE_IDN,
	MSG_SYSTEM_SPEC,
	MSG_SHIELD_AVAIL,

	MSG_CNT_CONFIG,
	MSG_CNT_SET_DIRECT_MODE,
	MSG_CNT_SET_RECIPROCAL_MODE,
	MSG_CNT_SET_REFERENCE_MODE,
	MSG_CNT_SET_INTERVAL_MODE,
	MSG_CNT_SET_QUANTITY_FREQUENCY,
	MSG_CNT_SET_QUANTITY_PERIOD,
	MSG_CNT_SET_IC1_QUANTITY_FREQUENCY,
	MSG_CNT_SET_IC1_QUANTITY_PERIOD,
	MSG_CNT_SET_IC2_QUANTITY_FREQUENCY,
	MSG_CNT_SET_IC2_QUANTITY_PERIOD,
	MSG_CNT_RESTART_IC1,
	MSG_CNT_RESTART_IC2,
	MSG_CNT_START,
	MSG_CNT_STOP,
	MSG_CNT_DEINIT,
	MSG_CNT_SET_GATE_TIME,
	MSG_CNT_SEND_DATA,

	MSG_SCOPE_SPEC,
	MSG_SCOPE_INPUTS,
	MSG_SCOPE_RESTART,
	MSG_SCOPE_START,
	MSG_SCOPE_STOP,
	MSG_SCOPE_DATA_SENT,
	MSG_SCOPE_DATA_READY,
	MSG_SCOPE_TRIGGER,
	MSG_SCOPE_SMPL_STARTED,

	MSG_LOGAN_CONFIG,
	MSG_LOGAN_INIT,
	MSG_LOGAN_DEINIT,
	MSG_LOGAN_START,
	MSG_LOGAN_STOP,
	MSG_LOGAN_SAMPLING_END,
	MSG_LOGAN_SEND_DATA,

	MSG_DAC_CONFIG,
	MSG_GEN_CONFIG,
	MSG_GEN_PWM_CONFIG,
	MSG_GEN_VOLTSOURCE_MODE,
	MSG_GEN_DAC_MODE,
	MSG_GEN_PWM_MODE,
	MSG_GEN_PWM_REAL_FREQ_CH1,
	MSG_GEN_PWM_REAL_FREQ_CH2,
	MSG_GEN_START,
	MSG_GEN_STOP,
	MSG_GEN_DEINIT,
	MSG_GEN_SIGNAL_REAL_SAMPLING_FREQ_CH1,
	MSG_GEN_SIGNAL_REAL_SAMPLING_FREQ_CH2,
	MSG_GEN_NEXT,
	MSG_GEN_OK,

	MSG_SYNCPWM_CONFIG,
	MSG_SYNCPWM_INIT,
	MSG_SYNCPWM_DEINIT,
	MSG_SYNCPWM_START,
	MSG_SYNCPWM_STOP,
	MSG_SYNCPWM_REAL_FREQ_CH1,
	MSG_SYNCPWM_REAL_FREQ_CH2,
	MSG_SYNCPWM_REAL_FREQ_CH3,
	MSG_SYNCPWM_REAL_FREQ_CH4,
	MSG_SYNCPWM_OPM_PERIOD_ELAPSED,

	MSG_COMMS_TRY_PARSE,
	MSG_COMMS_FLUSH,
	MSG_INVALIDATE,
	MSG_ACK
};

#endif /* MESSAGES_H_ */
