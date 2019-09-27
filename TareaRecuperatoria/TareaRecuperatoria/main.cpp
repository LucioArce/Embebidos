/*
 * TareaRecuperatoria.cpp
 *
 * Created: 25/9/2019 8:59:47 a. m.
 * Author : Julian
 */ 

#include <avr/io.h>
#include "timers.h"
#include "fnqueue.h"
#include "Arduino.h"

void toogle1()
{
	PORTB ^= (1<<DDB1);
}

void toogle2()
{
	PORTB ^= (1<<DDB2);
}

void toogle3()
{
	PORTB ^= (1<<DDB3);	
}

void setup()
{
	 /* Setear los pines B1, B2 y B3 como salidas */
	DDRB |= (1<<DDB1);			
	DDRB |= (1<<DDB2);
	DDRB |= (1<<DDB3);

	
	timers_start(timers_new(toogle1),25);	
	timers_start(timers_new(toogle2),50);
	timers_start(timers_new(toogle3),100);
    }

void loop()
{
	fnqueue_run();
}

