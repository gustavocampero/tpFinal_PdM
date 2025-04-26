/*
 * API_led.h
 *
 *  Created on: Apr 24, 2025
 *      Author: Gus
 */

#ifndef API_INC_API_LED_H_
#define API_INC_API_LED_H_

#include "stm32f4xx_hal.h"

#include "API_delay.h"

typedef enum{
	LED_OFF,
	LED_ON,
	LED_OFF_SHORT,
	LED_OFF_LONG,
}ledState_t;

typedef enum{
	LED_STOP = 0,
	LED_BLINK_ONCE,
	LED_BLINK_FOREVER,
}running_t;

void ledFSM_update();

void ledInit(GPIO_TypeDef* ledPort, uint16_t ledPin, tick_t onTime, tick_t offTimeLong, tick_t offTimeShort, uint8_t cantBlink);
void ledStart(running_t running);
void ledStop();

#endif /* API_INC_API_LED_H_ */
