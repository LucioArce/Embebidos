#include <stdint.h>
#include <avr/interrupt.h>
#include "sensor.h"
#include "fnqueue.h"
#include "adc.h"

static uint8_t contador;
static uint16_t medicion=0;
static volatile uint16_t analogVal;

static adc_cfg config;
static callbackSensor sensorCallback;

void medicion_callback(void (*handler)(int))
{
	sensorCallback.callback=handler;
}

void procesar_adc_sensor()
{
	analogVal=config.valor;
	medicion=analogVal * 1.1 * 100 / 1024;			// formula de conversion de temp del lm35
	sensorCallback.callback(medicion);
}

void sensor_callback(int valor)
{
	contador++;
	if(contador > 50)
	{
		fnqueue_add(procesar_adc_sensor);
		contador=0;
	}
}

void sensor_setup()
{
	config.canal=1;
	config.valor=0;
	config.callback=sensor_callback;
	adc_init(&config);
	//...
}
