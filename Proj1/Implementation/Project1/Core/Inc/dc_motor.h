/*
 * dc_motor.h
 *
 *  Created on: Dec 14, 2020
 *      Author: Tiago
 */

#ifndef SRC_DC_MOTOR_H_
#define SRC_DC_MOTOR_H_

#include <stdint.h>
#include <stdbool.h>


bool start_motor(void);

bool stop_motor(void);

bool verify_motor(void);


#endif /* SRC_DC_MOTOR_H_ */
