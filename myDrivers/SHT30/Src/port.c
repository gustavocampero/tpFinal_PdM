/*
 *	@file port.c
 *	@version 1.0
 *	@date 19/4/2025
 *  @author Ing. Gustavo Campero
 *  @title API de manejo de SHT30 (bajo nivel)
 *  @brief funciones port.c de bajo nivel
 */

#include "sht30.h"
#include "stm32f4xx_hal.h"

#define SHT30_I2C_ADDR (0x44 << 1)

extern I2C_HandleTypeDef hi2c1;

/**
  * @brief  Envia un comando al sensor SHT30 por I2C.
  * @param  data Puntero al búfer donde se almacenarán los datos leídos.
  * @param  len Cantidad de bytes a leer.
  * @retval Código de error de tipo sht30_err_t
  */
sht30_err_t sht30_write_command(uint16_t cmd)
{
	uint8_t data[2] = { cmd >> 8, cmd & 0xFF }; // descompone en 2 bytes
	return (sht30_err_t)HAL_I2C_Master_Transmit(&hi2c1, SHT30_I2C_ADDR, data, 2, HAL_MAX_DELAY);
}

/**
  * @brief  Lee datos desde el sensor SHT30 por I2C.
  * @param  cmd: Comando de 16 bits a enviar al sensor.
  * @retval Código de error de tipo sht30_err_t
  */
sht30_err_t sht30_read(uint8_t *data, uint16_t len)
{
    return (sht30_err_t)HAL_I2C_Master_Receive(&hi2c1, SHT30_I2C_ADDR, data, len, HAL_MAX_DELAY);
}

/**
  * @brief  Ejecuta delay en milisegundos.
  * @param  ms: Milisegundos.
  */
void sht30_delay(uint32_t ms)
{
    HAL_Delay(ms);
}
