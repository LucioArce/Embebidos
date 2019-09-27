#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#define MAX_CHANNELS 6 // son 6 (A0:A5) el A0 es para el teclado y el A1 para el sensor

struct adc_cfg
{
	int channel;
	void (*callback)(uint16_t);
	uint16_t value;
	int active;
	int finish_convertion;
};

int adc_init(adc_cfg *cfg);

#endif /* ADC_DRIVER_H_ */