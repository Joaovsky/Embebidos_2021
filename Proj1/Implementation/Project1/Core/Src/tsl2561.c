/*
 * tsl2561.c
 *
 *  Created on: 30/04/2020
 *      Author: Tiago
 */
//#include <stdbool.h>
#include <math.h>
#include "stm32f7xx_hal.h"
#include "i2c.h"
#include "tsl2561.h"


#define LUX_SENSOR_ADDR (0x0029<<1)

Lux_sensor in_sensor;



/**************************************************************************/
/*!
    Enables the device
    @param sensor The all parameters of sensor
*/
/**************************************************************************/
void enable(Lux_sensor *sensor){
  /* Enable the device by setting the control bit to 0x03 */
  write8(TSL2561_COMMAND_BIT | TSL2561_REGISTER_CONTROL, TSL2561_CONTROL_POWERON, LUX_SENSOR_ADDR);
}

/**************************************************************************/
/*!
    Disables the device (putting it in lower power sleep mode)
    @param sensor The all parameters of sensor
*/
/**************************************************************************/
void disable(Lux_sensor *sensor){
  /* Turn the device off to save power */
  write8(TSL2561_COMMAND_BIT | TSL2561_REGISTER_CONTROL, TSL2561_CONTROL_POWEROFF,LUX_SENSOR_ADDR);
}

/**************************************************************************/
/*!
    Private function to read luminosity on both channels
*/
/**************************************************************************/
void getData (Lux_sensor *sensor, uint16_t *broadband, uint16_t *ir){

  /* Reads a two byte value from channel 0 (visible + infrared) */
  *broadband = read16(TSL2561_COMMAND_BIT | TSL2561_WORD_BIT | TSL2561_REGISTER_CHAN0_LOW, sensor->_addr);

  /* Reads a two byte value from channel 1 (infrared) */
  *ir = read16(TSL2561_COMMAND_BIT | TSL2561_WORD_BIT | TSL2561_REGISTER_CHAN1_LOW, sensor->_addr);

}



/**************************************************************************/
/*!
    @brief Initialize the sensor
    @param sensor The all parameters of sensor
    @param hi2c Pointer to I2c channel in use (I2C address of this chip is 0x29)
*/
/**************************************************************************/
bool TSL2561_Init(Lux_sensor *sensor){
	sensor->_addr = LUX_SENSOR_ADDR;
	sensor->_tsl2561IntegrationTime = TSL2561_INTEGRATIONTIME_13MS;
	sensor->_tsl2561Gain = TSL2561_GAIN_1X;
  /* Make sure we're actually connected */
    uint8_t x = read8(TSL2561_REGISTER_ID, sensor->_addr);
    if (x & 0x05) { // ID code for TSL2561
      return false;
    }
    sensor->_tsl2561Initialised = true;

    /* Set default integration time and gain */
    TSL2561_setIntegrationTime(sensor, sensor->_tsl2561IntegrationTime);
    TSL2561_setGain(sensor, sensor->_tsl2561Gain);

    /* Note: by default, the device is in power down mode on bootup */
    disable(sensor);
return true;
}

/**************************************************************************/
/*!
    @brief      Sets the integration time for the TSL2561. Higher time means
    more light captured (better for low light conditions) but will
	take longer to run readings.
	@param sensor The all parameters of sensor
    @param time The amount of time we'd like to add up values
*/
/**************************************************************************/
void TSL2561_setIntegrationTime(Lux_sensor *sensor, tsl2561IntegrationTime_t time){
  if (!sensor->_tsl2561Initialised) TSL2561_Init(sensor);

  /* Enable the device by setting the control bit to 0x03 */
  enable(sensor);

  /* Update the timing register */
  write8(TSL2561_COMMAND_BIT | TSL2561_REGISTER_TIMING, time | sensor->_tsl2561Gain, sensor->_addr);

  /* Update value placeholders */
  sensor->_tsl2561IntegrationTime = time;

  /* Turn the device off to save power */
  disable(sensor);
}

/**************************************************************************/
/*!
    @brief  Adjusts the gain on the TSL2561 (adjusts the sensitivity to light)
    @param sensor The all parameters of sensor
    @param gain The value we'd like to set the gain to
*/
/**************************************************************************/
void TSL2561_setGain(Lux_sensor *sensor, tsl2561Gain_t gain){
  if (!sensor->_tsl2561Initialised) TSL2561_Init(sensor);

  /* Enable the device by setting the control bit to 0x03 */
  enable(sensor);

  /* Update the timing register */
  write8(TSL2561_COMMAND_BIT | TSL2561_REGISTER_TIMING, sensor->_tsl2561IntegrationTime | gain, sensor->_addr);

  /* Update value placeholders */
  sensor->_tsl2561Gain = gain;

  /* Turn the device off to save power */
  disable(sensor);
}

/**************************************************************************/
/*!		(NOT IN USE)
    @brief  Gets the broadband (mixed lighting) and IR only values from
            the TSL2561, adjusting gain if auto-gain is enabled
    @param sensor The all parameters of sensor
    @param  broadband Pointer to a uint16_t we will fill with a sensor
                      reading from the IR+visible light diode.
    @param  ir Pointer to a uint16_t we will fill with a sensor the
               IR-only light diode.
*/
/**************************************************************************/
void TSL2561_getLuminosity (Lux_sensor *sensor, uint16_t *broadband, uint16_t *ir){
	  /* Enable the device by setting the control bit to 0x03 */
	  enable(sensor);

	  /* Wait x ms for ADC to complete */
	  switch (sensor->_tsl2561IntegrationTime)
	  {
	    case TSL2561_INTEGRATIONTIME_13MS:
	    	 HAL_Delay(TSL2561_DELAY_INTTIME_13MS);  // KTOWN: Was 14ms
	      break;
	    case TSL2561_INTEGRATIONTIME_101MS:
	    	 HAL_Delay(TSL2561_DELAY_INTTIME_101MS); // KTOWN: Was 102ms
	      break;
	    default:
	    	 HAL_Delay(TSL2561_DELAY_INTTIME_402MS); // KTOWN: Was 403ms
	      break;
	  }

	  /* Reads a two byte value from channel 0 (visible + infrared) */
	  *broadband = read16(TSL2561_COMMAND_BIT | TSL2561_WORD_BIT | TSL2561_REGISTER_CHAN0_LOW, sensor->_addr);

	  /* Reads a two byte value from channel 1 (infrared) */
	  *ir = read16(TSL2561_COMMAND_BIT | TSL2561_WORD_BIT | TSL2561_REGISTER_CHAN1_LOW, sensor->_addr);

	  /* Turn the device off to save power */
	  disable(sensor);
	}

/**************************************************************************/
/*!
    @brief  Converts the raw sensor values to the standard SI lux equivalent.
    @param  broadband The 16-bit sensor reading from the IR+visible light diode.
    @param  ir The 16-bit sensor reading from the IR-only light diode.
    @returns The integer Lux value we calcuated.
             Returns 0 if the sensor values are unreliable, or 65536 if the sensor is saturated.
*/
/**************************************************************************/
uint16_t TSL2561_calculateLux(uint16_t broadband, uint16_t ir) {
	double ratio=0;
	uint16_t x=0;
		// Convert from unsigned integer to floating point
	ratio = ir / broadband;
	if(ratio<=0.5){x = 0.0304 * broadband - 0.062 * ir * pow(ratio,1.4);}
	else if(ratio<=0.61){x = 0.0224 * broadband - 0.031 * ir;}
	else if(ratio<=0.8){x = 0.0128 * broadband - 0.0153 * ir;}
	else if(ratio<=1.3){x = 0.00146 * broadband - 0.00112 * ir;}
	else{x=0;}
  return x;
}



