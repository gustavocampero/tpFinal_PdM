/*
 * API_delay.c
 *
 *  Created on: Mar 20, 2025
 *      Author: Gus
 */
#include "API_delay.h"

/**
  * @brief Inicializa la estructura de delay.
  *
  * Configura el retardo inicializando la estructura delay_t con el tiempo actual,
  * asigna la duración especificada y establece el estado del delay como inactivo.
  *
  * @param delay Puntero a la estructura delay_t que se va a inicializar.
  * @param duration Valor en ms que representa la duración del delay.
  *
  * @retval void
  */
void delayInit( delay_t * delay, tick_t duration )
{
	assert(delay);
	delay->duration = duration;
	delay->running = false;
}

/**
  * @brief Verifica y actualiza el estado del delay.
  *
  * Esta función evalúa si ha transcurrido el tiempo especificado en la estructura de delay.
  * Si el delay no estaba en ejecución, se inicia estableciendo el tiempo de inicio actual.
  * Si ya estaba en ejecución, compara el tiempo transcurrido con la duración configurada.
  * Retorna verdadero si el delay ha completado su duración y lo detiene, o falso si aún está en curso.
  *
  * @param delay Puntero a la estructura delay_t que contiene la configuración del delay.
  *
  * @retval bool_t Verdadero si se cumplió la duración del delay, falso en caso contrario.
  */
bool_t delayRead( delay_t * delay )
{
	assert(delay);
	bool_t ret = false;
	uint32_t currentTime = HAL_GetTick();
	if(delay->running == false)
	{
		delay->startTime = currentTime;
		delay->running = true;
	}
	else
	{
		ret = ((currentTime - delay->startTime) >= delay->duration);
		delay->running = !ret; // ret = false -> still running -> running = true :: ret = true -> completed -> running = false
	}

	return ret;
}

/**
  * @brief Modifica la duración del delay.
  *
  * Actualiza la duración del delay en la estructura delay_t con el valor especificado.
  * Esta función permite cambiar el tiempo de espera sin afectar otros parámetros del delay.
  *
  * @param delay Puntero a la estructura delay_t que contiene la configuración del delay.
  * @param duration Nueva duración en ticks para el delay.
  *
  * @retval void
  */
void delayWrite( delay_t * delay, tick_t duration )
{
	assert(delay);
	delay->duration = duration;
}

/**
  * @brief Detiene el delay.
  *
  * Esta función detiene el delay configurando su duración a 0 y marcándolo como no activo.
  *
  * @param delay Puntero a la estructura delay_t que contiene la configuración del delay.
  *
  * @retval void
  */
void delayStop( delay_t * delay )
{
	assert(delay);
	delay->duration = 0;
	delay->running = false;
}

/**
 * @brief Comprueba si el delay se encuentra en ejecución.
 *
 * Verifica el estado "running" del objeto delay para determinar si está activo.
 *
 * @param delay Puntero a la estructura delay_t que contiene la información del retardo.
 * @return bool_t Valor booleano: true si el delay está en ejecución, false en caso contrario.
 */
bool_t delayIsRunning(delay_t * delay)
{
	assert(delay);
	return delay->running;
}
