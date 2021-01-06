/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <float.h>
#include <stdbool.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

double getReward(char b_state, char Action)
{
  const signed int iv[7] = { 100, -1, -2, -5, -5, -2, -1 };

  const signed int iv1[5] = { 2, 1, 0, 1, 2 };

  /* Reward is a function of state and Action */
  return (double)iv[b_state-1] + 0.5*(double)iv1[Action];
}

double rtNaN = (double)NAN;
double rtInf = (double)INFINITY;
double rtMinusInf = -(double)INFINITY;
double rtNaNF = (double)NAN;
double rtInfF = (double)INFINITY;
double rtMinusInfF = -(double)INFINITY;

#if defined(__ICL) && __ICL == 1700

#pragma warning(default: 264)

#endif

/* Function: rtIsInf ==================================================
 * Abstract:
 * Test if value is infinite
 */
bool rtIsInf(double value)
{
  return (isinf(value) != 0U);
}

/* Function: rtIsInfF =================================================
 * Abstract:
 * Test if single-precision value is infinite
 */
bool rtIsInfF(double value)
{
  return (isinf((double)value) != 0U);
}

/* Function: rtIsNaN ==================================================
 * Abstract:
 * Test if value is not a number
 */
bool rtIsNaN(double value)
{
  return (isnan(value) != 0U);
}

/* Function: rtIsNaNF =================================================
 * Abstract:
 * Test if single-precision value is not a number
 */
bool rtIsNaNF(double value)
{
  return (isnan((double)value) != 0U);
}

double rtGetInf(void)
{
  return rtInf;
}

/* Function: rtGetInfF =================================================================
 * Abstract:
 * Initialize rtInfF needed by the generated code.
 */
double rtGetInfF(void)
{
  return rtInfF;
}

/* Function: rtGetMinusInf =============================================================
 * Abstract:
 * Initialize rtMinusInf needed by the generated code.
 */
double rtGetMinusInf(void)
{
  return rtMinusInf;
}

/* Function: rtGetMinusInfF ============================================================
 * Abstract:
 * Initialize rtMinusInfF needed by the generated code.
 */
double rtGetMinusInfF(void)
{
  return rtMinusInfF;
}

double rtGetNaN(void)
{
  return rtNaN;
}

/* Function: rtGetNaNF =====================================================================
 * Abstract:
 * Initialize rtNaNF needed by the generated code.
 * NaN is initialized as non-signaling. Assumes IEEE.
 */
double rtGetNaNF(void)
{
  return rtNaNF;
}

static double rt_remd_snf(double u0, double u1)
{
  double q;
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1) || rtIsInf(u0)) {
    y = rtNaN;
  } else if (rtIsInf(u1)) {
    y = u0;
  } else if ((u1 != 0.0) && (u1 != trunc(u1))) {
    q = fabs(u0 / u1);
    if (!(fabs(q - floor(q + 0.5)) > DBL_EPSILON * q)) {
      y = 0.0 * u0;
    } else {
      y = fmod(u0, u1);
    }
  } else {
    y = fmod(u0, u1);
  }

  return y;
}

char getState(double ref, double cur)
{
  static const signed char lookup_table[45] = { 5, 5, 5, 5, 5, 0, 0, 0, 0, 6, 6,
    6, 6, 6, 0, 0, 0, 0, 7, 7, 7, 7, 1, 2, 2, 2, 2, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0,
    0, 0, 0, 4, 4, 4, 4, 4 };

  double d;
  char s;

  d = ref - cur;
  if (d >= 15.0) {
    s = 4;
  } else if (cur - ref >= 15.0) {
    s = 5;
  } else {
    /* Get State */
    s = lookup_table[((int)(trunc(rt_remd_snf(d, 5.0)) + 5.0) + 9 * ((int)(trunc
      (d / 5.0) + 3.0) - 1)) - 1];
  }

  return s;
}

void environment(signed char Action, int ref_temp,
                 double *cur_temp, char *b_state, double *rewards)
{
  const signed int iv1[5] = { -2, -1, 0, 1, 2 };

  *cur_temp = (*cur_temp + 0.5 * iv1[Action] );

  /* Determine state and reward */
  *b_state = getState(ref_temp, *cur_temp);

  /* Give reward according to the respective state */
  /* Reward is a function of state and Action */
	*rewards = getReward(*b_state,Action);
}

int best_action(int m, int n, double arr[7][5], int cs) 
{ 
     
    // Initialize maximum element 
    double max = arr[cs-1][0]; 
		int choosen_action = 0;
	
    // Traverse array elements from second and 
    // compare every element with current max   
    for (int i = 0; i < n; i++) 
        if (arr[cs-1][i] > max) 
				{
					max = arr[cs-1][i]; 
					choosen_action = i;
				}
    return choosen_action; 
} 

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	
	//PWM
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	//uint8_t  duty =100;
	
	//Initialize Qtable
	int N_states = 7;
	int N_actions = 5;
	
		double Q_table[7][5] = {
	 {12.8684,   44.2582,  200.0000,   51.7647,   52.2693},
   {10.4893,   15.9640,   11.6202,   59.3819,  131.5921},
	 {-3.5485,   -4.4212,    0.6732,   -4.4901,   36.4415},
   {-0.5118,   -1.9012,   -0.2872,    0.7894,    0.1470},
   {1.4994,  -9.5072,   -9.3773,   -9.2471,   -9.4757},
   {-2.6484,   20.5034,   -1.4582,   -1.5423,   -3.4611},
   {124.7742,   15.4848,   19.2549,    8.5964,   18.2362}
	 };

	//Initial Parameters
	char Action = 2;
	double Disc_factor = 0.5;           	
	char cs, ns;
  double r;
	double max_q = 0;
  int ref_temp = 27;
	double initial_cur_temp = 20;
	double *actual_temp = &initial_cur_temp;
	double Learn_rate = 0.3;
	int it=0;
	//Initialize S
	environment(Action, ref_temp, actual_temp, &cs, &r);

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  //MX_FREERTOS_Init();
  /* Start scheduler */
  //osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	htim3.Instance->CCR2 = 10;
	//int prob;
  while (1)
  {
		ref_temp = 27;
		initial_cur_temp = 16;
		ns = getState(ref_temp, *actual_temp);
		cs = ns;
		while(ns != 1)
		{		
		//Action = best_action(N_states, N_actions, Q_table, cs);
	  Action = best_action(N_states, N_actions, Q_table, ns);
			
		/* Take Action A, observe R, S' */
		environment(Action, ref_temp, actual_temp, &ns, &r);
		//htim3.Instance->CCR1 = (100/(N_actions-1))*Action;
		
		/* Determine max Q value of given Action and next state */		
		//max_q = Q_table[ns][best_action(N_states, N_actions, Q_table, ns)];

		/*Update Q value from current state and Action */
		//Q_table[cs][Action] = (1-Learn_rate)*Q_table[cs][Action] + Learn_rate*(/*getReward(cs, Action)*/r + Disc_factor*max_q);
		
		/*S <- S'*/
		//cs = ns;
			
		it = it+1;
		}
		it = 0;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
		//htim3.Instance->CCR2 = 100;
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
