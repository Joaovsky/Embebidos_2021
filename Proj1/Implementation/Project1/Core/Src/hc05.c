/*
 * HC05.cpp
 *
 *  Created on: Dec 10, 2020
 *      Author: Tiago
 */

/* Includes ------------------------------------------------------------------*/
#include "hc05.h"
#include "usart.h"
#include "gpio.h"
#include "stm32f7xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "cmsis_os.h"
#include "dc_motor.h"
#include "Sys.h"

uint8_t atu_index;
uint8_t ant_index;
uint8_t bth_rx_buffer[20];
//uint8_t aux_var=0;

extern osMessageQId Queue_Reference_TempHandle;
extern osMessageQId Queue_Refernce_LightHandle;
extern osMessageQId Queue_Light_ModeHandle;
extern osMessageQId Queue_Real_TempHandle;
extern osMessageQId Queue_Real_HumHandle;
extern osMessageQId Queue_Real_LuminHandle;

extern uint8_t flag_motor_control;


bool verifyConnection(void){
	if(HAL_GPIO_ReadPin(BTH_ST_GPIO_Port,BTH_ST_Pin)==1){
		return true;
	}
	else {
		return false;
	}
}

bool send_bth(uint8_t *str){
	// transmit via uart 2
	uint8_t count;
	for(count=1;str[count]!='\n';count++){}
	HAL_UART_Transmit(&huart2, str, count,50);
	return true;
}

bool receive_bth(void){
	uint8_t tam=0;
	static uint8_t val=0;
	atu_index=(Rx_Uart2_index&15);
		if(atu_index!=ant_index){
			for(int x=0;x<=(atu_index-ant_index);x++){
				bth_rx_buffer[x]=Rx_Uart2[x+ant_index];
				bth_rx_buffer[x+1]=0;
				tam=x;
			}
			switch(bth_rx_buffer[0]){
				case 'T': // Temperature
					val=(((bth_rx_buffer[1]-'0')*10)+(bth_rx_buffer[2]-'0'));
//					xQueueOverwrite(Queue_Reference_TempHandle, &val);
					_mains._setpoint_temp=val;
					_mains._fl_changed=1;
					break;
				case 'L': // light mode
					if(bth_rx_buffer[1]=='M'){
						val=(bth_rx_buffer[2]-'0');
//						xQueueOverwrite(Queue_Light_ModeHandle, ptr);
//						printf("%d\n",*ptr);
						_mains._fl_lux_mode=val;
						_mains._fl_changed=2;
					}
					else{
						if(tam==6){val = 100;}
						if(tam==5){val = (((bth_rx_buffer[1]-'0')*10)+(bth_rx_buffer[2]-'0'));
						}
						if(tam==4){val = ((bth_rx_buffer[1]-'0'));
						}
//						xQueueOverwrite(Queue_Refernce_LightHandle, &val);
						_mains._setpoint_lux=val;
						_mains._fl_changed=3;
					}
					break;
				case 'M': // start/stop motor
					if((bth_rx_buffer[1]-'0')==1){
						_mains._fl_motor_control=1;
						flag_motor_control=1;
						start_motor();
					}
					else{
						stop_motor();
						_mains._fl_motor_control=0;
						flag_motor_control=0;
					}
					break;
				default:
					break;
			}
		}
	ant_index=atu_index;
	return true;
}

bool actual_data(void){
return true;
}
