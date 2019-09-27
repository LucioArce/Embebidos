// struct callbackSensor
// {
// 	void (*callback)(int);
// };

//void medicion_callback(void (*handler)(int));
int sensor_setup(void (* callback_sensor)(int));
