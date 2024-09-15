#include "stepper_motor.h"

StepperMtr_HandleTypeDef hstepmtr = {.run_state = STEPPER_MOTOR_STOP,
									 .direction = MOTOR_DIRECTION_CCW,
									 .current_scale = 25,// 25% (about 1V)
									 .speed = MIN_MOTOR_SPEED_RPM,
									 .standalone_cfg = {.chopper_off_time = TOFF_140TCLK,
														.microstep_res = FULLSTEP_1,
														.interpolation = NO_INTERPOLATION,
														.chopper_mode = MODE_SPREADCYCLE,
														.current_setting = EXT_SENSE_RES_SCALED_AIN,
														.chopper_hysteresis = HYSTERESIS_END_5,
														.chopper_blank_time = BLANK_TIME_24,
														.driver_enable = DRIVER_ENABLE,
														.standstill_pwr_down = STANDSTILL_ON}
									 };

void Init_Stepper_Motor(void)
{
	Update_Stepper_Motor_Operation();
	LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);	// LED off
	
	Update_Standalone_Configuration();
}

void Update_Stepper_Motor_Operation(void)
{
	Update_Motor_Speed();
	Update_Motor_Current_Scale();
	
	switch(hstepmtr.direction){
	case MOTOR_DIRECTION_CCW:
		if(LL_GPIO_IsOutputPinSet(DIR_GPIO_Port, DIR_Pin) == 1){
			LL_GPIO_ResetOutputPin(DIR_GPIO_Port, DIR_Pin);
		}// else pin already in reset state
		break;
	case MOTOR_DIRECTION_CW:
		if(LL_GPIO_IsOutputPinSet(DIR_GPIO_Port, DIR_Pin) == 0){
			LL_GPIO_SetOutputPin(DIR_GPIO_Port, DIR_Pin);
		}// else pin already in set state
		break;
	default:
		break;
	}
	
	switch(hstepmtr.run_state){
	case STEPPER_MOTOR_STOP:
		if(HAL_TIM_GetChannelState(&htim2, TIM_CHANNEL_3) == HAL_TIM_CHANNEL_STATE_BUSY){
			HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);		// stop the PWM on motor STEP pin
			LL_GPIO_SetPinMode(STEP_GPIO_Port, STEP_Pin, LL_GPIO_MODE_OUTPUT);
			LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);	// LED off
		}// else if not STATE_BUSY that means the PWM already stopped assumed
		break;
	case STEPPER_MOTOR_START:
		if(HAL_TIM_GetChannelState(&htim2, TIM_CHANNEL_3) == HAL_TIM_CHANNEL_STATE_READY){
			LL_GPIO_SetPinMode(STEP_GPIO_Port, STEP_Pin, LL_GPIO_MODE_ALTERNATE);
			HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);		// start generating PWM on motor STEP pin
			LL_GPIO_SetOutputPin(LD2_GPIO_Port, LD2_Pin);	// LED on
		}// else if not STATE_READY that means the PWM already started assumed
		break;
	default:
		break;
	}
}

void Update_Motor_Current_Scale(void)
{
	// DACout = Vref x DOR / 4096
	// DOR = DACout/Vref * 4096
	float upper_dac_val = UPPER_DAC_OUTPUT / 3.3f * 4096.0f;
	float lower_dac_val = LOWER_DAC_OUTPUT / 3.3f * 4096.0f;
	float slope = (upper_dac_val - lower_dac_val) / ((float)MAX_CURRENT_SCALE - (float)MIN_CURRENT_SCALE);
	float offset = lower_dac_val - (slope * (float)MIN_CURRENT_SCALE);
	
	int8_t new_current_scale = (int8_t)hstepmtr.current_scale;
	if(new_current_scale > MAX_CURRENT_SCALE){	new_current_scale = MAX_CURRENT_SCALE;	}// filter max percentage
	if(new_current_scale < MIN_CURRENT_SCALE){	new_current_scale = MIN_CURRENT_SCALE;	}// filter min percentage
	
	uint16_t new_dac_val = (uint16_t)((float)new_current_scale * slope + offset);
	if(HAL_DAC_GetValue(&hdac1, DAC_CHANNEL_1) == (uint32_t)new_dac_val){
		return;	// not necessary to update
	}
	// proceed with updating DAC
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, new_dac_val);
}

void Update_Motor_Speed(void)
{
	float step_freq;
	// step freq = (rpm) rev/min * 1 min/60sec * 360 deg/rev * 1 pulse/1.8 deg * (microstep resolution)
	step_freq = (float)hstepmtr.speed / 60.0f * 360.0f / 1.8f * (float)hstepmtr.standalone_cfg.microstep_res;
	uint32_t tim_period;
	// SystemCoreClock = 80000000U
	tim_period = (uint32_t)((float)SystemCoreClock / (float)(htim2.Init.Prescaler + 1) / step_freq);
	
	if(htim2.Init.Period == (tim_period - 1)){
		return;	// not necessary to update period & duty (pulse)
	}
	// proceed with updating PWM parameters below
	if(HAL_TIM_GetChannelState(&htim2, TIM_CHANNEL_3) == HAL_TIM_CHANNEL_STATE_BUSY){// check if PWM is currently active
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);		// stop the PWM because we want to update its parameters
		LL_GPIO_SetPinMode(STEP_GPIO_Port, STEP_Pin, LL_GPIO_MODE_OUTPUT);
		LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);	// LED off, if there'll be a glitch on the LED, I don't mind since less risk
	}// else if not STATE_BUSY that means the PWM already stopped assumed
	
	htim2.Init.Period = tim_period - 1;	// only period need to be changed
	HAL_TIM_PWM_Init(&htim2);
	
	TIM_OC_InitTypeDef sConfigOC = {0};
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = (tim_period / 2) - 1;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3);
	
	// if hstepmtr.run_state == STEPPER_MOTOR_START, then PWM will be started again during run_state check after this function execution done
}

void Update_Standalone_Configuration(void)
{// Refer to TMC2130 datasheet Ch 24 Standalone Operation (table for CFG0-CFG6)
	/* CFG0 */
	if(hstepmtr.standalone_cfg.chopper_off_time == TOFF_332TCLK){
		if(LL_GPIO_GetPinOutputType(CFG0_GPIO_Port, CFG0_Pin) == LL_GPIO_OUTPUT_PUSHPULL){
			LL_GPIO_SetPinOutputType(CFG0_GPIO_Port, CFG0_Pin, LL_GPIO_OUTPUT_OPENDRAIN);	// set open drain type
		}
	} else {
		if(LL_GPIO_GetPinOutputType(CFG0_GPIO_Port, CFG0_Pin) == LL_GPIO_OUTPUT_OPENDRAIN){
			LL_GPIO_SetPinOutputType(CFG0_GPIO_Port, CFG0_Pin, LL_GPIO_OUTPUT_PUSHPULL);	// set to output push-pull
		}
	}
	if(hstepmtr.standalone_cfg.chopper_off_time == TOFF_140TCLK){
		if(LL_GPIO_IsOutputPinSet(CFG0_GPIO_Port, CFG0_Pin) == 1){
			LL_GPIO_ResetOutputPin(CFG0_GPIO_Port, CFG0_Pin);	// set to output level low
		}
	} else {	// TOFF_236TCLK and also for TOFF_332TCLK (open)
		if(LL_GPIO_IsOutputPinSet(CFG0_GPIO_Port, CFG0_Pin) == 0){
			LL_GPIO_SetOutputPin(CFG0_GPIO_Port, CFG0_Pin);	// set to output level high
		}
	}
	/* CFG1 */
	if((hstepmtr.standalone_cfg.interpolation == USE_INTERPOLATION) && 
	   ((hstepmtr.standalone_cfg.microstep_res == HALFSTEP_2) || 
	    ((hstepmtr.standalone_cfg.microstep_res == QUARTERSTEP_4) && (hstepmtr.standalone_cfg.chopper_mode == MODE_SPREADCYCLE)) || 
		((hstepmtr.standalone_cfg.microstep_res == MICROSTEP_16) && (hstepmtr.standalone_cfg.chopper_mode == MODE_STEALTHCHOP))
	    ))
	{
		if(LL_GPIO_GetPinOutputType(CFG1_GPIO_Port, CFG1_Pin) == LL_GPIO_OUTPUT_PUSHPULL){
			LL_GPIO_SetPinOutputType(CFG1_GPIO_Port, CFG1_Pin, LL_GPIO_OUTPUT_OPENDRAIN);	// set open drain type
		}
	} else {
		if(LL_GPIO_GetPinOutputType(CFG1_GPIO_Port, CFG1_Pin) == LL_GPIO_OUTPUT_OPENDRAIN){
			LL_GPIO_SetPinOutputType(CFG1_GPIO_Port, CFG1_Pin, LL_GPIO_OUTPUT_PUSHPULL);	// set to output push-pull
		}
	}
	if((hstepmtr.standalone_cfg.microstep_res == FULLSTEP_1) || 
	   ((hstepmtr.standalone_cfg.microstep_res == QUARTERSTEP_4) && (hstepmtr.standalone_cfg.interpolation == NO_INTERPOLATION)) || 
	   ((hstepmtr.standalone_cfg.microstep_res == MICROSTEP_16) && (hstepmtr.standalone_cfg.interpolation == USE_INTERPOLATION) && 
	    (hstepmtr.standalone_cfg.chopper_mode == MODE_SPREADCYCLE))
	   )
	{
		if(LL_GPIO_IsOutputPinSet(CFG1_GPIO_Port, CFG1_Pin) == 1){
			LL_GPIO_ResetOutputPin(CFG1_GPIO_Port, CFG1_Pin);	// set to output level low
		}
	} else {
		if(LL_GPIO_IsOutputPinSet(CFG1_GPIO_Port, CFG1_Pin) == 0){
			LL_GPIO_SetOutputPin(CFG1_GPIO_Port, CFG1_Pin);	// set to output level high
		}
	}
	/* CFG2 */
	if((hstepmtr.standalone_cfg.chopper_mode == MODE_STEALTHCHOP) || 
	   ((hstepmtr.standalone_cfg.interpolation == USE_INTERPOLATION) && (hstepmtr.standalone_cfg.microstep_res == MICROSTEP_16)))
	{
		if(LL_GPIO_GetPinOutputType(CFG2_GPIO_Port, CFG2_Pin) == LL_GPIO_OUTPUT_PUSHPULL){
			LL_GPIO_SetPinOutputType(CFG2_GPIO_Port, CFG2_Pin, LL_GPIO_OUTPUT_OPENDRAIN);	// set open drain type
		}
	} else {
		if(LL_GPIO_GetPinOutputType(CFG2_GPIO_Port, CFG2_Pin) == LL_GPIO_OUTPUT_OPENDRAIN){
			LL_GPIO_SetPinOutputType(CFG2_GPIO_Port, CFG2_Pin, LL_GPIO_OUTPUT_PUSHPULL);	// set to output push-pull
		}
	}
	if(hstepmtr.standalone_cfg.microstep_res <= HALFSTEP_2){	// fullstep(1) or halfstep(2)
		if(LL_GPIO_IsOutputPinSet(CFG2_GPIO_Port, CFG2_Pin) == 1){
			LL_GPIO_ResetOutputPin(CFG2_GPIO_Port, CFG2_Pin);	// set to output level low
		}
	} else {
		if(LL_GPIO_IsOutputPinSet(CFG2_GPIO_Port, CFG2_Pin) == 0){
			LL_GPIO_SetOutputPin(CFG2_GPIO_Port, CFG2_Pin);		// set to output level high
		}
	}
	/* CFG3 */
	if(hstepmtr.standalone_cfg.current_setting == EXT_SENSE_RES_SCALED_AIN){
		if(LL_GPIO_GetPinOutputType(CFG3_GPIO_Port, CFG3_Pin) == LL_GPIO_OUTPUT_PUSHPULL){
			LL_GPIO_SetPinOutputType(CFG3_GPIO_Port, CFG3_Pin, LL_GPIO_OUTPUT_OPENDRAIN);	// set open drain type
		}
	} else {
		if(LL_GPIO_GetPinOutputType(CFG3_GPIO_Port, CFG3_Pin) == LL_GPIO_OUTPUT_OPENDRAIN){
			LL_GPIO_SetPinOutputType(CFG3_GPIO_Port, CFG3_Pin, LL_GPIO_OUTPUT_PUSHPULL);	// set to output push-pull
		}
	}
	if(hstepmtr.standalone_cfg.current_setting == EXT_SENSE_RES){
		if(LL_GPIO_IsOutputPinSet(CFG3_GPIO_Port, CFG3_Pin) == 1){
			LL_GPIO_ResetOutputPin(CFG3_GPIO_Port, CFG3_Pin);	// set to output level low
		}
	} else {	// INT_SENSE_RES and also for EXT_SENSE_RES_SCALED_AIN (open)
		if(LL_GPIO_IsOutputPinSet(CFG3_GPIO_Port, CFG3_Pin) == 0){
			LL_GPIO_SetOutputPin(CFG3_GPIO_Port, CFG3_Pin);	// set to output level high
		}
	}
	/* CFG4 */
	if(hstepmtr.standalone_cfg.chopper_hysteresis == HYSTERESIS_END_13){
		if(LL_GPIO_GetPinOutputType(CFG4_GPIO_Port, CFG4_Pin) == LL_GPIO_OUTPUT_PUSHPULL){
			LL_GPIO_SetPinOutputType(CFG4_GPIO_Port, CFG4_Pin, LL_GPIO_OUTPUT_OPENDRAIN);	// set open drain type
		}
	} else {
		if(LL_GPIO_GetPinOutputType(CFG4_GPIO_Port, CFG4_Pin) == LL_GPIO_OUTPUT_OPENDRAIN){
			LL_GPIO_SetPinOutputType(CFG4_GPIO_Port, CFG4_Pin, LL_GPIO_OUTPUT_PUSHPULL);	// set to output push-pull
		}
	}
	if(hstepmtr.standalone_cfg.chopper_hysteresis == HYSTERESIS_END_5){
		if(LL_GPIO_IsOutputPinSet(CFG4_GPIO_Port, CFG4_Pin) == 1){
			LL_GPIO_ResetOutputPin(CFG4_GPIO_Port, CFG4_Pin);	// set to output level low
		}
	} else {	// HYSTERESIS_END_9 and also for HYSTERESIS_END_13 (open)
		if(LL_GPIO_IsOutputPinSet(CFG4_GPIO_Port, CFG4_Pin) == 0){
			LL_GPIO_SetOutputPin(CFG4_GPIO_Port, CFG4_Pin);	// set to output level high
		}
	}
	/* CFG5 */
	if(hstepmtr.standalone_cfg.chopper_blank_time == BLANK_TIME_36){
		if(LL_GPIO_GetPinOutputType(CFG5_GPIO_Port, CFG5_Pin) == LL_GPIO_OUTPUT_PUSHPULL){
			LL_GPIO_SetPinOutputType(CFG5_GPIO_Port, CFG5_Pin, LL_GPIO_OUTPUT_OPENDRAIN);	// set open drain type
		}
	} else {
		if(LL_GPIO_GetPinOutputType(CFG5_GPIO_Port, CFG5_Pin) == LL_GPIO_OUTPUT_OPENDRAIN){
			LL_GPIO_SetPinOutputType(CFG5_GPIO_Port, CFG5_Pin, LL_GPIO_OUTPUT_PUSHPULL);	// set to output push-pull
		}
	}
	if(hstepmtr.standalone_cfg.chopper_blank_time == BLANK_TIME_16){
		if(LL_GPIO_IsOutputPinSet(CFG5_GPIO_Port, CFG5_Pin) == 1){
			LL_GPIO_ResetOutputPin(CFG5_GPIO_Port, CFG5_Pin);	// set to output level low
		}
	} else {	// BLANK_TIME_24 and also for BLANK_TIME_36 (open)
		if(LL_GPIO_IsOutputPinSet(CFG5_GPIO_Port, CFG5_Pin) == 0){
			LL_GPIO_SetOutputPin(CFG5_GPIO_Port, CFG5_Pin);	// set to output level high
		}
	}
	/* CFG6 */
	if((hstepmtr.standalone_cfg.driver_enable == DRIVER_ENABLE) && (hstepmtr.standalone_cfg.standstill_pwr_down == STANDSTILL_ON)){
		if(LL_GPIO_GetPinOutputType(CFG6_EN_GPIO_Port, CFG6_EN_Pin) == LL_GPIO_OUTPUT_PUSHPULL){
			LL_GPIO_SetPinOutputType(CFG6_EN_GPIO_Port, CFG6_EN_Pin, LL_GPIO_OUTPUT_OPENDRAIN);	// set open drain type
		}
	} else {
		if(LL_GPIO_GetPinOutputType(CFG6_EN_GPIO_Port, CFG6_EN_Pin) == LL_GPIO_OUTPUT_OPENDRAIN){
			LL_GPIO_SetPinOutputType(CFG6_EN_GPIO_Port, CFG6_EN_Pin, LL_GPIO_OUTPUT_PUSHPULL);	// set to output push-pull
		}
	}
	if((hstepmtr.standalone_cfg.driver_enable == DRIVER_ENABLE) && (hstepmtr.standalone_cfg.standstill_pwr_down == STANDSTILL_OFF)){
		if(LL_GPIO_IsOutputPinSet(CFG6_EN_GPIO_Port, CFG6_EN_Pin) == 1){
			LL_GPIO_ResetOutputPin(CFG6_EN_GPIO_Port, CFG6_EN_Pin);	// set to output level low
		}
	} else {	// DRIVER_DISABLE and also for STANDSTILL_ON (open)
		if(LL_GPIO_IsOutputPinSet(CFG6_EN_GPIO_Port, CFG6_EN_Pin) == 0){
			LL_GPIO_SetOutputPin(CFG6_EN_GPIO_Port, CFG6_EN_Pin);	// set to output level high
		}
	}
}
