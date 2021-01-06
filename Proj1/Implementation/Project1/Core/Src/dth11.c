/*
 * dth11.cpp
 *
 *  Created on: Dec 10, 2020
 *      Author: Tiago
 */

//#include "tim.h"
#include "usart.h"
#include "dth11.h"
//#include <stdbool.h>


void start_start_sign(void){
/*	// por o pino a 0
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  //Configure GPIO pin Output Level
	  HAL_GPIO_WritePin(Signal_DHT_GPIO_Port, Signal_DHT_Pin, GPIO_PIN_RESET);
	  //Configure GPIO pins : PBPin PBPin PBPin
	  GPIO_InitStruct.Pin = Signal_DHT_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(Signal_DHT_GPIO_Port, &GPIO_InitStruct);

*/
}

void stop_start_sign(void){
/*	// por o pino a 1
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_GPIO_WritePin(Signal_DHT_GPIO_Port, Signal_DHT_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Signal_DHT_GPIO_Port, Signal_DHT_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = Signal_DHT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(Signal_DHT_GPIO_Port, &GPIO_InitStruct);
		 //iniciar a captura por interrupçao
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1); // start


*/
}

void stop_sign(void){
/*	HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1);
	i=0;
*/
}

bool verifyData(char *buff){
	char x=0;
	char y=0;
	char z=0;
	char w=0;
	char t=0;
	for(int i=0;i<8;i++){
		if(buff[i]>100){
			x = (x<<1)+1;
		}
		else{
			x = (x<<1);
		}
		if(buff[i+8]>100){
			y = (y<<1)+1;
		}
		else{
			y = (y<<1);
		}
		if(buff[i+16]>100){
			z = (z<<1)+1;
		}
		else{
			z = (z<<1);
		}
		if(buff[i+24]>100){
			w = (w<<1)+1;
		}
		else{
			w = (w<<1);
		}
		if(buff[i+32]>100){
			t = (t<<1)+1;
		}
		else{
			t = (t<<1);
		}
	}
	if((x+y+z+w)==t){
		return true;
	}
	return false;
}

uint8_t getHumm(char *buff){
	char x=0;
	for(int i=0;i<8;i++){
		if(buff[i]>100){
			x = (x<<1)+1;
		}
		else{
			x = (x<<1);
		}
	}

return x;
}

float getTemp(char *buff){
	char x=0;
	char z=0;
	float y=0.0;
	for(int i=0;i<8;i++){
		if(buff[i+16]>100){
			x = (x<<1)+1;
		}
		else{
			x = (x<<1);
		}
		if(buff[i+24]>100){
			z = (z<<1)+1;
		}
		else{
			z = (z<<1);
		}
	}
	y=x+(z/10);
return y;
}

