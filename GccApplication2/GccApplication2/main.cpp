#define F_CPU 16000000U
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

// include the library code:
#include "Arduino.h"
#include "LiquidCrystal.h"
#include "teclado.h"
#include "fnqueue.h"
#include "sensor.h"


const uint8_t numRows = 2;
const uint8_t numCols = 16;

static uint16_t volatile contador = 0; //contador
static uint16_t temperaturaMedida = 0;


#define MAX_TEMPS 100
static uint16_t temps[MAX_TEMPS];

static int8_t tempActual = 0;
static int8_t tempMin = 200;
static int8_t tempMax = 0;
static int8_t tempPromedio = 0;

static int8_t cantTemps = 0;
static int8_t indiceArreglo = 0;

static uint8_t timerOvf = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


/************************/
/*                          AUXILIARES                                  */
/************************/
void guardarTemp(int8_t temp)
{
	temps[indiceArreglo]=temp;
	indiceArreglo=(indiceArreglo+1)%100;
	if (cantTemps<=100)
	cantTemps++;
}

void updateCurrentTemp()
{
	tempActual=temperaturaMedida;
}
void updateAverageTemp()
{
	int16_t sumaTemps=0;
	tempPromedio=0;
	if(cantTemps>0)
	{
		for(int i = 0; i<cantTemps; i++)
		{
			sumaTemps+=temps[i];
		}
		tempPromedio=sumaTemps/cantTemps;
	}
}
void updateMaxTemp()
{
	for(int i = 0; i<indiceArreglo; i++)
	{
		if (temps[i]>tempMax)
		{
			tempMax=temps[i];
		}
	}
}
void updateMinTemp()
{
	for(int i = 0; i<indiceArreglo; i++)
	{
		if (temps[i]<tempMin)
		{
			tempMin=temps[i];
		}
	}
}
void showCurrentTemp()
{
	lcd.setCursor(0,0);
	lcd.print("Temp Actual       ");
	lcd.setCursor(1,1);
	lcd.print(tempActual);
}

void showAverageTemp()
{
	lcd.setCursor(0,0);
	lcd.print("Temp Promedio           ");
	lcd.setCursor(1,1);
	lcd.print(tempPromedio);
}

void showMaxTemp()
{
	lcd.setCursor(0,0);
	lcd.print("Temp Maxima     ");
	lcd.setCursor(1,1);
	lcd.print(tempMax);
}

void showMinTemp()
{
	lcd.setCursor(0,0);
	lcd.print("Temp Minima       ");
	lcd.setCursor(1,1);
	lcd.print(tempMin);
}

void recibirComando()
{
	if(Serial.available()>0)
	{
		switch(Serial.read())
		{
			case 'p':
			{
				showAverageTemp();
			}break;
			case 'm':
			{
				showMinTemp();
			}break;
			case 'M':
			{
				showMaxTemp();
			}break;
			case 'a':
			{
				showCurrentTemp();
			}break;
		}
	}
}

void enviarDatos()
{
	Serial.write('a');
	Serial.write(tempActual);
	Serial.write('M');
	Serial.write(tempMax);
	Serial.write('m');
	Serial.write(tempMin);
	Serial.write('p');
	Serial.write(tempPromedio);
}

void procesarTimer()
{
	recibirComando();
	enviarDatos();
}



/************************/
/*                           CALLBACKS                                  */
/************************/


void tomar_medicion(int valor)
{
	temperaturaMedida=valor;
	guardarTemp(valor);
	updateCurrentTemp();
	updateMinTemp();
	updateMaxTemp();
	updateAverageTemp();
}


void setup()
{
	fnqueue_init();
	lcd.begin(numCols,numRows);
	sensor_setup(tomar_medicion);
	lcd.setCursor(0,0);
	lcd.print("            ");
	
	Serial.begin(9600);
	
	// Inicializa las temps
	for(int i = 0; i<MAX_TEMPS; i++)
	temps[i] = 0;
	
	
	lcd.setCursor(0,0);
	lcd.print("hol           ");


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


ISR(TIMER2_OVF_vect) // TIMER2_OVF_vect
{
	TCNT2 = 0;
	timerOvf= (timerOvf + 1) % 9;
	if(timerOvf == 0 ) //como el prescaler esta seteado en 1024, cada 9 interrupciones cuento 1 seg(0.144 seg)
	{
		contador++;
		//timerOvf=0;
		fnqueue_add(procesarTimer);
	}
	
}