
/*
 *	@file port.c
 *	@version 1.0
 *	@date 19/4/2025
 *  @author Ing. Gustavo Campero
 *  @title API de manejo de SD Card (bajo nivel)
 *  @brief funciones port.c de bajo nivel
 */

#include "sd_card.h"
#include "stm32f4xx_hal.h" /* HAL include */

extern SPI_HandleTypeDef hspi2;

#define SPI2_CS_Pin GPIO_PIN_12
#define SPI2_CS_GPIO_Port GPIOB

static GPIO_TypeDef * csPort;
static uint16_t csbuttonPin;

/**
  * @brief  cs_Low Activa la línea CS (Chip Select) llevándola a nivel bajo.
  * @note   Utilizado para iniciar una comunicación SPI con la tarjeta SD.
  */
void cs_Low(void)
{
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  cs_High Desactiva la línea CS (Chip Select) llevándola a nivel alto.
  * @note   Utilizado para finalizar una comunicación SPI con la tarjeta SD.
  */
void cs_High(void)
{
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief  Ejecuta delay en milisegundos.
  * @param  ms: Milisegundos.
  */
void sd_Delay(uint32_t ms)
{
	HAL_Delay(ms);
}

/**
  * @brief  Envía y recibe datos simultáneamente por SPI.
  * @param  dataTx: Puntero al buffer de datos a transmitir.
  * @param  dataRx: Puntero al buffer donde se almacenarán los datos recibidos.
  * @param  size: Tamaño del buffer en bytes que se envían y reciben.
  * @retval Código de error de tipo sd_err_t
  */
sd_err_t sd_TransmitReceive(const uint8_t *dataTx, uint8_t *dataRx, uint16_t size)
{
	return (sd_err_t)HAL_SPI_TransmitReceive(&hspi2, dataTx, dataRx, size, HAL_MAX_DELAY);
}

/**
  * @brief  Envía un bloque de datos por SPI.
  * @param  dataTx: Puntero al buffer de datos a transmitir.
  * @param  size: Cantidad de bytes a enviar.
  * @retval Código de error de tipo sd_err_t
  */
sd_err_t sd_Transmit(const uint8_t *dataTx, uint16_t size)
{
	return (sd_err_t)HAL_SPI_Transmit(&hspi2, dataTx, size, HAL_MAX_DELAY);
}

/**
  * @brief  Recibe un bloque de datos por SPI.
  * @param  dataRx: Puntero al buffer donde se almacenarán los datos recibidos.
  * @param  size: Cantidad de bytes a recibir.
  * @retval Código de error de tipo sd_err_t
  */
sd_err_t sd_Receive(uint8_t *dataRx, uint16_t size)
{
	return (sd_err_t)HAL_SPI_Receive(&hspi2, dataRx, size, HAL_MAX_DELAY);
}
