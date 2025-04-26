/*
 * API_debounce.c
 *
 *  Created on: Mar 27, 2025
 *      Author: Gus
 */

#include "API_debounce.h"

#define LONG_PRESSED_DELAY		3000
#define DEBOUNCE_DELAY			40

static const uint8_t verb = 0;

static debounceState_t current_state;
static delay_t delay_debounce;
static delay_t timer_pressed;

static keyState_t key = KEY_NO_PRESS;

static GPIO_TypeDef * buttonPort;
static uint16_t buttonPin;

/**
 * @brief Cambia el estado actual de la máquina de estados.
 * @param new_state Nuevo estado al cual se cambiará.
 */
static void setState(debounceState_t new_state)
{
	current_state = new_state;
}

/**
 * @brief Lee el estado actual del botón desde el puerto GPIO configurado.
 * @return Estado actual del pin GPIO (GPIO_PIN_SET o GPIO_PIN_RESET).
 */
static GPIO_PinState readButton()
{
	return HAL_GPIO_ReadPin(buttonPort, buttonPin);
}

/**
 * @brief Acción a realizar cuando el botón es presionado.
 */
static void buttonPressed()
{
	if(verb>0) uartSendString((uint8_t *)"BUTTON PRESSED\n\r");
}

/**
 * @brief Acción a realizar cuando el botón es liberado.
 * @note  Setea key dependiendo si se libera desde un single press o un long press
 * @param long_pressed: bool que indica si se libera luego de un long press
 */
static void buttonReleased(bool_t long_pressed)
{
	if(long_pressed)
	{
		if(verb>0) uartSendString((uint8_t *)"LIBERADO DESPUES DE LONG PRESSED\n\r");
		key = KEY_LONG_PRESS;
	}
	else
	{
		if(verb>0) uartSendString((uint8_t *)"LIBERADO DESPUES DE SINGLE PRESSED\n\r");
		if(key != KEY_LONG_PRESS) key = KEY_SINGLE_PRESS; // no sobreescribir
	}
}

/**
 * @brief Inicializa la máquina de estados para el debounce del botón.
 * @param GPIOx Puerto GPIO del botón.
 * @param GPIO_Pin Pin GPIO del botón.
 */
void debounceFSM_init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	buttonPort = GPIOx;
	buttonPin = GPIO_Pin;
	current_state = BUTTON_UP;
	delayInit(&delay_debounce, DEBOUNCE_DELAY);
	delayInit(&timer_pressed, LONG_PRESSED_DELAY);
}

/**
 * @brief Actualiza la máquina de estados para el debounce. Debe llamarse periódicamente.
 */
void debounceFSM_update()
{
	static bool_t long_pressed = false;
	switch(current_state)
	{
		case BUTTON_UP:
		{
			if(readButton() == GPIO_PIN_RESET)
			{
				setState(BUTTON_FALLING);
				delayRead(&delay_debounce);
			}
			break;
		}

		case BUTTON_FALLING:
		{
			if(delayRead(&delay_debounce))
			{
				if(readButton() == GPIO_PIN_RESET)
				{
					setState(BUTTON_DOWN);
					buttonPressed();
					delayRead(&timer_pressed); // reset
				}
				else
				{
					setState(BUTTON_UP);
				}
			}
			break;
		}

		case BUTTON_DOWN:
		{
			if(delayRead(&timer_pressed))
			{
				setState(BUTTON_TOO_DOWN);
			}

			if(readButton() == GPIO_PIN_SET)
			{
				setState(BUTTON_RAISING);
				delayRead(&delay_debounce);
			}
			break;
		}

		case BUTTON_TOO_DOWN:
		{
			if(readButton() == GPIO_PIN_SET)
			{
				long_pressed = true;
				setState(BUTTON_RAISING);
				delayRead(&delay_debounce);
			}
			break;
		}

		case BUTTON_RAISING:
		{
			if(delayRead(&delay_debounce))
			{
				if(readButton() == GPIO_PIN_SET)
				{
					setState(BUTTON_UP);

					buttonReleased(long_pressed);
					long_pressed = false;

					delayRead(&timer_pressed);
				}
				else
				{
					setState(BUTTON_DOWN);
				}
			}
			break;
		}
	}
}

/**
 * @brief Lee el valor del flag que indica si el botón fue presionado una vez o durante un tiempo.
 *        Luego de leer, restablece el flag.
 * @return key state.
 */
keyState_t readKey()
{
	keyState_t ret = key;
	if(key != KEY_NO_PRESS) key = KEY_NO_PRESS;

	return ret;
}
