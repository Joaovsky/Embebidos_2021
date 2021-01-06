/*
 * Sys.h
 *
 *  Created on: 21/11/2020
 *      Author: Tiago
 */

#ifndef SRC_SYS_H_
#define SRC_SYS_H_

#include <stdint.h>



typedef struct main_vars{
	uint8_t _fl_changed;
	uint8_t _fl_sys_changed;
	uint8_t _fl_motor_control;
	uint8_t _fl_motor_blocked;
	uint8_t _fl_fan_control;
	uint8_t _fl_lux_mode;
	uint8_t _fl_states_rl;
	int16_t _actual_temp;
	int8_t _setpoint_temp;
	uint16_t _actual_lux;
	uint8_t _setpoint_lux;
	uint8_t _actual_humm;


}vars_t;


extern vars_t _mains;

void main_vars_inti(void);



/*
//float erro;
//int accao;
//float reward;


void func_accao(void);
void func_aval(void);
void func_trab(void);
*/

#endif /* SRC_SYS_H_ */
