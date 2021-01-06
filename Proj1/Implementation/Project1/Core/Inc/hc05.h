/*
 * hc05.h
 *
 *  Created on: Dec 10, 2020
 *      Author: Tiago
 */

#ifndef SRC_HC05_H_
#define SRC_HC05_H_

#include <stdint.h>
#include <stdbool.h>

#define BTH_EN_Pin GPIO_PIN_7
#define BTH_EN_GPIO_Port GPIOD
#define BTH_ST_Pin GPIO_PIN_4
#define BTH_ST_GPIO_Port GPIOD

bool verifyConnection(void);
bool send_bth(uint8_t *str);
bool receive_bth(void);








#endif /* SRC_HC05_H_ */
