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
	// Si se ingresa un canal invalido se retorna 0 (error).
	if(cfg->canal>5)
	{
		return 0;
	}
	// Si el adc no fue iniciado se configura:
	if(iniciado==0)
	{
		// clear ADLAR in ADMUX (0x7C) to right-adjust the result
		// ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
		ADMUX &= ~(1<<ADLAR);
		
		// Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
		// proper source (01)
		ADMUX |= (1<<REFS0);
		
		// Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog
		// input
		ADMUX &= ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);
		
		// Set MUX3..0 in ADMUX (0x7C) to read from AD8 (Internal temp)
		// Do not set above 15! You will overrun other parts of ADMUX. A full
		// list of possible inputs is available in Table 24-4 of the ATMega328
		// datasheet
		
		// Ya que es el primero se setea el canal directamente.
		
		//ADMUX |= cfg->canal;
		// ADMUX |= B00001000; // Binary equivalent
		
		// Set ADEN in ADCSRA (0x7A) to enable the ADC.
		// Note, this instruction takes 12 ADC clocks to execute
		ADCSRA |= 1<<ADEN;
		
		// Deshabilitar autotrigger.
		ADCSRA &= ~(1<<ADATE);
		
		// Clear ADTS2..0 in ADCSRB (0x7B) to set trigger mode to free running.
		// This means that as soon as an ADC has finished, the next will be
		// immediately started.
		// Esta vez no queremos AutoTrigger.
		// ADCSRB &= ~(1<<ADTS2 | 1<<ADTS1 | 1<<ADTS0);
		
		// Set the Prescaler to 128 (16000KHz/128 = 125KHz)
		// Above 200KHz 10-bit results are not reliable.
		ADCSRA |= (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS2);
		
		// Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
		// Without this, the internal interrupt will not trigger.
		ADCSRA |= 1<<ADIE;
		
		// Enable global interrupts
		// AVR macro included in <avr/interrupts.h>, which the Arduino IDE
		// supplies by default.
		sei();
		
		// Kick off the first ADC
		
		// Set ADSC in ADCSRA (0x7A) to start the ADC conversion
		ADCSRA |= 1<<ADSC;
		
		//inicializar el arreglo con 0s.
		for(i=0;i<6;i++)
		{
			seteados[i]=0;
		}
		
		iniciado=1;
	}
	// Si el canal esta en uso retorna con 0 (error)
	if (seteados[cfg->canal] == 0)
	{
		return 0;
	}
	seteados[cfg->canal] = 1;
	canales[cfg->canal] = * cfg;
	return 1;
}

void llamarCallback()
{
	canales[fnLlamar].callback(valor_leido);
}

ISR (ADC_vect)
{
	if(contador>=100)
	{
		valor_leido = ADC;
		fnLlamar = canalConvirtiendo;
		fnqueue_add(llamarCallback);
		for(i=1;i<=6;i++)
		{
			if(seteados[(canalConvirtiendo+i)&6]==1)
			{
				canalConvirtiendo=(canalConvirtiendo+i)%6;
				break;
			}
		}
		//ADMUX |= canalConvirtiendo;
		contador = 0;
	} else
	{
		contador++;

	}

	//Llamar a la proxima convercion AC.
	ADCSRA |= 1<<ADSC;

}