#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include "main.h"

#define STEPPER_MOTOR_STOP		0
#define STEPPER_MOTOR_START		1
#define MOTOR_DIRECTION_CCW		0
#define MOTOR_DIRECTION_CW		1
#define MIN_MOTOR_SPEED_RPM		1
#define MAX_MOTOR_SPEED_RPM		50

typedef struct {
	uint8_t chopper_off_time;
	uint8_t microstep_res;
	uint8_t interpolation;
	uint8_t chopper_mode;
	uint8_t current_setting;
	uint8_t chopper_hysteresis;
	uint8_t chopper_blank_time;
	uint8_t driver_enable;
	uint8_t standstill_pwr_down;
} Standalone_Config;

typedef struct {
	uint8_t run_state;
	uint8_t direction;
	uint8_t current_scale;// 0.5V - 2.5V
	uint8_t speed;// 1 - 50 rpm
	Standalone_Config standalone_cfg;
} StepperMtr_HandleTypeDef;

extern StepperMtr_HandleTypeDef hstepmtr;

void Init_Stepper_Motor(void);
void Update_Stepper_Motor_Operation(void);

#define LOWER_DAC_OUTPUT	0.5f// 1V (I got the actual 0.98V measured with voltmeter)
#define UPPER_DAC_OUTPUT	2.5f// 2.5V (I got the actual 2.46V measured with voltmeter)
#define MAX_CURRENT_SCALE	100// 100%
#define MIN_CURRENT_SCALE	0// 0%
void Update_Motor_Current_Scale(void);

/*
DACout = Vref x DOR / 4096
DOR = DACout/Vref * 4096
0.5V --> DOR = 0.5V/3.3V * 4096 = 620
2.5V --> DOR = 2.5V/3.3V * 4096 = 3103
current scale:
0% --> 620
100% --> 3103
slope = (3103 - 620)/(100 - 0) = 24.83
offset = 620
*/

void Update_Motor_Speed(void);
void Update_Standalone_Configuration(void);

/* Standalone Operation */
// CFG0: Chopper off time (duration of slow decay phase)
#define	TOFF_140TCLK	0
#define	TOFF_236TCLK	1
#define	TOFF_332TCLK	2
// CFG1 and CFG2: Microstep resolution for STEP input
#define	MODE_SPREADCYCLE	0x00
#define	MODE_STEALTHCHOP	0x40
#define	NO_INTERPOLATION	0x00
#define	USE_INTERPOLATION	0x20
#define	FULLSTEP_1			0x01
#define	HALFSTEP_2			0x02
#define	QUARTERSTEP_4		0x04
#define	MICROSTEP_16		0x10
// CFG3: Current setting
#define	EXT_SENSE_RES				0
#define	INT_SENSE_RES				1
#define	EXT_SENSE_RES_SCALED_AIN	2
// CFG4: Chopper Hysteresis
#define	HYSTERESIS_END_5	0
#define	HYSTERESIS_END_9	1
#define	HYSTERESIS_END_13	2
// CFG5: Chopper Blank Time
#define	BLANK_TIME_16		0
#define	BLANK_TIME_24		1
#define	BLANK_TIME_36		2
// CFG6: Enable pin, standstill power down
#define	DRIVER_ENABLE		0
#define	DRIVER_DISABLE		1
#define	STANDSTILL_OFF		0
#define	STANDSTILL_ON		2

#endif
