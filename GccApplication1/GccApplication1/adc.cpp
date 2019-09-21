#define F_CPU 16000000U
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>

// include the library code:
#include "Arduino.h"

int adc_init(adc_cfg *cfg)
{
	
}

ISR (ADC_vect)
{
	
}