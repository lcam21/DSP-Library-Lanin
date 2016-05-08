//============================================================================
// Name        : DSP-Library.cpp
// Author      : Luis Alvarez
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <iostream>

#include <ch.h>
#include <hal.h>
#include <stdio.h>
#include <serial.h>
#include <chprintf.h>

#include "header/IIR.h"
#include "header/FIR.h"
#include "header/DataFilter.h"


//***********IIR***********
static THD_WORKING_AREA(waThread1, 256);

static THD_FUNCTION(Thread1, pArg) { //Funtion that the thread execute
	(void) pArg;
	DataFilter *_DataFilter = (DataFilter *) pArg;

	chRegSetThreadName("Filtro");

	//data
	float _EntradasX[9] = { 0.00000, 0.62791, 1.25333, 1.87381, 2.48690,
			3.09017, 3.68125, 4.25779, 4.81754 };

	IIR *_ClassIIR = (IIR*) chHeapAlloc(NULL, sizeof(IIR));
	_ClassIIR->setInitialDataFilter(_DataFilter); //set data of filter

	float _Result;

	chprintf((BaseSequentialStream *) &SD1, "\r\nResultado:");

	int _Cont;
	//input data of filter
	for (_Cont = 0; _Cont < 9; _Cont++) {
		chThdSleepMilliseconds(500);
		palTogglePad(GPIOG, GPIOG_LED1);
		_Result = _ClassIIR->directFormI(_EntradasX[_Cont]); //set data
		chprintf((BaseSequentialStream *) &SD1, "%f ", _Result); //print result
		chThdSleepMilliseconds(500);
		palTogglePad(GPIOG, GPIOG_LED1);
	}
}

int main() {

	halInit();
	chSysInit();

	SerialConfig uartCfg = { 9600, };

	sdStart(&SD1, &uartCfg);

	int _FilterOrder = 3;

	float _CondicionesInicialesX[3] = { 0, 0, 0 };
	float _CoeficientesB[4] = { -0.0209863, 0.0203430, 0.0203430, -0.0209863 };

	float _CondicionesInicialesY[3] = { 0, 0, 0 };
	float _CoeficientesA[3] = { -3.1089549, 3.2355190, -1.1278510 };

	DataFilter *_DataFilter = (DataFilter*) chHeapAlloc(NULL,
			sizeof(DataFilter));

	//set data of filter
	_DataFilter->newDataFilter(_FilterOrder);
	_DataFilter->setArrayCoefficientsB(_CoeficientesB);
	_DataFilter->setArrayInitialConditionsX(_CondicionesInicialesX);
	_DataFilter->setArrayInitialConditionsY(_CondicionesInicialesY);
	_DataFilter->setArrayCoefficientsA(_CoeficientesA);

	void *_Arg = _DataFilter;

	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, _Arg);

	while (TRUE) {
		if (palReadPad(GPIOG, GPIOG_BUTTON)) {
			chprintf((BaseSequentialStream *) &SD1,
					"Se ha presionado SW1 \r\n");
			chThdSleepMilliseconds(200);
		}
		chThdSleepMilliseconds(200);
	}
}

//***********FIR***********

/*
 static THD_WORKING_AREA(waThread2, 256);

 static THD_FUNCTION(Thread2, pArg) {
 (void) pArg;
 DataFilter *_DataFilter = (DataFilter *) pArg;

 chRegSetThreadName("Filtro");

 FIR *_ClassFIR = (FIR*) chHeapAlloc(NULL, sizeof(FIR));
 _ClassFIR->setInitialDataFilter(_DataFilter);

 float _Result;

 chprintf((BaseSequentialStream *) &SD1, "\r\nResultado:");

 float _EntradasX[16] = { -1, 2, 4, 6, 4, 0, 0, 0, -1, 2, 8, 5, 3, -1, 7, 5 };

 int _Cont;
 for (_Cont = 0; _Cont < 16; _Cont++) {
 chThdSleepMilliseconds(500);
 palTogglePad(GPIOG, GPIOG_LED1);
 _Result = _ClassFIR->directFormI(_EntradasX[_Cont]);
 chprintf((BaseSequentialStream *) &SD1, "%f ", _Result);
 chThdSleepMilliseconds(500);
 palTogglePad(GPIOG, GPIOG_LED1);

 }
 }

 int main() {

 halInit();
 chSysInit();

 SerialConfig uartCfg = { 9600, };

 sdStart(&SD1, &uartCfg);

 int _FilterOrder = 2;

 float _CondicionesInicialesX[2] = { 0, 0 };
 float _CoeficientesB[3] = { 0.333, 0.333, 0.333 };

 DataFilter *_DataFilter = (DataFilter*) chHeapAlloc(NULL,
 sizeof(DataFilter));

 _DataFilter->newDataFilter(_FilterOrder);
 _DataFilter->setArrayCoefficientsB(_CoeficientesB);
 _DataFilter->setArrayInitialConditionsX(_CondicionesInicialesX);

 void *_Arg = _DataFilter;

 chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, _Arg);

 while (TRUE) {
 if (palReadPad(GPIOG, GPIOG_BUTTON)) {
 chprintf((BaseSequentialStream *) &SD1,
 "Se ha presionado SW1 \r\n");
 chThdSleepMilliseconds(200);
 }
 chThdSleepMilliseconds(200);
 }
 }

 */
