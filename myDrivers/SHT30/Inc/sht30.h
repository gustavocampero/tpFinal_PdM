/*
 *	@file sht30.h
 *	@version 1.0
 *	@date 19/4/2025
 *  @author Ing. Gustavo Campero
 *  @title API de manejo de SHT30 (.h)
 *  @brief Definicion de funciones del driver de SHT30
 */

#ifndef SHT30_INC_SHT30_H_
#define SHT30_INC_SHT30_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
  SHT30_OK       = 0x00U,
  SHT30_ERROR    = 0x01U,
  SHT30_BUSY     = 0x02U,
  SHT30_TIMEOUT  = 0x03U,

  SHT30_CRC_FAIL = 0x04U
} sht30_err_t;

typedef enum{
	SHT30_REPEATABILITY_HIGH,
	SHT30_REPEATABILITY_MEDIUM,
	SHT30_REPEATABILITY_LOW
} sht30_repeatability_t;

typedef enum{
	SHT30_MPS_05,
	SHT30_MPS_1,
	SHT30_MPS_2,
	SHT30_MPS_4,
	SHT30_MPS_10
} sht30_mps_t;

// port.c
sht30_err_t sht30_write_command(uint16_t cmd);
sht30_err_t sht30_read(uint8_t *data, uint16_t len);
void sht30_delay(uint32_t ms);

// sht30.c
sht30_err_t SHT30_init(bool clock_stretching, sht30_repeatability_t repeatability);
void SHT30_config(bool clock_stretching, sht30_repeatability_t repeatability);
sht30_err_t SHT30_softReset(void);
sht30_err_t SHT30_readTemperatureAndHumidity(float *temperature, float *humidity);
sht30_err_t SHT30_startPeriodicRead(sht30_repeatability_t repeatability, sht30_mps_t mps);
sht30_err_t SHT30_stopPeriodicRead(void);
sht30_err_t SHT30_periodicRead(float *temperature, float *humidity);

#endif /* SHT30_INC_SHT30_H_ */
