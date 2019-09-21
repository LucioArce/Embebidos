struct adc_cfg
{
	uint8_t canal;
	void (*callback)(int);
	uint8_t valor;
	uint8_t idle;
	
};
int adc_init(adc_cfg *cfg);