/*
 * dc_motor.c
 *
 *  Created on: Dec 14, 2020
 *      Author: Tiago
 */

#include "dc_motor.h"
#include "gpio.h"
#include "tim.h"
#include "stm32f7xx_hal.h"
#include "Sys.h"

extern uint8_t flag_blocked;

/** GPIO Configuration
	PE9     ------> TIM1_CH1	//motor r A2
    PE11     ------> TIM1_CH2	//motor l B2
    PF13     ------> OUT_A1	//motor r A1
    PF14     ------> OUT_B1	//motor l B1
*/


bool start_motor(void){
//	if(!_mains._fl_motor_blocked){
//		htim1.Instance->CCR1=500;
//		htim1.Instance->CCR2=500;
//		return true;
//	}
	return false;
}

bool stop_motor(void){
//	htim1.Instance->CCR1=0;
//	htim1.Instance->CCR2=0;
	return true;
}

bool verify_motor(void){
//	if((htim1.Instance->CCR1>0)&&(htim1.Instance->CCR2>0)){
//		return true;
//	}
	return false;
}
