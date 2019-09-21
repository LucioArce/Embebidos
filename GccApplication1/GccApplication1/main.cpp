
#define F_CPU 16000000U
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

// include the library code:
#include "Arduino.h"
#include "LiquidCrystal.h"
#include "teclado.h"
#include "fnqueue.h"

const uint8_t numRows = 2;
const uint8_t numCols = 16;

static uint16_t volatile contador = 0; //contador


#define MAX_TEMPS 100
static uint16_t temps[MAX_TEMPS];

static uint8_t tempActual = 0; 
static uint8_t tempMin = 200;
static uint8_t tempMax = 0;
static uint8_t tempPromedio = 0;

static uint8_t cantTemps = 0;
static uint8_t indiceArreglo = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


/************************************************************************/
/*                          AUXILIARES                                  */
/************************************************************************/
void updateCurrentTemp()
{
	
}
void updateAverageTemp()
{
	
}
void updateMaxTemp()
{
	
}
void updateMinTemp()
{
	
}
void showCurrentTemp()
{
	lcd.setCursor(0,0);
	lcd.print("Temp Actual");
	lcd.setCursor(0,1);
	lcd.print(tempActual);
}

void showAverageTemp()
{
	lcd.setCursor(0,0);
	lcd.print("Temp Promedio");
	lcd.setCursor(0,1);
	lcd.print(tempPromedio);
}

void showMaxTemp()
{
	lcd.setCursor(0,0);
	lcd.print("Temp Maxima");
	lcd.setCursor(0,1);
	lcd.print(tempMax);
}

void showMinTemp()
{
	lcd.setCursor(0,0);
	lcd.print("Temp Minima");
	lcd.setCursor(0,1);
	lcd.print(tempMin);
}

/************************************************************************/
/*                           CALLBACKS                                  */
/************************************************************************/

void up_keyLeft()
{
	updateCurrentTemp();
	showCurrentTemp();
}

void up_keyRight()
{
	updateAverageTemp();
	showAverageTemp();
}

void up_keyUp()
{
	updateMaxTemp();
	showMaxTemp();
}

void up_keyDown()
{
	updateMinTemp();
	showMinTemp();
}

void up_keySelect()
{
	
}

void down_keyUp()
{
	
}

void down_keyDown()
{
	
}

void down_keyLeft()
{
	
}

void down_keyRight()
{
	
}

void down_keySelect()
{

}

void setup()
{
	fnqueue_init();
	teclado_setup();
	sensor_setup();
	
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
	for(int i = 0; i<MAX_TEMPS; i++)
	tiempos[i] = 0;
	
}

void loop()
{
	fnqueue_run();
}

void procesarTimer()
{
	
}

