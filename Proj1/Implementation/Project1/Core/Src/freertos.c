/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "actuators.h"
#include "dth11.h"
#include "bme280.h"
#include "tsl2561.h"
#include "hc05.h"
#include "dc_motor.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"
#include "Sys.h"
#include <stdio.h>
#include <string.h>





/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint8_t flag_blocked=0;
uint8_t flag_motor_control=0;
uint8_t flag_fan_control=1;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId vTaskRunningHandle;
osThreadId vTaskSendInfoHandle;
osThreadId vTaskProcessInfHandle;
osThreadId vTaskAquireTempHandle;
osThreadId vTaskAquireBrigHandle;
osThreadId vTaskSetSecureMHandle;
osThreadId vTaskRlControlHandle;
osThreadId vTaskLightActuaHandle;
osMessageQId Queue_Reference_TempHandle;
osMessageQId Queue_Refernce_LightHandle;
osMessageQId Queue_Light_ModeHandle;
osMessageQId Queue_Real_TempHandle;
osMessageQId Queue_Real_HumHandle;
osMessageQId Queue_Real_LuminHandle;
osTimerId Fast_TimerHandle;
osTimerId Slow_TimerHandle;
osTimerId Samplig_TimerHandle;
osMutexId Mutex_SendHandle;
osMutexId Mutex_Receive_Uart2Handle;
osMutexId Mutex_Receive_Uart3Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartTaskRunning(void const * argument);
void StartTaskSendInfo(void const * argument);
void StartTaskProcessInfo(void const * argument);
void StartTaskAquireTemp(void const * argument);
void StartTaskAquireBright(void const * argument);
void StartTaskSetSecureMode(void const * argument);
void StartTaskRlControl(void const * argument);
void StartTaskLightActuator(void const * argument);
void Callback_Fast_Timer(void const * argument);
void Callback_Slow_Timer(void const * argument);
void Callback_Sampling_Timer(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];
  
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )  
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}                   
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of Mutex_Send */
  osMutexDef(Mutex_Send);
  Mutex_SendHandle = osMutexCreate(osMutex(Mutex_Send));

  /* definition and creation of Mutex_Receive_Uart2 */
  osMutexDef(Mutex_Receive_Uart2);
  Mutex_Receive_Uart2Handle = osMutexCreate(osMutex(Mutex_Receive_Uart2));

  /* definition and creation of Mutex_Receive_Uart3 */
  osMutexDef(Mutex_Receive_Uart3);
  Mutex_Receive_Uart3Handle = osMutexCreate(osMutex(Mutex_Receive_Uart3));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of Fast_Timer */
  osTimerDef(Fast_Timer, Callback_Fast_Timer);
  Fast_TimerHandle = osTimerCreate(osTimer(Fast_Timer), osTimerOnce, NULL);

  /* definition and creation of Slow_Timer */
  osTimerDef(Slow_Timer, Callback_Slow_Timer);
  Slow_TimerHandle = osTimerCreate(osTimer(Slow_Timer), osTimerOnce, NULL);

  /* definition and creation of Samplig_Timer */
  osTimerDef(Samplig_Timer, Callback_Sampling_Timer);
  Samplig_TimerHandle = osTimerCreate(osTimer(Samplig_Timer), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of Queue_Reference_Temp */
  osMessageQDef(Queue_Reference_Temp, 1, uint8_t);
  Queue_Reference_TempHandle = osMessageCreate(osMessageQ(Queue_Reference_Temp), NULL);

  /* definition and creation of Queue_Refernce_Light */
  osMessageQDef(Queue_Refernce_Light, 1, uint8_t);
  Queue_Refernce_LightHandle = osMessageCreate(osMessageQ(Queue_Refernce_Light), NULL);

  /* definition and creation of Queue_Light_Mode */
  osMessageQDef(Queue_Light_Mode, 1, uint8_t);
  Queue_Light_ModeHandle = osMessageCreate(osMessageQ(Queue_Light_Mode), NULL);

  /* definition and creation of Queue_Real_Temp */
  osMessageQDef(Queue_Real_Temp, 1, float);
  Queue_Real_TempHandle = osMessageCreate(osMessageQ(Queue_Real_Temp), NULL);

  /* definition and creation of Queue_Real_Hum */
  osMessageQDef(Queue_Real_Hum, 1, uint8_t);
  Queue_Real_HumHandle = osMessageCreate(osMessageQ(Queue_Real_Hum), NULL);

  /* definition and creation of Queue_Real_Lumin */
  osMessageQDef(Queue_Real_Lumin, 1, uint16_t);
  Queue_Real_LuminHandle = osMessageCreate(osMessageQ(Queue_Real_Lumin), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of vTaskRunning */
  osThreadDef(vTaskRunning, StartTaskRunning, osPriorityIdle, 0, 128);
  vTaskRunningHandle = osThreadCreate(osThread(vTaskRunning), NULL);

  /* definition and creation of vTaskSendInfo */
  osThreadDef(vTaskSendInfo, StartTaskSendInfo, osPriorityHigh, 0, 128);
  vTaskSendInfoHandle = osThreadCreate(osThread(vTaskSendInfo), NULL);

  /* definition and creation of vTaskProcessInf */
  osThreadDef(vTaskProcessInf, StartTaskProcessInfo, osPriorityHigh, 0, 128);
  vTaskProcessInfHandle = osThreadCreate(osThread(vTaskProcessInf), NULL);

  /* definition and creation of vTaskAquireTemp */
  osThreadDef(vTaskAquireTemp, StartTaskAquireTemp, osPriorityAboveNormal, 0, 128);
  vTaskAquireTempHandle = osThreadCreate(osThread(vTaskAquireTemp), NULL);

  /* definition and creation of vTaskAquireBrig */
  osThreadDef(vTaskAquireBrig, StartTaskAquireBright, osPriorityAboveNormal, 0, 128);
  vTaskAquireBrigHandle = osThreadCreate(osThread(vTaskAquireBrig), NULL);

  /* definition and creation of vTaskSetSecureM */
  osThreadDef(vTaskSetSecureM, StartTaskSetSecureMode, osPriorityNormal, 0, 128);
  vTaskSetSecureMHandle = osThreadCreate(osThread(vTaskSetSecureM), NULL);

  /* definition and creation of vTaskRlControl */
  osThreadDef(vTaskRlControl, StartTaskRlControl, osPriorityNormal, 0, 128);
  vTaskRlControlHandle = osThreadCreate(osThread(vTaskRlControl), NULL);

  /* definition and creation of vTaskLightActua */
  osThreadDef(vTaskLightActua, StartTaskLightActuator, osPriorityNormal, 0, 128);
  vTaskLightActuaHandle = osThreadCreate(osThread(vTaskLightActua), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osTimerStart(Samplig_TimerHandle,100);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTaskRunning */
/**
  * @brief  Function implementing the vTaskRunning thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTaskRunning */
void StartTaskRunning(void const * argument)
{
  /* USER CODE BEGIN StartTaskRunning */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOB,LD1_Pin);
	  osDelay(500);
  }
  /* USER CODE END StartTaskRunning */
}

/* USER CODE BEGIN Header_StartTaskSendInfo */
/**
* @brief Function implementing the vTaskSendInfo thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskSendInfo */
void StartTaskSendInfo(void const * argument)
{
  /* USER CODE BEGIN StartTaskSendInfo */
  /* Infinite loop */
  for(;;)
  {
	  uint8_t *poiter_to_text;
	  poiter_to_text= pvPortMalloc(10);

	  switch (_mains._fl_changed){
	  case 1 : // reference temperature changed via bth
		  sprintf((char*)poiter_to_text,"RT%d\n",_mains._setpoint_temp);
		  break;
	  case 2 : // light mode changed via bth
		  sprintf((char*)poiter_to_text,"ML%d\n",_mains._fl_lux_mode);
		  break;
	  case 3 : // light reference changed via bth
		  sprintf((char*)poiter_to_text,"RL%d\n",_mains._setpoint_lux);
		  break;
	  case 11 : // reference temperature changed via uart3
		  sprintf((char*)poiter_to_text,"RT%d\r\n",_mains._setpoint_temp);
		  break;
	  case 12 : // light mode changed via uart3
		  sprintf((char*)poiter_to_text,"ML%d\r\n",_mains._fl_lux_mode);
		  break;
	  case 13 : // light reference changed via uart3
		  sprintf((char*)poiter_to_text,"RL%d\r\n",_mains._setpoint_lux);
		  break;
	  default:
		  sprintf((char*)poiter_to_text,"0\n");
		  break;
	  }
	  if((_mains._fl_changed>0)&&(_mains._fl_changed<10)){
		  _mains._fl_changed=0;
	//	  mutex take
		  send_uart3(poiter_to_text);
	//	  mutex_give
	  }
	  else if(_mains._fl_changed>10){
		  _mains._fl_changed=0;
	  	 //	  mutex take
	  	  if(verifyConnection()){
	  		  send_bth(poiter_to_text);
	  	  }
	  	//	  mutex_give
	  }
	  vPortFree(poiter_to_text);
	  osDelay(500);

	  poiter_to_text= pvPortMalloc(10);
	  switch (_mains._fl_sys_changed){
	  case 1 : // temperature changed sensor
		  sprintf((char*)poiter_to_text,"T%d,%d\r\n",(_mains._actual_temp/10),(_mains._actual_temp%10));
		  _mains._fl_sys_changed &= ~(1);
		  _mains._fl_sys_changed |= (128);
	  	  break;
	  case 2 : // humidity changed sensor
	  case 3 : // humidity changed sensor
	  	  sprintf((char*)poiter_to_text,"H%d\r\n",_mains._actual_humm);
	  	_mains._fl_sys_changed &= ~(2);
	  	_mains._fl_sys_changed |= (128);
	  	  break;
	  case 4 : // light changed sensor
	  case 5 : // light changed sensor
	  case 6 : // light changed sensor
	  case 7 : // light changed sensor
	  	  sprintf((char*)poiter_to_text,"L%d\r\n",(_mains._actual_lux/10));
	  	_mains._fl_sys_changed &= ~(4);
	  	_mains._fl_sys_changed |= (128);
	  	  break;
	  default:
		  sprintf((char*)poiter_to_text,"0\n");
		  break;
	  }
	  if(_mains._fl_sys_changed>0){
		  //	  mutex take
		  _mains._fl_sys_changed=0;
		  send_uart3(poiter_to_text);
		  if(verifyConnection()){
			  send_bth(poiter_to_text);
		  }
	  }
	  //	  mutex_give
	  vPortFree(poiter_to_text);
	  osDelay(500);
  }
  /* USER CODE END StartTaskSendInfo */
}

/* USER CODE BEGIN Header_StartTaskProcessInfo */
/**
* @brief Function implementing the vTaskProcessInf thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskProcessInfo */
void StartTaskProcessInfo(void const * argument)
{
  /* USER CODE BEGIN StartTaskProcessInfo */
  /* Infinite loop */
  for(;;)
  {
//	  uint8_t received_value=0;
	  ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
//	  printf("Process_Info_function\n");
	  if(uart2_received_flag){
		  uart2_received_flag=0;
		  receive_bth();
	  }
	  if(uart3_received_flag){
		  uart3_received_flag=0;
//		  printf("recebeu");
		  receive_uart3();
	  }
    osDelay(1);
  }
  /* USER CODE END StartTaskProcessInfo */
}

/* USER CODE BEGIN Header_StartTaskAquireTemp */
/**
* @brief Function implementing the vTaskAquireTemp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskAquireTemp */
void StartTaskAquireTemp(void const * argument)
{
  /* USER CODE BEGIN StartTaskAquireTemp */
  /* Infinite loop */
  for(;;)
  {
	  /* FORCED*/
	  int16_t temperature;
	  static int16_t last_temperature=0;
	  uint8_t humidity;
	  static uint8_t last_humidity=0;
	  rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &dev);
	  osDelay(40);
	  rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
	  if(rslt == BME280_OK){
		  temperature = comp_data.temperature / 10;      // ºC
		  humidity = comp_data.humidity / 1024.0;           // %
		  _mains._actual_temp=temperature;
		  _mains._actual_humm=humidity;
		  if(last_temperature!=temperature){
			  _mains._fl_sys_changed|=1;
			  last_temperature=temperature;
		  }
		  if(last_humidity!=humidity){
			  _mains._fl_sys_changed|=2;
			  last_humidity=humidity;
		  }
//		  xQueueOverwrite(Queue_Real_TempHandle, &temperature);
//		  xQueueOverwrite(Queue_Real_HumHandle, &humidity);
	  }
	  osDelay(60);	// periodo de amostragem de 100ms
  }
  /* USER CODE END StartTaskAquireTemp */
}

/* USER CODE BEGIN Header_StartTaskAquireBright */
/**
* @brief Function implementing the vTaskAquireBrig thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskAquireBright */
void StartTaskAquireBright(void const * argument)
{
  /* USER CODE BEGIN StartTaskAquireBright */
  /* Infinite loop */
  for(;;)
  {
	  uint16_t ir_value=0;
	  uint16_t vl_value=0;
	  uint16_t lux_value=0;
	  static uint16_t last_lux_value=0;
	  enable(&in_sensor);
	  osDelay(15);
	  getData (&in_sensor, &vl_value, &ir_value);
	  disable(&in_sensor);
	  lux_value=TSL2561_calculateLux(vl_value, ir_value);
	  //mandar para queue
	  _mains._actual_lux=lux_value;
	  if(last_lux_value!=lux_value){
		  _mains._fl_sys_changed|=4;
		  last_lux_value=lux_value;
	  }
//	  xQueueOverwrite(Queue_Real_LuminHandle, &lux_value);
	  osDelay(135);	// periodo de amostragem de 150ms
  }
  /* USER CODE END StartTaskAquireBright */
}

/* USER CODE BEGIN Header_StartTaskSetSecureMode */
/**
* @brief Function implementing the vTaskSetSecureM thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskSetSecureMode */
void StartTaskSetSecureMode(void const * argument)
{
  /* USER CODE BEGIN StartTaskSetSecureMode */
  /* Infinite loop */
  for(;;)
  {
	  ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
	  printf("Task_secure_active");
	  osTimerStart(Fast_TimerHandle,5000);	// 300 000 ms --> 5 min

    osDelay(10);
  }
  /* USER CODE END StartTaskSetSecureMode */
}

/* USER CODE BEGIN Header_StartTaskRlControl */
/**
* @brief Function implementing the vTaskRlControl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskRlControl */
void StartTaskRlControl(void const * argument)
{
  /* USER CODE BEGIN StartTaskRlControl */
  /* Infinite loop */
  for(;;)
  {
	  ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
//	  printf("RL_Control_function");
//	  uint8_t states_rl=0;

	  if((_mains._fl_fan_control==1)&&((_mains._fl_states_rl==1)||(_mains._fl_states_rl==2))){

	  }
    osDelay(1);
  }
  /* USER CODE END StartTaskRlControl */
}

/* USER CODE BEGIN Header_StartTaskLightActuator */
/**
* @brief Function implementing the vTaskLightActua thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLightActuator */
void StartTaskLightActuator(void const * argument)
{
  /* USER CODE BEGIN StartTaskLightActuator */
  /* Infinite loop */
  for(;;)
  {

	  ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
	  actuate_light((5+(_mains._setpoint_lux/1.06)+(-(_mains._setpoint_lux/1.06)+(_mains._setpoint_lux/3)+(_mains._actual_lux/10))*_mains._fl_lux_mode));
	  osDelay(10);
  }
  /* USER CODE END StartTaskLightActuator */
}

/* Callback_Fast_Timer function */
void Callback_Fast_Timer(void const * argument)
{
  /* USER CODE BEGIN Callback_Fast_Timer */
//	printf("Fast_Timer_ends");
	osTimerStart(Slow_TimerHandle,10000);	// 600 000 ms --> 10 min
	//block the motor
	_mains._fl_motor_blocked=1;
	stop_motor();
  /* USER CODE END Callback_Fast_Timer */
}

/* Callback_Slow_Timer function */
void Callback_Slow_Timer(void const * argument)
{
  /* USER CODE BEGIN Callback_Slow_Timer */
//	printf("Slow_Timer_ends");
	//unblock the motor
	_mains._fl_motor_blocked=0;
  /* USER CODE END Callback_Slow_Timer */
}

/* Callback_Sampling_Timer function */
void Callback_Sampling_Timer(void const * argument)
{
  /* USER CODE BEGIN Callback_Sampling_Timer */
//	xTaskNotifyGive( vTaskRlControlHandle );
	xTaskNotifyGive( vTaskLightActuaHandle );
  /* USER CODE END Callback_Sampling_Timer */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
