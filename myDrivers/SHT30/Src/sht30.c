/*
 *	@file sht30.c
 *	@version 1.0
 *	@date 19/4/2025
 *  @author Ing. Gustavo Campero
 *  @title API de manejo de SHT30
 *  @brief funciones principales para manejar un sensor SHT30
 */


#include "sht30.h"

static uint16_t SINGLE_SHOT_CMD = 0x2C06;
static uint32_t SINGLE_SHOT_MEASUREMENT_DELAY_MS = 15;

static void build_SingleShotCommand(bool clock_stretching, sht30_repeatability_t repeatability);
static uint16_t get_PeriodicDataAcquisitionCommand(sht30_repeatability_t repeatability, sht30_mps_t mps);
static uint8_t SHT30_CRC8(const uint8_t *data, uint8_t len);

/**
  * @brief  Inicializa el sensor SHT30.
  * @note 	Configura el sensor en modo de medición por disparo único (single shot) y realiza un soft reset.
  * @param  clock_stretching: Indica si se habilita o no el clock stretching.
  * @param  repeatability: Nivel de repetibilidad de la medición (baja, media o alta).
  * @retval sht30_err_t (SHT30_OK si la operación fue exitosa)
  */
sht30_err_t SHT30_init(bool clock_stretching, sht30_repeatability_t repeatability)
{
	SHT30_config(clock_stretching, repeatability);
	return SHT30_softReset();
}

/**
  * @brief  Configura el comando de medición en modo single shot para el SHT30.
  * @param  clock_stretching: Si es true, se habilita el clock stretching durante la conversión.
  * @param  repeatability: Nivel de repetibilidad de la medición (baja, media o alta).
  */
void SHT30_config(bool clock_stretching, sht30_repeatability_t repeatability)
{
	build_SingleShotCommand(clock_stretching, repeatability);
}

/**
  * @brief  Reinicio por software del sensor SHT30.
  * @retval sht30_err_t (SHT30_OK si la operación fue exitosa)
  */
sht30_err_t SHT30_softReset(void)
{
    return sht30_write_command(0x30A2); // comando para soft reset
}

/**
  * @brief  Medición de temperatura y humedad con el sensor SHT30.
  * @param  temperature: Puntero donde se almacenará la temperatura medida (en °C).
  * @param  humidity: Puntero donde se almacenará la humedad relativa medida (en %HR).
  * @retval SHT30_OK si la medición fue exitosa y los CRCs son válidos.
  * 		SHT30_CRC_FAIL si alguna validación CRC falla.
  * 		Otro código de error de tipo sht30_err_t si hubo fallo de comunicación.
  */
sht30_err_t SHT30_readTemperatureAndHumidity(float *temperature, float *humidity)
{
	sht30_err_t err;
	err = sht30_write_command(SINGLE_SHOT_CMD);
	if(err != SHT30_OK) return err;

	sht30_delay(SINGLE_SHOT_MEASUREMENT_DELAY_MS);

	uint8_t data[6];
	err = sht30_read(data, 6);
	if (err != SHT30_OK) return err;

	if (SHT30_CRC8(data, 2) != data[2] || SHT30_CRC8(&data[3], 2) != data[5]) return SHT30_CRC_FAIL;

	uint16_t raw_temp = (data[0] << 8) | data[1];
	uint16_t raw_hum  = (data[3] << 8) | data[4];

	*temperature = -45 + 175 * ((float)raw_temp / 65535.0f);
	*humidity = 100 * ((float)raw_hum / 65535.0f);

	return SHT30_OK;
}

/**
  * @brief  Inicia el modo de medición periódica del sensor SHT30.
  * @note	Una vez iniciado se lee con SHT30_periodicRead y se sale del modo con SHT30_stopPeriodicRead
  * @retval sht30_err_t (SHT30_OK si la operación fue exitosa)
  */
sht30_err_t SHT30_startPeriodicRead(sht30_repeatability_t repeatability, sht30_mps_t mps)
{
	SHT30_stopPeriodicRead();

	uint16_t cmd = get_PeriodicDataAcquisitionCommand(repeatability, mps);
	return sht30_write_command(cmd);
}

/**
  * @brief  Detiene las mediciones periódicas del sensor SHT30.
  * @retval sht30_err_t (SHT30_OK si la operación fue exitosa)
  */
sht30_err_t SHT30_stopPeriodicRead(void)
{
    return sht30_write_command(0x3093);
}

/**
  * @brief  Función para obtener la última medición del sensor SHT30 en el modo de medición periódica.
  * @param  temperature: Puntero donde se almacenará la temperatura medida (en °C).
  * @param  humidity: Puntero donde se almacenará la humedad relativa medida (en %HR).
  * @retval SHT30_OK si la medición fue exitosa y los CRCs son válidos.
  * 		SHT30_CRC_FAIL si alguna validación CRC falla.
  * 		Otro código de error de tipo sht30_err_t si hubo fallo de comunicación.
  */
sht30_err_t SHT30_periodicRead(float *temperature, float *humidity)
{
	sht30_err_t err;

    err = sht30_write_command(0xE000);
	if(err != SHT30_OK) return err;

    uint8_t data[6];
    err = sht30_read(data, 6);
    if (err != SHT30_OK) return err;

    if (SHT30_CRC8(data, 2) != data[2] || SHT30_CRC8(&data[3], 2) != data[5]) return SHT30_CRC_FAIL;

    uint16_t raw_temp = (data[0] << 8) | data[1];
    uint16_t raw_hum  = (data[3] << 8) | data[4];

    *temperature = -45 + 175 * ((float)raw_temp / 65535.0f);
    *humidity = 100 * ((float)raw_hum / 65535.0f);

    return SHT30_OK;
}

/**
  * @brief  Función para construir el comando de medición para modo "single shot".
  * @param  clock_stretching: Si es true, se habilita clock stretching (SCL bloqueado durante conversión).
  * @param  repeatability: Nivel de precisión/repetibilidad deseado (alta, media o baja).
  */
static void build_SingleShotCommand(bool clock_stretching, sht30_repeatability_t repeatability)
{
    if (clock_stretching) {
        switch (repeatability) {
            case SHT30_REPEATABILITY_HIGH:
            	SINGLE_SHOT_MEASUREMENT_DELAY_MS = 15;
            	SINGLE_SHOT_CMD = 0x2C06;
            	return;

            case SHT30_REPEATABILITY_MEDIUM:
            	SINGLE_SHOT_MEASUREMENT_DELAY_MS = 6;
            	SINGLE_SHOT_CMD = 0x2C0D;
            	return;

            case SHT30_REPEATABILITY_LOW:
            	SINGLE_SHOT_MEASUREMENT_DELAY_MS = 4;
            	SINGLE_SHOT_CMD = 0x2C10;
            	return;
        }
    } else {
        switch (repeatability) {
            case SHT30_REPEATABILITY_HIGH:
            	SINGLE_SHOT_MEASUREMENT_DELAY_MS = 15;
            	SINGLE_SHOT_CMD = 0x2400;
            	return;

            case SHT30_REPEATABILITY_MEDIUM:
            	SINGLE_SHOT_MEASUREMENT_DELAY_MS = 6;
            	SINGLE_SHOT_CMD = 0x240B;
            	return;

            case SHT30_REPEATABILITY_LOW:
            	SINGLE_SHOT_MEASUREMENT_DELAY_MS = 4;
            	SINGLE_SHOT_CMD = 0x2416;
            	return;
        }
    }

    SINGLE_SHOT_MEASUREMENT_DELAY_MS = 15;
	SINGLE_SHOT_CMD = 0x2C06;
    return; // Default fallback
}

/**
  * @brief  Función para construir el comando para iniciar una medición periódica en el SHT30.
  * @param  repeatability: Nivel de repetibilidad deseado (baja, media o alta).
  * @param  mps: Frecuencia de medición en muestras por segundo (mps).
  * @retval uint16_t: Comando.
  */
static uint16_t get_PeriodicDataAcquisitionCommand(sht30_repeatability_t repeatability, sht30_mps_t mps)
{
	switch(mps)
	{
		case SHT30_MPS_05:
			if(repeatability == SHT30_REPEATABILITY_HIGH) return 0x2032;
			else if(repeatability == SHT30_REPEATABILITY_MEDIUM) return 0x2024;
			else if(repeatability == SHT30_REPEATABILITY_LOW) return 0x202F;

		case SHT30_MPS_1:
			if(repeatability == SHT30_REPEATABILITY_HIGH) return 0x2130;
			else if(repeatability == SHT30_REPEATABILITY_MEDIUM) return 0x2126;
			else if(repeatability == SHT30_REPEATABILITY_LOW) return 0x212D;

		case SHT30_MPS_2:
			if(repeatability == SHT30_REPEATABILITY_HIGH) return 0x2236;
			else if(repeatability == SHT30_REPEATABILITY_MEDIUM) return 0x2220;
			else if(repeatability == SHT30_REPEATABILITY_LOW) return 0x222B;

		case SHT30_MPS_4:
			if(repeatability == SHT30_REPEATABILITY_HIGH) return 0x2334;
			else if(repeatability == SHT30_REPEATABILITY_MEDIUM) return 0x2322;
			else if(repeatability == SHT30_REPEATABILITY_LOW) return 0x2329;

		case SHT30_MPS_10:
			if(repeatability == SHT30_REPEATABILITY_HIGH) return 0x2737;
			else if(repeatability == SHT30_REPEATABILITY_MEDIUM) return 0x2721;
			else if(repeatability == SHT30_REPEATABILITY_LOW) return 0x272A;
	}

	return 0x2032;
}

/**
  * @brief  Calcula el CRC-8 para verificación de integridad de datos del sensor SHT30.
  * @param  data: Puntero al arreglo de bytes sobre el que se calcula el CRC.
  * @param  len: Cantidad de bytes a evaluar (usualmente 2).
  * @note	El algoritmo implementado es el definido por Sensirion: 0x31 (x^8 + x^5 + x^4 + 1)
  * @retval uint8_t: Valor de CRC calculado.
  */
static uint8_t SHT30_CRC8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++)
            crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
    }
    return crc;
}
