#define TECLA0 1 //botón up del LCD Keypad Shield
#define TECLA1  2 //botón down del LCD Keypad Shield
#define TECLA2  3 //botón left del LCD Keypad Shield
#define TECLA3  0 //botón right del LCD Keypad Shield
#define TECLA4  4 //boton select

struct callbackSTR{

	void (*callbackUp) ();
	void (*callbackDown) ();	

};

void key_down_callback(void (*handler)(),int tecla);
void key_up_callback(void (*handler)(), int tecla);
void teclado_setup();
void teclado_loop();