#include "sensor.h"
#include <stdint.h>
#include <avr/interrupt.h>
#include "fnqueue.h"

static uint16_t medicion=0;
static volatile analogVal;


static volatile uint16_t analogVal;


static adc_cfg config;


void sensor_callback()
{
		
}

void procesar_adc_sensor()
{
	analogVal=config.value;
	medicion=analogVal * 1.1 * 100 / 1024;			// formula de conversion de temp del lm35
	//como la mando al main???
}

void teclado_callback()
{
	contador++;
	if(contador > 50)
	{
		fnqueue_add(procesar_adc_sensor);
		contador=0;
	}
}

void teclado_setup()
{
	config.canal=0;
	config.valor=0;
	config.callback=teclado_callback;
	
	//...
}
