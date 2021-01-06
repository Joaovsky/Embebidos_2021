/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20404768;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**************************************************************************/
/*!
    @brief  Writes a register and an 8 bit value over I2C
    @param  reg I2C register to write the value to
    @param  value The 8-bit value we're writing to the register
    @param _addr address of device
*/
/**************************************************************************/
void write8 (uint8_t reg, uint8_t value, int8_t _addr){
	uint8_t  buf[1]={0};
	buf[0] =reg;
	HAL_I2C_Master_Transmit(&hi2c1, _addr, buf, 1, HAL_MAX_DELAY);
	buf[0] =value;
	HAL_I2C_Master_Transmit(&hi2c1, _addr, buf, 1, HAL_MAX_DELAY);
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
    @param  reg I2C register to read from
    @param _addr address of device
    @returns 8-bit value containing single byte data read
*/
/**************************************************************************/
uint8_t read8(uint8_t reg, int8_t _addr){
	uint8_t  buf[1]={0};
	buf[0] =reg;
	HAL_I2C_Master_Transmit(&hi2c1, _addr, buf, 1, HAL_MAX_DELAY);
	buf[0] =0;
	HAL_I2C_Master_Receive(&hi2c1, _addr, buf, 1, HAL_MAX_DELAY);
	return buf[0];
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit values over I2C
    @param  reg I2C register to read from
    @param _addr address of device
    @returns 16-bit value containing 2-byte data read
*/
/**************************************************************************/
uint16_t read16(uint8_t reg, int8_t _addr){
  uint16_t x;
  uint8_t  buf[2]={0,0};
  buf[0] =reg;
  HAL_I2C_Master_Transmit(&hi2c1, _addr, buf, 1, HAL_MAX_DELAY);
  buf[0] =0;
  HAL_I2C_Master_Receive(&hi2c1, _addr, buf, 2, HAL_MAX_DELAY);
  x=buf[0];
  x <<= 8;
  x |= buf[1];
  return x;
}


/**************************************************************************/
/*!
    @brief  Reads a 32 bit values over I2C
    @param  reg I2C register to read from
    @param _addr address of device
    @returns 16-bit value containing 2-byte data read
*/
/**************************************************************************/
uint32_t read32(uint8_t reg, int8_t _addr){
  uint32_t x;
  uint8_t  buf[4]={0,0,0,0};
  buf[0] =reg;
  HAL_I2C_Master_Transmit(&hi2c1, _addr, buf, 1, HAL_MAX_DELAY);
  buf[0] =0;
  HAL_I2C_Master_Receive(&hi2c1, _addr, buf, 4, HAL_MAX_DELAY);
  x=buf[0];
  x <<= 8;
  x |= buf[1];
  x <<= 8;
  x |= buf[2];
  x <<= 8;
  x |= buf[3];

  return x;
}

int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
  if(HAL_I2C_Master_Transmit(&hi2c1, (id << 1), &reg_addr, 1, 10) != HAL_OK) return -1;
  if(HAL_I2C_Master_Receive(&hi2c1, (id << 1) | 0x01, data, len, 10) != HAL_OK) return -1;

  return 0;
}

void user_delay_ms(uint32_t period)
{
  HAL_Delay(period);
}

int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
  int8_t *buf;
  buf = (int8_t *)malloc(len +1);
  buf[0] = reg_addr;
  memcpy(buf +1, data, len);

  if(HAL_I2C_Master_Transmit(&hi2c1, (id << 1), (uint8_t*)buf, len + 1, HAL_MAX_DELAY) != HAL_OK) return -1;

  free(buf);
  return 0;
}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
