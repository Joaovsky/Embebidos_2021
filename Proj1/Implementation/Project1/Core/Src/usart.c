/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include "cmsis_os.h"
#include "Sys.h"

extern osThreadId vTaskProcessInfHandle;

extern osMessageQId Queue_Reference_TempHandle;
extern osMessageQId Queue_Refernce_LightHandle;
extern osMessageQId Queue_Light_ModeHandle;
extern osMessageQId Queue_Real_TempHandle;
extern osMessageQId Queue_Real_HumHandle;
extern osMessageQId Queue_Real_LuminHandle;

uint8_t Rx_Uart2[16];
uint8_t Rx_Uart3[16];
uint8_t Rx_Uart2_index=0;
uint8_t Rx_Uart3_index=0;

uint8_t uart2_received_flag=0;
uint8_t uart3_received_flag=0;

uint8_t Uart3_Buffer[16];
uint8_t uart3_atual_index=0;
uint8_t uart3_ant_index=0;


/* USER CODE END 0 */

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, STLK_RX_Pin|STLK_TX_Pin);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void init_UART3(void){
	HAL_UART_Receive_IT(&huart3, &Rx_Uart3[0], 1);
}

void init_UART2(void){
	HAL_UART_Receive_IT(&huart2, &Rx_Uart2[0], 1);
}

/**************************************************************************/
/*!
   @brief  Reception Complete callback for UART 2 e UART 3
   	   	   Uart 2 Receives bluetooth messages
   	   	   Uart 3 Receives via Usb
*/
/**************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
	if(huart->Instance == USART2){ //foi a uart2 que gerou a interrupçao?
		HAL_UART_Receive_IT(&huart2,&Rx_Uart2[ (++Rx_Uart2_index & 15) ],1);
		if(Rx_Uart2[(Rx_Uart2_index & 15)-1]=='\n'){// if received endline character
			uart2_received_flag=1;
			vTaskNotifyGiveFromISR( vTaskProcessInfHandle, NULL );
		}
	}
	if(huart->Instance == USART3) { //foi a uart3 que gerou a interrupçao?
		HAL_UART_Receive_IT(&huart3,&Rx_Uart3[ (++Rx_Uart3_index & 15) ],1);
		if(Rx_Uart3[(Rx_Uart3_index & 15)-1]=='\n'){ // if received endline character
			uart3_received_flag=1;
			vTaskNotifyGiveFromISR( vTaskProcessInfHandle, NULL );
		}
	}
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNCU__ */

PUTCHAR_PROTOTYPE
{
	char n_l= 13;
	HAL_UART_Transmit(&huart3, (uint8_t*)&ch,1,0xFFFF);
	if(ch==10){
	HAL_UART_Transmit(&huart3, (uint8_t*)&n_l,1,100);
	}

	return ch;
}

bool send_uart3(uint8_t *str){
	// transmit via uart 3
	uint8_t count;
	for(count=1;str[count]!='\n';count++){}
	HAL_UART_Transmit(&huart3, str, count,50);
	return true;
}

bool receive_uart3(void){

	uint8_t tam=0;
	uint8_t val=0;
//	int8_t aux temp=0;
	uart3_atual_index=(Rx_Uart3_index&15);
		if(uart3_atual_index!=uart3_ant_index){

			for(int x=0;x<=(uart3_atual_index-uart3_ant_index);x++){
				Uart3_Buffer[x]=Rx_Uart3[x+uart3_ant_index];
				Uart3_Buffer[x+1]=0;
				tam=x;
			}
			switch(Uart3_Buffer[0]){
//			case 'T': // Temperature
//				aux_var=(((Uart3_Buffer[1]-'0')*10)+(Uart3_Buffer[2]-'0'));
////				osMessagePut (Queue_Reference_TempHandle, aux_var, 10);
//				break;
//			case 'L': // light mode
//				if(Uart3_Buffer[1]=='M'){
//					aux_var=(Uart3_Buffer[2]-'0');
////					osMessagePut (Queue_Light_ModeHandle, aux_var, 10);
//				}
//				else{
//					if(tam==4){aux_var = 100;}
//					if(tam==3){aux_var = (((Uart3_Buffer[1]-'0')*10)+(Uart3_Buffer[2]-'0'));}
//					if(tam==2){aux_var = ((Uart3_Buffer[1]-'0'));}
////					osMessagePut (Queue_Refernce_LightHandle, aux_var, 10);
//				}
////				printf("%c %c %c %c %d %d\n",bth_rx_buffer[0],bth_rx_buffer[1],bth_rx_buffer[2],bth_rx_buffer[3],tam);
//				break;
			case 'T': // Temperature
				val=(((Uart3_Buffer[1]-'0')*10)+(Uart3_Buffer[2]-'0'));
//					xQueueOverwrite(Queue_Reference_TempHandle, &val);
				_mains._setpoint_temp=val;
				_mains._fl_changed=11;

				break;
			case 'L': // light mode
				if(Uart3_Buffer[1]=='M'){
					val=(Uart3_Buffer[2]-'0');
//						xQueueOverwrite(Queue_Light_ModeHandle, ptr);
//						printf("%d\n",*ptr);
					_mains._fl_lux_mode=val;
					_mains._fl_changed=12;
				}
				else{
					if(tam==6){val = 100;}
					if(tam==5){val = (((Uart3_Buffer[1]-'0')*10)+(Uart3_Buffer[2]-'0'));
					}
					if(tam==4){val = ((Uart3_Buffer[1]-'0'));
					}
//						xQueueOverwrite(Queue_Refernce_LightHandle, &val);
					_mains._setpoint_lux=val;
					_mains._fl_changed=13;
				}
				break;
			default:
				break;
			}
		}
	uart3_ant_index=uart3_atual_index;
	return true;
}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
