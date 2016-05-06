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

static THD_WORKING_AREA(waThread1, 256);

static THD_FUNCTION(Thread1, pArg) {
	(void) pArg;
	DataFilter *_DataFilter = (DataFilter *) pArg;

	chRegSetThreadName("Filtro");

	chprintf((BaseSequentialStream *) &SD1, "Entro hilo\r\n");

	float _EntradasX[16] = { -1, 2, 4, 6, 4, 0, 0, 0, -1, 2, 8, 5, 3, -1, 7, 5 };

	IIR *_ClassIIR = (IIR*) chHeapAlloc(NULL, sizeof(IIR));
	_ClassIIR->setInitialDataFilter(_DataFilter);

	float _Result;

	chprintf((BaseSequentialStream *) &SD1, "Resultado:");

	int _Cont;
	for (_Cont = 0; _Cont < 16; _Cont++) {
		chThdSleepMilliseconds(500);
		palTogglePad(GPIOG, GPIOG_LED1);
		chprintf((BaseSequentialStream *) &SD1, " inicio ");
		_Result = _ClassIIR->directFormI(_EntradasX[_Cont]);
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

	chprintf((BaseSequentialStream *) &SD1, "\r\n\n\nInicio de programa\r\n");

	int _FilterOrder = 2;

	float _CondicionesInicialesX[2] = { 0, 0 };
	float _CoeficientesB[3] = { 0.333, 0.333, 0.333 };

	float _CondicionesInicialesY[2] = { 0, 0 };
	float _CoeficientesA[2] = { 0.167, 0.167 };

	DataFilter *_DataFilter = (DataFilter*) chHeapAlloc(NULL,
			sizeof(DataFilter));

	_DataFilter->newDataFilter(_FilterOrder);
	_DataFilter->setArrayCoefficientsB(_CoeficientesB);
	_DataFilter->setArrayInitialConditionsX(_CondicionesInicialesX);
	_DataFilter->setArrayInitialConditionsY(_CondicionesInicialesY);
	_DataFilter->setArrayCoefficientsA(_CoeficientesA);

	void *_Arg = _DataFilter;

	chprintf((BaseSequentialStream *) &SD1, "Inicio de hilo\r\n");
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

/*static THD_WORKING_AREA(waThread2, 256);

 static THD_FUNCTION(Thread2, pArg) {
 (void) pArg;
 DataFilter *_DataFilter = (DataFilter *) pArg;

 chRegSetThreadName("Filtro");

 chprintf((BaseSequentialStream *) &SD1, "Entro hilo\r\n");

 FIR *_ClassFIR = (FIR*) chHeapAlloc(NULL, sizeof(FIR));

 float *_Result;

 chprintf((BaseSequentialStream *) &SD1, "Resultado:");

 chThdSleepMilliseconds(500);
 palTogglePad(GPIOG, GPIOG_LED1);
 chThdSleepMilliseconds(500);

 _Result = _ClassFIR->directFormI(_DataFilter);

 int cont;
 for (cont = 0; cont < 16; cont++){
 chprintf((BaseSequentialStream *) &SD1, "%f ", _Result[cont]);
 }
 chThdSleepMilliseconds(500);
 palTogglePad(GPIOG, GPIOG_LED1);

 }

 int main() {

 halInit();
 chSysInit();

 SerialConfig uartCfg = { 9600, };

 sdStart(&SD1, &uartCfg);

 chprintf((BaseSequentialStream *) &SD1, "\r\n\n\nInicio de programa\r\n");

 int _FilterOrder = 2;

 float _EntradasX[16] = { -1, 2, 4, 6, 4, 0, 0, 0, -1, 2, 8, 5, 3, -1, 7, 5 };

 float _CondicionesInicialesX[2] = { 0, 0 };
 float _CoeficientesB[3] = { 0.333, 0.333, 0.333 };

 DataFilter *_DataFilter = (DataFilter*) chHeapAlloc(NULL,
 sizeof(DataFilter));

 _DataFilter->newDataFilter(_FilterOrder);
 _DataFilter->setArrayCoefficientsB(_CoeficientesB);
 _DataFilter->setArrayInitialConditionsX(_CondicionesInicialesX);
 _DataFilter->setArrayInputsX(_EntradasX, 16);

 void *_Arg = _DataFilter;

 chprintf((BaseSequentialStream *) &SD1, "Inicio de hilo\r\n");
 chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, _Arg);

 while (TRUE) {
 if (palReadPad(GPIOG, GPIOG_BUTTON)) {
 chprintf((BaseSequentialStream *) &SD1,
 "Se ha presionado SW1 \r\n");
 chThdSleepMilliseconds(200);
 }
 chThdSleepMilliseconds(200);
 }
 }*/

