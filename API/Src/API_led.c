/*
 * API_led.c
 *
 *  Created on: Apr 24, 2025
 *      Author: Gus
 */

#include "API_led.h"

static GPIO_TypeDef* _ledPort;
static uint16_t _ledPin;
static tick_t _onTime, _offTimeLong, _offTimeShort;
static uint8_t _cantBlink;
static running_t _running;

static ledState_t current_state;

static delay_t delay_led;

/**
  * @brief  Funcion para cambiar el valor de current_state
  * @param	ledState_t nuevo estado
  */
static void setState(ledState_t state)
{
	current_state = state;
}

/**
  * @brief  led_On Activa el Led.
  */
void led_On(void)
{
	HAL_GPIO_WritePin(_ledPort, _ledPin, GPIO_PIN_SET);
}

/**
  * @brief  led_On Desactiva el Led.
  */
void led_Off(void)
{
	HAL_GPIO_WritePin(_ledPort, _ledPin, GPIO_PIN_RESET);
}

/**
  * @brief  Inicializa led
  * @param	ledPort: puerto GPIO del led
  * @param	ledPin: pin GPIO del led
  * @param	onTime: tiempo en estado ON
  * @param	offTimeLong: tiempo en estado off largo
  * @param	offTimeShort: tiempo en estado off corto
  * @param	cantBlink: cantidad de veces que blinkea en un combo
  */
void ledInit(GPIO_TypeDef* ledPort, uint16_t ledPin, tick_t onTime, tick_t offTimeLong, tick_t offTimeShort, uint8_t cantBlink)
{
	_ledPort = ledPort;
	_ledPin = ledPin;
	_onTime = onTime;
	_offTimeLong = offTimeLong;
	_offTimeShort = offTimeShort;
	_cantBlink = cantBlink;

	_running = LED_STOP;
	current_state = LED_OFF;
	delayInit(&delay_led, _onTime);
	led_Off();
}

/**
  * @brief  led FSM
  */
void ledFSM_update()
{
	static uint8_t aux_cantBlink;
	switch(current_state)
	{
		case LED_OFF:
		{
			if(_running > LED_STOP)
			{
				aux_cantBlink = _cantBlink;
				delayWrite(&delay_led, _onTime);
				delayRead(&delay_led);

				led_On();
				setState(LED_ON);
			}
			break;
		}

		case LED_ON:
		{
			if(delayRead(&delay_led))
			{
				aux_cantBlink--;
				if(aux_cantBlink > 0)
				{
					delayWrite(&delay_led, _offTimeShort);
					led_Off();
					setState(LED_OFF_SHORT);
				}
				else
				{
					aux_cantBlink = _cantBlink;
					if(_running == LED_BLINK_FOREVER)
					{
						delayWrite(&delay_led, _offTimeLong);
						led_Off();
						setState(LED_OFF_LONG);
					}
					else
					{
						_running = LED_STOP;
						led_Off();
						setState(LED_OFF);
					}
				}
			}
			break;
		}

		case LED_OFF_SHORT:
		{
			if(delayRead(&delay_led))
			{
				delayWrite(&delay_led, _onTime);
				led_On();
				setState(LED_ON);
			}
			break;
		}

		case LED_OFF_LONG:
		{
			if(delayRead(&delay_led))
			{
				delayWrite(&delay_led, _onTime);
				led_On();
				setState(LED_ON);
			}
			break;
		}
	}
}

/**
  * @brief  Start led
  * @param	running: tipo de blinkeo que se quiere activar
  */
void ledStart(running_t running)
{
	_running = running;
}

/**
  * @brief  Stop led (en el caso que se haya activado un blink forever)
  */
void ledStop()
{
	_running = LED_STOP;
	setState(LED_OFF);
}
