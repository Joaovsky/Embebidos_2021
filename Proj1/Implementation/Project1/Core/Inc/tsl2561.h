/*
 * TSL2561.h
 *
 *  Created on: Dec 10, 2020
 *      Author: Tiago
 */

#ifndef SRC_TSL2561_H_
#define SRC_TSL2561_H_

#include <stdint.h>
#include <stdbool.h>

// TSL2561 addr
#define LUX_SENSOR_ADDR (0x0029<<1)


// TSL2561 registers
#define TSL2561_COMMAND_BIT       (0x80)    ///< Must be 1
#define TSL2561_CLEAR_BIT         (0x40)    ///< Clears any pending interrupt (write 1 to clear)
#define TSL2561_WORD_BIT          (0x20)    ///< 1 = read/write word (rather than byte)
#define TSL2561_BLOCK_BIT         (0x10)    ///< 1 = using block read/write
#define TSL2561_CONTROL_POWERON   (0x03)    ///< Control register setting to turn on
#define TSL2561_CONTROL_POWEROFF  (0x00)    ///< Control register setting to turn off

// Delay for integration times
#define TSL2561_DELAY_INTTIME_13MS    (15)    ///< Wait 15ms for 13ms integration
#define TSL2561_DELAY_INTTIME_101MS   (120)   ///< Wait 120ms for 101ms integration
#define TSL2561_DELAY_INTTIME_402MS   (450)   ///< Wait 450ms for 402ms integration

/** TSL2561 I2C Registers */
enum
{
  TSL2561_REGISTER_CONTROL          = 0x00, // Control/power register
  TSL2561_REGISTER_TIMING           = 0x01, // Set integration time register
  TSL2561_REGISTER_THRESHHOLDL_LOW  = 0x02, // Interrupt low threshold low-byte
  TSL2561_REGISTER_THRESHHOLDL_HIGH = 0x03, // Interrupt low threshold high-byte
  TSL2561_REGISTER_THRESHHOLDH_LOW  = 0x04, // Interrupt high threshold low-byte
  TSL2561_REGISTER_THRESHHOLDH_HIGH = 0x05, // Interrupt high threshold high-byte
  TSL2561_REGISTER_INTERRUPT        = 0x06, // Interrupt settings
  TSL2561_REGISTER_CRC              = 0x08, // Factory use only
  TSL2561_REGISTER_ID               = 0x0A, // TSL2561 identification setting
  TSL2561_REGISTER_CHAN0_LOW        = 0x0C, // Light data channel 0, low byte
  TSL2561_REGISTER_CHAN0_HIGH       = 0x0D, // Light data channel 0, high byte
  TSL2561_REGISTER_CHAN1_LOW        = 0x0E, // Light data channel 1, low byte
  TSL2561_REGISTER_CHAN1_HIGH       = 0x0F  // Light data channel 1, high byte
};

/** Three options for how long to integrate readings for */
typedef enum
{
  TSL2561_INTEGRATIONTIME_13MS      = 0x00,    // 13.7ms
  TSL2561_INTEGRATIONTIME_101MS     = 0x01,    // 101ms
  TSL2561_INTEGRATIONTIME_402MS     = 0x02     // 402ms
}
tsl2561IntegrationTime_t;

/** TSL2561 offers 2 gain settings */
typedef enum
{
  TSL2561_GAIN_1X                   = 0x00,    // No gain
  TSL2561_GAIN_16X                  = 0x10,    // 16x gain
}
tsl2561Gain_t;



typedef struct tsl2561_defs{
	int8_t _addr;
	bool _tsl2561Initialised;
	tsl2561IntegrationTime_t _tsl2561IntegrationTime;
	tsl2561Gain_t _tsl2561Gain;
	int32_t _tsl2561SensorID;
}Lux_sensor;


extern Lux_sensor in_sensor;


/*========================================================================*/
/*                                FUNCTIONS                               */
/*========================================================================*/
void enable(Lux_sensor *sensor);
void disable(Lux_sensor *sensor);
bool TSL2561_Init(Lux_sensor *sensor);
void TSL2561_setIntegrationTime(Lux_sensor *sensor, tsl2561IntegrationTime_t time);
void TSL2561_setGain(Lux_sensor *sensor, tsl2561Gain_t gain);
void getData (Lux_sensor *sensor, uint16_t *broadband, uint16_t *ir);
void TSL2561_getLuminosity (Lux_sensor *sensor, uint16_t *broadband, uint16_t *ir);
uint16_t TSL2561_calculateLux( uint16_t broadband, uint16_t ir);






#endif /* SRC_TSL2561_H_ */
