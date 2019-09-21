struct callbackSensor
{
	void (*callback)(int);
};

void medicion_callback(void (*handler)(int));
void sensor_setup();