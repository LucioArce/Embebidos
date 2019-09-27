#include <stdint.h>
#include <avr/interrupt.h>

#include "fnqueue.h"
static uint8_t iniciado=0;
static volatile uint16_t timerOvf = 0;
static uint16_t volatile contador = 0; //contador


void (* procesarTimer)(void);
void timer_init(void (* callback)(void))
{
	// El timer se setea la primera vez que se llama a la funcion.
	// Pero se puede reemplazar la funcion de callback con llamadas sucesivas.
	if(iniciado==0)
	{
		//setup del timer2
		cli();
		iniciado = 1;
		OCR2A = 156; // Setear el comparador en 20.
		TCCR2A = 0;// set entire TCCR2A register to 0
		TCCR2B = 0;// same for TCCR2B
		TCCR2A |= (1 << WGM21);  // ctc for timer 0
		TIMSK2 |= (1<<OCIE2A); //setea la interrupcion por comparacion
		TCNT2 = 0; //set timer en 0
		TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);   // preescaler = 8
		sei();
	} else
	{
		
	}
	procesarTimer = callback;
}

ISR(TIMER2_COMPA_vect) // TIMER2_OVF_vect
{

	TCNT2 = 0;
	fnqueue_add(procesarTimer);	//Encolar la funcion de callback cada 10 ms.
	
}


