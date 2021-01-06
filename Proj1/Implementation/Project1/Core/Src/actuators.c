/*
 * fan_heating.c
 *
 *  Created on: Dec 14, 2020
 *      Author: Tiago
 */
#include "actuators.h"
#include "gpio.h"
#include "tim.h"
#include "stm32f7xx_hal.h"


/** GPIO Configuration
    PE13     ------> TIM1_CH3	//fan
    PE14     ------> TIM1_CH4	//heating
    PB1		 ------> TIM3_CH4	//ligt
*/

bool actuate_light(uint8_t pwm){
	if((pwm>=0)&&(pwm<=100)){
//		htim3.Instance->CCR4=pwm*10;
		return true;
	}
return false;

}

bool actuate_fan(uint8_t pwm){
	if((pwm>=0)&&(pwm<=100)){
//		htim1.Instance->CCR3=pwm*10;
		return true;
	}
return false;

}

bool actuate_heating(uint8_t pwm){
	if((pwm>=0)&&(pwm<=100)){
//		htim1.Instance->CCR4=pwm*10;
		return true;
	}
return false;
}

uint8_t getPWM_fan(void){
//	return (htim1.Instance->CCR3/10);
return 0;
}

uint8_t getPWM_heating(void){
//	return (htim1.Instance->CCR4/10);
	return 0;
}


