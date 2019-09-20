
#define F_CPU 16000000U
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

// include the library code:
#include "Arduino.h"
#include "LiquidCrystal.h"
#include "device.h"
#include "fnqueue.h"

const uint8_t numRows = 2;
const uint8_t numCols = 16;

static int8_t volatile modo;
static volatile uint16_t timerOvf = 0;
static uint16_t volatile contador = 0; //contador

#define COEFBRILLO 51  // 51 es el 20% de 255
static uint8_t brilloActual = 4; // el brillo inicial es del 80% 0<=brilloActal<=5

#define MAX_TIEMPOS 10
static uint16_t tiempos[MAX_TIEMPOS];
static uint8_t tiempoActual = 0; //tiempo del arreglo de tiempos para escribir
static int8_t posEnTiempos = 0; //indice actual del arreglo de tiempos para mostrar

static uint16_t contadorPausado = 0; //contador para mostrar en MP
static uint16_t contadorAux = 0;
static int16_t contadorPosta = 0;
static int8_t contadorMAD3seg1 = 0;
static int8_t contadorMAD3seg2 = 0;
static int8_t contador5seg = 0;

bool sentido=true;

int contScroll=0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


//MODOS:
#define START 0
#define MCA   1
#define MP    2
#define MVT   3
#define MAD   4


/************************************************************************/
/*                          AUXILIARES                                  */
/************************************************************************/
void iniciarTimerMAD()
{
	contadorMAD3seg1=contador;
}

void iniciarTimerMAD2()
{
	contadorMAD3seg2=contador;
}

void iniciarTimerMAD5()
{
	contador5seg=contador;
}

void saveSplit()
{
	if(tiempoActual == 9)
	tiempoActual=-1;

	tiempoActual++;
	tiempos[tiempoActual]=contadorPosta;
}

void brightnessUp()
{
	if(brilloActual<5)
	{
		brilloActual++;
		analogWrite(10,brilloActual*COEFBRILLO);
	}
}
void brightnessDown()
{
	if(brilloActual>1)
	{
		brilloActual--;
		analogWrite(10,brilloActual*COEFBRILLO);
	}
}

int showNextTime()
{
	if(posEnTiempos>8)
	posEnTiempos=-1;

	posEnTiempos++;
	return tiempos[posEnTiempos];
}

int showPrevTime()
{
	if(posEnTiempos<1)
	posEnTiempos=10;

	posEnTiempos--;
	return tiempos[posEnTiempos];
}

void resetTimer()
{
	contadorPosta=0;
	contadorPausado=0;
	contador=0;
}

void pauseTimer()
{
	contadorPausado=contador;
	contadorPosta=contadorPausado;
}

void continueTimer()
{
	contador=contadorPausado;
}

void imprimirModo(int8_t m)
{
	lcd.setCursor(13,0);
	switch (m)
	{
		case MCA:
		{
			lcd.print("MCA");
		}
		break;
		case MP:
		{
			lcd.print("MP ");
		}
		break;
		case MVT:
		{
			lcd.print("MVT");
		}
		break;
		case MAD:
		{
			lcd.print("MAD");
		}
		break;
	}
}

/************************************************************************/
/*                           CALLBACKS                                  */
/************************************************************************/
void down_keyUp()
{
	
}

void down_keyDown()
{
	
}

void up_keyLeft()
{
	
}

void down_keyLeft()
{
	
}

void up_keyRight()
{
	
}

void down_keyRight()
{
	
}

void down_keySelect()
{
	if (modo==MP)
	iniciarTimerMAD();
}

void up_keyUp()
{
	switch (modo)
	{
		case START:
		{
			modo = MP;
		}break;
		case MCA:
		{
			pauseTimer();
			modo = MP;
		}break;
		case MP:
		{
			continueTimer();
			modo = MCA;
		}break;
		case MVT:
		{
			contadorPosta = showNextTime();
		}break;
		case MAD:
		{
			iniciarTimerMAD5();
			if(contadorAux != contadorPausado)
			contadorAux = contadorPausado;
			brightnessUp();
		}break;
	}
}

void up_keyDown()
{
	switch (modo)
	{
		case START:
		{
			modo = MP;
		}break;
		case MCA:
		{
			saveSplit();
		}break;
		case MP:
		{
			saveSplit();
			resetTimer();
		}break;
		case MVT:
		{
			contadorPosta = showPrevTime();
		}break;
		case MAD:
		{
			iniciarTimerMAD5();
			brightnessDown();
		}break;
	}
}

void up_keySelect()
{
	switch (modo)
	{
		case START:
		{
			modo = MP;
		}break;
		case MP:
		{
			iniciarTimerMAD2();
			// Si la cantidad de segundos que pasaron es menor a 3
			//  -> me voy a MVT
			//  sino me voy a MAD
			if((contadorMAD3seg2-contadorMAD3seg1) < 3)
			{
				modo = MVT;
			}
			else
			{
				modo = MAD;
				iniciarTimerMAD5();
			}
		}break;
		case MVT:
		{
			modo = MP;
		}break;
		case MAD:
		{
			modo = MP;
		}break;
	}
}



void setup()
{
	teclado_setup();
	fnqueue_init();
	
	// Define los callbacks de cada Tecla
	key_up_callback( up_keyUp, TECLA0);
	key_down_callback( down_keyUp, TECLA0);
	key_up_callback( up_keyDown, TECLA1);
	key_down_callback( down_keyDown, TECLA1);
	key_up_callback( up_keyLeft, TECLA2);
	key_down_callback( down_keyLeft, TECLA2);
	key_up_callback( up_keyRight, TECLA3);
	key_down_callback( down_keyRight, TECLA3);
	key_up_callback( up_keySelect, TECLA4);
	key_down_callback( down_keySelect, TECLA4);
	
	// Inicializa los tiempos
	for(int i = 0; i<MAX_TIEMPOS; i++)
	tiempos[i] = 0;
	
	// set up the LCD's number of columns and rows:
	lcd.begin(numCols, numRows);
	analogWrite(10, brilloActual * COEFBRILLO); //Controla intensidad backlight
	lcd.setCursor(0, 0);
	lcd.print("Lab 2 Sist. Emb.");
	lcd.setCursor(0, 1);
	lcd.print("2do.C 2019 COM 3");
	
	
	modo = START;
	
	//setup del timer2
	cli();
	TCCR2A = 0;// set entire TCCR2A register to 0
	TCCR2B = 0;// same for TCCR2B
	TCCR2A |= (1 << WGM21);  // ctc for timer 0
	TIMSK2 |= (1<<TOIE2); //setea la interrupcion por overflow
	TCNT2 = 0; //set timer en 0
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);  // CS#2 y CS#0 bits -> Timer0 preescaler = 1024
	sei();
	
}

void loop()
{
	fnqueue_run();
}

void procesarTimer()
{
	if (modo==START)
	{
		if (contScroll==0)
		{
			lcd.setCursor(0,0);
			lcd.print("Sistemas Embebidos 2019");
			lcd.setCursor(0,1);
			lcd.print("2do Lab 2 Cuat. Arce-Rey");
		}
		else if ((contScroll%2)==0)
		{
			if(sentido)
			{
				lcd.scrollDisplayLeft();
			}
			else
			{
				lcd.scrollDisplayRight();
			}
			
		}
		if(contScroll>=16)
		{
			sentido=!sentido;
			contScroll=0;
		}
		contScroll++;
	}
	else
	{
		lcd.clear();
		if(modo==MCA)
		{
			contadorPosta = contador;
		}
		// Vuelve a MP si pasaron 5 segundos y no se apreto nada en MAD
		if ((modo==MAD) && ((contador-contador5seg)>5))
		{
			modo = MP;
		}
		
		lcd.setCursor(0,0);
		lcd.print("                "); // Blanqueo la Linea para no imprimir basura
		imprimirModo(modo);
		lcd.setCursor(0,0);
		lcd.print(contadorPosta);
	}
}

ISR(TIMER2_OVF_vect) // TIMER2_OVF_vect
{
	TCNT2 = 0;
	timerOvf= (timerOvf + 1) % 60;
	// El modo START para contemplar el estado incial con la info de la materia, etc.
	if(timerOvf == 0 ) //como el prescaler esta seteado en 1024, cada 60 interrupciones cuento 1 seg(0.98 seg)
	{
		contador++;
		//timerOvf=0;
		fnqueue_add(procesarTimer);
	}
	
}
