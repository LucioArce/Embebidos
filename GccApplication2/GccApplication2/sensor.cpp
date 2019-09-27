#include <stdint.h>
#include <avr/interrupt.h>
#include "sensor.h"
#include "fnqueue.h"
#include "adc.h"

static volatile uint16_t analogVal=0;

static adc_cfg config;


// static callbackSensor sensorCallback;
void (* callback_sensor) (int);



void procesar_adc_sensor()
{
	
	uint16_t medicion = analogVal * 5 * 100 / 1024;			// formula de conversion de temp del lm35
	callback_sensor(medicion);
}

void sensor_callback(uint16_t valor)
{
	analogVal=valor;
	fnqueue_add(procesar_adc_sensor);
}

int sensor_setup(void (* callback_s)(int))
{
	callback_sensor=callback_s ;
	config.channel=1;
	config.value=0;
	config.callback=sensor_callback;
	adc_init(&config);
	return 1;
	//...
}
