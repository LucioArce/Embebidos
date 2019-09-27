#include <stdint.h> 
#include <avr/interrupt.h>
#include "teclado.h"
#include "fnqueue.h"
#include "adc.h"

static uint8_t NUM_KEYS = 5;
static volatile uint16_t analogVal;
static uint16_t adc_key_val[5] ={50, 230, 360, 535, 760 };
static int16_t oldkey=-1;

static adc_cfg config;

struct callbackSTR teclasCallback[5];

void key_up_callback(void (*handler)(),int tecla){
	if(tecla>=0 && tecla<5)
		teclasCallback[tecla].callbackUp=handler;
}
	
void key_down_callback(void (*handler)(),int tecla){
	if(tecla>=0 && tecla<5)
		teclasCallback[tecla].callbackDown=handler;
}
	
void procesar_adc_teclado(uint16_t analogVal)
{
	int16_t k;
	for (k = 0; k < NUM_KEYS; k++)
		if (analogVal < adc_key_val[k])
		{
			if(oldkey!=k)
			{	
				oldkey=k;
				teclasCallback[k].callbackDown();
			}
			break;
		}
	if (k >= NUM_KEYS)		//solto el boton
		if(oldkey!= -1)
		{
			teclasCallback[oldkey].callbackUp();
			oldkey = -1;     // Solto el boton y cambio el valor de oldkey 
		};
}



void teclado_setup()
{
	config.channel=0;
	config.value=0;
	config.callback=procesar_adc_teclado;
	adc_init(&config);
	
	//...
}