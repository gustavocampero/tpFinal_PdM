# Driver SHT30 para STM32

Driver en lenguaje C para operar el sensor digital de temperatura y humedad **SHT30** de Sensirion, utilizando microcontroladores **STM32** con la biblioteca **HAL (Hardware Abstraction Layer)**.
Incluye soporte para modos de adquisición **single shot** y **periódica**.

---

## Características

- Soporte para modos de medición:
  - **Single Shot** (sin o con clock stretching)
  - **Medición Periódica** (frecuencias configurables)
- Conversión de datos raw a unidades físicas:
  - Temperatura en °C
  - Humedad relativa en %
- Validación CRC-8 de datos recibidos.

---

## Requisitos

- STM32 con periférico I2C1 configurado.
- HAL I2C y HAL GPIO habilitados
- Sensor SHT30 conectado mediante I2C:
	- **VCC:** 3v3
	- **SDA:** PB9
	- **SCL:** PB8
	
---

## Estructura del driver

| Archivo           | Descripción                                      |
|-------------------|--------------------------------------------------|
| `sht30.c`         | Implementación del driver                        |
| `sht30.h`         | Prototipos y definiciones del driver             |
| `port.c`          | Funciones de acceso a bajo nivel                 |

---

### Inicialización

SHT30_init(bool clock_stretching, sht30_repeatability_t repeatability);
