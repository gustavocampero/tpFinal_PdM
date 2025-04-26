/*
 * API_delay.h
 *
 *  Created on: Mar 20, 2025
 *      Author: Gus
 */

#ifndef API_API_DELAY_H_
#define API_API_DELAY_H_

#include "stm32f4xx_hal.h"

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );
void delayStop(delay_t * delay);

bool_t delayIsRunning(delay_t * delay);

#endif /* API_API_DELAY_H_ */
