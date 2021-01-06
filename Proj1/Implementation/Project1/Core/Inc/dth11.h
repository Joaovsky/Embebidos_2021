/*
 * fdg.h
 *
 *  Created on: 12/12/2020
 *      Author: Tiago
 */

#ifndef SRC_DTH11_H_
#define SRC_DTH11_H_

#include <stdint.h>
#include <stdbool.h>

void start_start_sign(void);

void stop_start_sign(void);

void stop_sign(void);

bool verifyData(char *buff);

uint8_t getHumm(char *buff);

float getTemp(char *buff);


#endif /* SRC_FDG_H_ */
