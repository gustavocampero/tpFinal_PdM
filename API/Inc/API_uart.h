/*
 * API_uart.h
 *
 *  Created on: Apr 3, 2025
 *      Author: Gus
 */

#ifndef INC_API_UART_H_
#define INC_API_UART_H_

#include "stm32f4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef bool bool_t;

bool_t uartInit();
// Se cambia el tipo de respuesta de las siguientes funciones para registrar la verificacion de las funciones de la HAL utilizadas
bool_t uartSendString(uint8_t * pstring);
bool_t uartSendStringSize(uint8_t * pstring, uint16_t size);
bool_t uartReceiveStringSize(uint8_t * pstring, uint16_t size);


#endif /* INC_API_UART_H_ */
