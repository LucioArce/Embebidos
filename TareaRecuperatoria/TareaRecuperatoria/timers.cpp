#include "timer.h"
#include <stdint.h>
#include "fnqueue.h"
struct timer_t
{
	uint16_t periodo;
	void (* callback)(void);
	uint8_t posicion;
	uint16_t contador;				//Almacena cuantas veces pasaron 10ms.
};

uint8_t inicio=0;
uint8_t libre=0;
timer_t timers[16];
uint8_t seteados[16];
static uint8_t i;

void procesarTimers()
{
	for (i=0;i<16;i++)
	{
		if (seteados[i]==1)
		{
			timers[i].contador=((timers[i].contador)+1)%timers[i].periodo;
			if (timers[i].contador==0)
			{
				fnqueue_add(timers[i].callback);
			}
		}
	}
}

timer_t *timers_new(void (* timeout_signal)(void))
{
	if (libre<16)
	{
		timers[libre].callback=timeout_signal;
		timers[libre].posicion=libre;
		libre++;
	}
	return &timers[libre-1];
}


bool timers_start(timer_t* timer, uint16_t timeout_cs)
{
	if(inicio==0)
	{
		timer_init(procesarTimers);
		inicio=1;
	}
	seteados[timer->posicion]=1;
	timer->periodo=timeout_cs;
	timer->contador=0;
	return true;
}

bool timers_stop(timer_t* timer)
{
	seteados[timer->posicion]=0;
	return true;
}