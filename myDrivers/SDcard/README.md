# SD SPI Driver para STM32

Driver en lenguaje C para operar tarjetas **SD** en modo **SPI**, utilizando microcontroladores **STM32** con la biblioteca **HAL (Hardware Abstraction Layer)**.
Está orientado a acceso **de bajo nivel** (raw) a bloques de 512 bytes, sin sistema de archivos, ideal para sistemas embebidos con recursos limitados.
El driver soporta operaciones básicas:
  - **SD_init:** Inicializa la SD card en modo SPI.
  - **SD_read:** Lee un bloque de 512 bytes.
  - **SD_write:** Escribe un bloque de 512 bytes.
  - **SD_erase:** Borra un bloque de 512 bytes.

---

## Características

- Compatible con tarjetas SD, SDHC y SDXC.
- Comunicación en modo SPI.
- Basado en comandos estándar.
- Acceso a bloques de 512 bytes.

---

## Requisitos

- STM32 con periférico SPI2 configurado.
- HAL SPI y HAL GPIO activados
- Tarjeta SD conectada a través de interfaz SPI:
	- **VCC:** 3v3
	- **CS:** PB12
	- **MISO:** PB14
	- **MOSI:** PB15
	- **SCK:** PB13
	
---

## Estructura del driver

| Archivo           | Descripción                                      |
|-------------------|--------------------------------------------------|
| `sd_card.c`       | Implementación del driver                        |
| `sd_card.h`       | Prototipos y definiciones del driver             |
| `port.c`          | Funciones de acceso a bajo nivel                 |

---

### Inicialización

SD_init();
