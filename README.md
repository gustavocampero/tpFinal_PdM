# Trabajo practico final Programacion de Microcontroladores

Este proyecto consite en un sistema capaz de realizar mediciones periódicas de temperatura y humedad y mostrar al usuario el promedio de todas las mediciones realizadas en un periodo mayor de tiempo.
Los valores medidos están protegidos de reinicios guardandolos en una memoria externa.

## Descripción

El firmware implementa las siguientes funcionalidades:

- **Configuración del sistema y periféricos:**  
  Inicializa el sistema, configura el reloj del microcontrolador, habilita los pines GPIO y configura el UART para comunicación serial.

- **Funciones de Delay sin Bloqueo:**  
  Se definen funciones para el manejo de delays sin utilizar retardos bloqueantes.

- **Control de LED:**  
  Implementa la gestión del LED (LD2) con una FSM para que parpadee dos veces cuando se realiza una medicion de temperatura y humedad.
  
- **API Debounce:**  
  Implementa la gestión del anti-rebote del botón utilizando una máquina de estados finitos (FSM). Se agrega la capacidad de detectar si el boton fue presionado
  una vez o si se mantuvo presionado durante un tiempo definido
 
- **API Uart:**  
  Configura el UART2 para comunicación serial.
  
- **SHT30:**
  Gestiona la comunicacion con un sensor SHT30 por I2C para medir temperatura y humedad.
  
- **SDCard:**
  Gestiona la comunicacion con una SDCard por SPI para leer y escribir informacion. No se implementa un sistema de archivos, la memoria se utiliza en formato RAW.  
