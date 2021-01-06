/*
 * Sys.c
 *
 *  Created on: 21/11/2020
 *      Author: Tiago
 */


#include "Sys.h"


vars_t _mains;

void main_vars_inti(void){
	_mains._fl_changed=0;
	_mains._fl_sys_changed=0;
	_mains._fl_motor_control=1;
	_mains._fl_motor_blocked=0;
	_mains._fl_fan_control=1;
	_mains._fl_lux_mode=0;
	_mains._fl_states_rl=0;
	_mains._actual_temp=0;
	_mains._setpoint_temp=0;
	_mains._actual_lux=0;
	_mains._setpoint_lux=0;
	_mains._actual_humm=0;
}


/*
float erro=-50;
float erro_ant=0;
int accao=0;
int accao_ant=0;
float reward=0;

void func_accao(void){
	if(erro!=0){
		accao=1;
	}
	if(erro_ant!=0){
		if((reward>0)&&((erro*erro_ant)>0)){
			accao=accao_ant;
		}
		else{
			accao=-accao_ant;
		}
	}
	if (erro==0){
		accao=0;
	}
	accao_ant=accao;
}
void func_aval(void){
	if(erro!=0){
		if((erro>0)&&(accao>0)){
			reward=1;
		}
		else if((erro<0)&&(accao<0)){
			reward=1;
		}
		else {
			reward=-1;
		}
	}
	else{
		reward=0;
	}
}
void func_trab(void){
	if(erro>0){
		if(reward>0){
			erro-=1;
		}
		else{
			erro+=1;
		}
	}
	else if(erro<0){
			if(reward>0){
				erro+=1;
			}
			else{
				erro-=1;
			}
		}
	erro_ant=erro;
}
*/

