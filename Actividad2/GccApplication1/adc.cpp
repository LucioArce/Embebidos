#define F_CPU 16000000U
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "fnqueue.h"

// include the library code:
#include "adc.h"
#include "Arduino.h"

uint8_t iniciado=0;
uint8_t canalConvirtiendo=0;
uint8_t fnLlamar=0;			// Almacena que funcion toca llamar.
uint8_t contador=0;			// Contador para descartar interrupciones.
adc_cfg canales[6];
uint8_t seteados[6];		// Almacena que canales estan en uso.
uint16_t valor_leido;		// Almacena el ultimo valor leido (util).
uint8_t i;

int adc_init(adc_cfg *cfg)
{
	if(cfg->channel>5)
	{
		return 0;
	}
	if(iniciado==0)
	{
		ADMUX &= ~(1<<ADLAR);
		ADMUX |= (1<<REFS0);
		ADMUX &= ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);

		switch(cfg->channel)
		{
			case 0: break;
			case 1:
			{
				ADMUX |= (1<<MUX0);
			}break;
			case 2:
			{
				ADMUX |= (1<<MUX1);
			}break;
			case 3:
			{
				ADMUX |= ( (1<<MUX1) | (1<<MUX0));
			}break;
			case 4:
			{
				ADMUX |= (1<<MUX2);
			}break;
			case 5:
			{
				ADMUX |= ((1<<MUX2)|(1<<MUX0));
			}break;
			case 6:
			{
				ADMUX |= ((1<<MUX2) |(1<<MUX1));
			}break;
		}

		ADCSRA |= 1<<ADEN;
		ADCSRA &= ~(1<<ADATE);
		ADCSRA |= (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS2);
		ADCSRA |= 1<<ADIE;
		sei();
		ADCSRA |= 1<<ADSC;
		for(i=0;i<6;i++)
		{
			seteados[i]=0;
		}

		iniciado=1;
		canalConvirtiendo=cfg->channel;
	}
	if (seteados[cfg->channel] != 0)
	{
		return 0;
	}
	seteados[cfg->channel] = 1;
	canales[cfg->channel] = * cfg;
	return 1;
}

void llamarCallback()
{
	canales[fnLlamar].callback(valor_leido);
}
//uint8_t corte=0;
ISR (ADC_vect)
{
	if(contador>=10)
	{
		valor_leido = ADC;
		fnLlamar = canalConvirtiendo;
		fnqueue_add(llamarCallback);
		for(i=1;i<=6;i++)
		{
			if(seteados[(canalConvirtiendo+i)%6]==1)
			{
				canalConvirtiendo=(canalConvirtiendo+i)%6;
				break;
			}
		}
        ADMUX &= ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);
        switch(canalConvirtiendo)
        {
            case 0: break;
            case 1:
            {
                ADMUX |= (1<<MUX0);
            }break;
            case 2:
            {
                ADMUX |= (1<<MUX1);
            }break;
            case 3:
            {
                ADMUX |= ( (1<<MUX1) | (1<<MUX0));
            }break;
            case 4:
            {
                ADMUX |= (1<<MUX2);
            }break;
            case 5:
            {
                ADMUX |= ((1<<MUX2)|(1<<MUX0));
            }break;
            case 6:
            {
                ADMUX |= ((1<<MUX2) |(1<<MUX1));
            }break;
        }
		contador = 0;
	} else
	{
		contador++;

	}
	ADCSRA |= 1<<ADSC;

}
