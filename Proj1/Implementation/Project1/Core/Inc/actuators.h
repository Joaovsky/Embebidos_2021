/*
 * fan_heating.h
 *
 *  Created on: Dec 14, 2020
 *      Author: Tiago
 */

#ifndef SRC_FAN_HEATING_H_
#define SRC_FAN_HEATING_H_

#include <stdint.h>
#include <stdbool.h>

bool actuate_light(uint8_t pwm);

bool actuate_fan(uint8_t pwm);

bool actuate_heating(uint8_t pwm);

uint8_t getPWM_fan(void);

uint8_t getPWM_heating(void);


#endif /* SRC_FAN_HEATING_H_ */
