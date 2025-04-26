
/*
 *	@file sd_card.h
 *	@version 1.0
 *	@date 19/4/2025
 *  @author Ing. Gustavo Campero
 *  @title API de manejo de SD Card (.h)
 *  @brief Definicion de funciones del driver de SD Card
 */

#ifndef SDCARD_INC_SD_CARD_H_
#define SDCARD_INC_SD_CARD_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define SD_BLOCK_SIZE 512

#define CMD0  	0x40
#define CMD8  	0x48
#define CMD17 	0x51
#define CMD24 	0x58
#define CMD55 	0x77
#define ACMD41 	0x69

typedef enum
{
  SD_OK       = 0x00U,
  SD_ERROR    = 0x01U,
  SD_BUSY     = 0x02U,
  SD_TIMEOUT  = 0x03U
} sd_err_t;

// port.c
void cs_Low(void);
void cs_High(void);
void sd_Delay(uint32_t ms);
sd_err_t sd_Transmit(const uint8_t *dataTx, uint16_t size);
sd_err_t sd_TransmitReceive(const uint8_t *dataTx, uint8_t *dataRx, uint16_t size);
sd_err_t sd_Receive(uint8_t *dataRx, uint16_t size);

// sd_card.c
sd_err_t SD_init();
sd_err_t SD_read(uint32_t block_addr, uint8_t *buffer);
sd_err_t SD_write(uint32_t block_addr, const uint8_t *buffer);
sd_err_t SD_erase(uint32_t block_addr);

#endif /* SDCARD_INC_SD_CARD_H_ */
