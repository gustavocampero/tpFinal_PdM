/*
 * API_uart.c
 *
 *  Created on: Apr 3, 2025
 *      Author: Gus
 */

#include "API_uart.h"

#define UART_TIMEOUT	10000

static UART_HandleTypeDef uartHandler;

/**
 * @brief Inicializa la interfaz UART2 con una configuración predeterminada.
 *
 * Configura parámetros como baud rate, bits de datos, bits de parada,
 * paridad, control de flujo y modo de operación.
 *
 * @return true si la inicialización fue exitosa, false en caso contrario.
 */
bool_t uartInit()
{
	HAL_StatusTypeDef uartRet;

	uartHandler.Instance = USART2;
	uartHandler.Init.BaudRate = 115200;
	uartHandler.Init.WordLength = UART_WORDLENGTH_8B;
	uartHandler.Init.StopBits = UART_STOPBITS_1;
	uartHandler.Init.Parity = UART_PARITY_NONE;
	uartHandler.Init.Mode = UART_MODE_TX_RX;
	uartHandler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartHandler.Init.OverSampling = UART_OVERSAMPLING_16;

	uartRet = HAL_UART_Init(&uartHandler);

	if(uartRet == HAL_OK) return true;
	else return false;
}

/**
 * @brief Envía una cadena de texto por UART.
 *
 * La cadena debe estar finalizada en NULL. Usa la función HAL_UART_Transmit
 * para enviar los datos.
 *
 * @param pstring Puntero a la cadena de caracteres a enviar.
 * @return true si la transmisión fue exitosa, false si falló o si el puntero es NULL.
 */
bool_t uartSendString(uint8_t * pstring)
{
	if(pstring == NULL)	return false;
	HAL_StatusTypeDef uartRet = HAL_UART_Transmit(&uartHandler, pstring, strlen((const char *)pstring), UART_TIMEOUT);
	if(uartRet == HAL_OK) return true;
	else return false;
}

/**
 * @brief Envía una cantidad específica de bytes por UART.
 *
 * No requiere que la cadena esté finalizada en NULL. Se especifica el tamaño a enviar.
 *
 * @param pstring Puntero a los datos a enviar.
 * @param size Cantidad de bytes a transmitir.
 * @return true si la transmisión fue exitosa, false si falló o si el puntero es NULL o el tamaño es 0.
 */
bool_t uartSendStringSize(uint8_t * pstring, uint16_t size)
{
	if(pstring == NULL)	return false;
	if(size > 0)
	{
		HAL_StatusTypeDef uartRet = HAL_UART_Transmit(&uartHandler, pstring, size, UART_TIMEOUT);
		if(uartRet == HAL_OK) return true;
	}
	return false;
}

/**
 * @brief Recibe una cantidad específica de bytes por UART.
 *
 * Espera recibir exactamente la cantidad de bytes especificada y los guarda en el buffer.
 *
 * @param pstring Puntero al buffer donde se almacenarán los datos recibidos.
 * @param size Cantidad de bytes a recibir.
 * @return true si la recepción fue exitosa, false si falló, si el puntero es NULL o si el tamaño es 0.
 */
bool_t uartReceiveStringSize(uint8_t * pstring, uint16_t size)
{
	if(pstring == NULL)	return false;
	if(size > 0)
	{
		HAL_StatusTypeDef uartRet = HAL_UART_Receive(&uartHandler, pstring, size, UART_TIMEOUT);
		if(uartRet == HAL_OK) return true;
	}
	return false;
}
