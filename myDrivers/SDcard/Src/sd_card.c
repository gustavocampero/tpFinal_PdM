/*
 *	@file sd_card.c
 *	@version 1.0
 *	@date 19/4/2025
 *  @author Ing. Gustavo Campero
 *  @title API de manejo de SD Card
 *  @brief funciones para iniciar, leer y escribir en SD Card (raw)
 */

#include "sd_card.h"

static const uint8_t dummy = 0xFF;
static const uint8_t start_token = 0xFE;

static void sd_dummy();
static void send_dummy_clocks();
static void waitBusy();
static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc, bool cs);
static sd_err_t send_CMD0();
static sd_err_t send_CMD8();
static sd_err_t send_ACMD41(bool force);


/* ====================  Funciones principales  ========================= */

/**
  * @brief  Inicializa la SD card en modo SPI.
  * @note   Realiza el procedimiento CMD0 -> CMD8 -> ACMD41.
  * @retval SD_OK: si la inicialización fue exitosa
  * 		SD_ERROR: caso contrario.
  */
sd_err_t SD_init()
{
	send_dummy_clocks();

	if(send_CMD0() != SD_OK) return SD_ERROR;

	sd_dummy();

	if(send_CMD8() != SD_OK) return SD_ERROR;

	sd_dummy();

	send_ACMD41(false);

	sd_dummy();

	return SD_OK;
}

/**
  * @brief  Lee un bloque de datos de la SD card en modo SPI.
  * @note	1- Ejecuta CMD17 (READ_SINGLE_BLOCK). La SD card debe responder con el token de inicio
  * 		de datos (0xFE) para confirmar lectura.
  * 		2- Se recibe un bloque (512 bytes) y 2 bytes de CRC (se descartan)
  * 		3- Se envia dummy para asegurar estabilidad.
  * @param  block_addr: Dirección del bloque a leer
  * @param  buffer: Puntero al buffer de destino.
  * @retval SD_OK: si la lectura fue exitosa.
  * 		SD_ERROR: caso contrario.
  */
sd_err_t SD_read(uint32_t block_addr, uint8_t *buffer)
{
	uint8_t token;
	uint8_t crc;

	sd_dummy();
	sd_dummy();
	sd_dummy();

	if (sd_send_cmd(CMD17, block_addr, 0x01, true) != 0x00)
	{
		return SD_ERROR;
	}

	cs_Low();
	for (int i = 0; i < 10000; i++) {
		sd_TransmitReceive(&dummy, &token, 1);
		if (token == 0xFE) break;
	}

	sd_Receive(buffer, SD_BLOCK_SIZE);
	// discard CRC
	sd_Receive(&crc, 1);
	sd_Receive(&crc, 1);
	cs_High();

	waitBusy();

	sd_dummy();
	sd_dummy();
	sd_dummy();

	return SD_OK;
}

/**
  * @brief  Escribe un bloque de datos en la SD card en modo SPI.
  * @note	1- Envia comando CMD55 + ACMD41 para despertar la SD en caso que haya quedado en estado IDLE.
  *  		2- Ejecuta CMD24 (WRITE_BLOCK). Debe responder 0x00.
  *  		3- Se envía un token de inicio de escritura (0xFE).
  *  		4- Se transmite el bloque de datos y un CRC (puede ser falso).
  *  		5- Se recibe respuesta de aceptación. LSB debe ser 0x05 para indicar éxito.
  *  		6- Se espera que termine la escritura con waitBusy().
  * @param  block_addr: Dirección del bloque donde se escribirá la información.
  * @param  buffer: Puntero al buffer de origen que contiene los 512 bytes a escribir.
  * @retval SD_OK: si la escritura fue aceptada y completada.
  *			SD_ERROR: si la respuesta fue negativa o si el token fue rechazado.
  */
sd_err_t SD_write(uint32_t block_addr, const uint8_t *buffer)
{
	waitBusy();

	send_ACMD41(true);

	sd_dummy();
	sd_dummy();

	cs_Low();
	uint8_t ret = sd_send_cmd(CMD24, block_addr, 0x01, false);
	if (ret != 0x00) {
		cs_High();
		return SD_ERROR;
	}

	sd_Transmit(&start_token, 1);
	sd_Transmit((uint8_t *)buffer, SD_BLOCK_SIZE);
	uint8_t crc[2] = {0xFF, 0xFF};
	sd_Transmit(crc, 2);

	uint8_t resp;
	sd_TransmitReceive(&dummy, &resp, 1);
	if ((resp & 0x1F) != 0x05) {
		cs_High();
		return SD_ERROR;
	}

	cs_High();

	waitBusy();

	return SD_OK;
}


/**
  * @brief  Borra (sobrescribe) un bloque de la SD card con 0xFF.
  * @param  block_addr: Dirección del bloque a sobrescribir.
  * @retval SD_OK: si la operación fue exitosa
  * 		SD_ERROR: caso contrario.
  */
sd_err_t SD_erase(uint32_t block_addr)
{
    uint8_t buffer[SD_BLOCK_SIZE];
    memset(buffer, 0xFF, sizeof(buffer));

    return SD_write(block_addr, buffer);
}

/* =====================  Utilidad y control  =========================== */

/**
  * @brief  Envía un byte dummy (0xFF) por SPI manteniendo CS en alto.
  * @note   Necesario luego de completar un comando. Genera 8 ciclos de clock dummy
  * 		para que la tarjeta SD pueda completar procesos internos. Mantiene la linea
  * 		SPI activa.
  */
static void sd_dummy()
{
	cs_High();
	sd_Transmit(&dummy, 1);
	sd_Delay(1);
}

/**
  * @brief  Envía 80 ciclos de reloj (10 bytes dummy) con CS alto.
  * @note   Requerido por el estándar de inicialización de tarjetas SD en modo SPI.
  * 		Permite salir a la SD de modo reset y prepararse para recibir comandos
  */
static void send_dummy_clocks()
{
	cs_High();
	sd_Delay(10);
	for (int i = 0; i < 10; i++) {
		cs_High();
		sd_Transmit(&dummy, 1);
	}
}

/**
  * @brief  Espera hasta que la SD card se libere.
  * @note   Envía dummy hasta recibir una respuesta distinta de 0x00.
  */
static void waitBusy()
{
	uint8_t resp;
	do {
		cs_High();
		sd_TransmitReceive(&dummy, &resp, 1);
		sd_Delay(1);
	} while (resp == 0x00);
}

/**
  * @brief  Envía un comando a la SD card en modo SPI.
  * @param  cmd: Comando.
  * @param  arg: Argumento de 32 bits del comando.
  * @param  crc: CRC del comando (obligatorio solo para algunos comandos).
  * @retval Respuesta R1 del comando (byte de estado).
  */
static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc, bool cs)
{
    uint8_t buf[6];
    uint8_t resp = 0xFF;

    buf[0] = cmd;
    buf[1] = (arg >> 24) & 0xFF;
    buf[2] = (arg >> 16) & 0xFF;
    buf[3] = (arg >> 8) & 0xFF;
    buf[4] = arg & 0xFF;
    buf[5] = crc;

    cs_Low();

    if(sd_Transmit(buf, 6) == SD_OK) // envia comando
    {
		for (int i = 0; i < 10; i++) {
			sd_TransmitReceive(&dummy, &resp, 1); // espera respuesta
			if ((resp & 0x80) == 0) break;
		}
		if(cs) cs_High();
    }

    return resp;
}

/* ====================  Comandos específicos  ========================== */

/**
  * @brief  Envía comando CMD0 (GO_IDLE_STATE)
  * @note 	Fuerza a la SD card a entrar en estado IDLE. Espera una respuesta
  * 	  	R1 = 0x01 si la SD card entro en modo SPI correctamente. Si no se logra
  * 	  	tras varios intentos la SD card podria estar defectuosa o no conectada.
  * @retval SD_OK: la tarjeta respondió correctamente
  * 		SD_ERROR: caso contrario.
  */
static sd_err_t send_CMD0()
{
	uint8_t tries = 10;

	for (int i = 0; i < tries; i++) {
		uint8_t resp = sd_send_cmd(CMD0, 0, 0x95, true);
		if (resp == 0x01) return SD_OK;
		sd_Delay(10);
	}
	return SD_ERROR;
}

/**
  * @brief  Envía comando CMD8 (SEND_IF_COND).
  * @note   Verifica si la SD card soporta la especificación 2.0 o superior.
  *         Se le envía el patrón 0x1AA como argumento (indicando rango de voltaje y código de eco).
  *         La respuesta incluye una cabecera R1 y un bloque R7 de 4 bytes que contienen:
  *           - Byte 0-1: Reservado.
  *           - Byte 2: Voltaje aceptado (debería incluir 0x01 si soporta 2.7-3.6V).
  *           - Byte 3: Código de eco (0xAA), debe coincidir con el enviado.
  * @retval SD_OK: si se recibe una respuesta válida R1 = 0x01 y el patrón se lee correctamente.
  *         SD_ERROR: caso contrario
  */
static sd_err_t send_CMD8()
{
	uint8_t r7[4];
	uint8_t resp = sd_send_cmd(CMD8, 0x1AA, 0x87, true);
	cs_Low();
	if(resp == 0x01)
	{
		for (int i = 0; i < 4; i++) {
		    uint8_t b = 0xFF;
		    uint16_t timeout = 1000;
		    while (b == 0xFF && timeout--) {
		        sd_TransmitReceive(&dummy, &b, 1);
		    }
		    r7[i] = b;
		}

		cs_High();

		uint16_t echo = (r7[2] << 8) | r7[3];
		if (echo != 0x1AA) return SD_ERROR;

		return SD_OK;
	}
	else
	{
		return SD_ERROR;
	}
}

/**
  * @brief  Envía la secuencia de comandos CMD55 + ACMD41.
  * @note   ACMD41 es el comando de inicialización usado por tarjetas SD versión 2.0 en adelante.
  *         Debe ser precedido por CMD55 (APP_CMD) para indicar que el siguiente comando es una aplicación (ACMD).
  *         Este comando debe repetirse hasta que la respuesta R1 sea 0x00 (indica card lista).
  * @param  force: Si es true, fuerza solo un intento sin bucle de espera (modo "rápido").
  * @retval SD_OK si la tarjeta fue inicializada correctamente, SD_TIMEOUT si se excedieron los intentos.
  */
static sd_err_t send_ACMD41(bool force)
{
	uint8_t resp = 0xFF;
	uint16_t tries = 10000;
	do
	{
		sd_send_cmd(CMD55, 0, 0x01, true); // no es necesario argumento

		resp = sd_send_cmd(ACMD41, 0x40000000, 0x01, true); // 0x40000000 -> bit HCS (Host Capacity Support) indica soporte a SDHC/SDXC.

		sd_dummy();

		tries--;
		if(tries <= 0) {
			return SD_TIMEOUT;
		}

		if(force) break;
	} while (resp != 0x00);

	return SD_OK;
}


