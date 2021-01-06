/*
 * Sys2.c
 *
 *  Created on: Nov 21, 2020
 *      Author: Tiago
 */

#include "Sys2.h"
float erro=-50;
float erro_ant=0;
int accao=0;
int accao_ant=0;
float reward=0;

void func_accao_2(void){
	if(erro!=0){
		accao=1;
	}
	if(erro_ant!=0){
		if(reward>0){
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
void func_aval_2(void){
	if(erro!=0){
		if((erro>0)&&(accao>0)){
			if ((erro_ant-erro)>0){
				reward=1;
			}
		}
		else if((erro<0)&&(accao<0)){
			if ((erro_ant-erro)<0){
				reward=1;
			}
		}
		else {
			reward=-1;
		}
	}
	else{
		reward=0;
	}
	erro_ant=erro;
}

void func_trab_2(void){
	if(erro>0){
		if(accao>0){
			erro-=1;
		}
		else{
			erro+=1;
		}
	}
	else if(erro<0){
			if(accao<0){
				erro+=1;
			}
			else{
				erro-=1;
			}
		}

}

