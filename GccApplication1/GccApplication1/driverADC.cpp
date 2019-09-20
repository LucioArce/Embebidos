#define F_CPU 16000000U
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>

// include the library code:
#include "Arduino.h"

struct adc_cfg
{
	uint8_t canal;
	void (callback*)(int);
	uint8_t valor;
	uint8_t idle;
		
};
int adc_init(adc_cfg *cfg)
{
	
}