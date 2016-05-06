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
#include "header/DataFilter.h"

static THD_WORKING_AREA(waThread1, 256);

static THD_FUNCTION(Thread1, pArg) {
	(void) pArg;

	DataFilter *_DataFilter1 = (DataFilter *) pArg;

	(void) pArg;
	chRegSetThreadName("Filtro");
	chprintf((BaseSequentialStream *) &SD1, "Entro hilo\r\n");
	//while (TRUE) {
	chThdSleepMilliseconds(1000);
	palTogglePad(GPIOG, GPIOG_LED1);
	chThdSleepMilliseconds(1000);
	IIR *_ClassIIR = (IIR*) chHeapAlloc(NULL, sizeof(IIR));
	_ClassIIR->directFormI(_DataFilter1);
	palTogglePad(GPIOG, GPIOG_LED1);
	chThdSleepMilliseconds(500);
	//}
}

int main() {

	halInit();
	chSysInit();

	SerialConfig uartCfg = { 9600, };

	sdStart(&SD1, &uartCfg);

	chprintf((BaseSequentialStream *) &SD1, "\n\n\nInicio de programa\r\n");

	int _FilterOrder = 2;

	float _CondicionesInicialesX[2] = { 0, 0 };
	float _CoeficientesB[3] = { 0.333, 0.333, 0.333 };

	float _EntradasX1[10] = { -1, 2, 4, 6, 4, 0, 0, 0, -1, 2 };
	float _EntradasX2[10] = { 8, 5, 3, -1, 7, 5, 0, 0, 0, 0 };

	float _CondicionesInicialesY[2] = { 0, 0 };
	float _CoeficientesA[2] = { 0.167, 0.167 };

	int _IntervaloResultado[2] = { -1, 14 };

	DataFilter *_DataFilter = (DataFilter*) chHeapAlloc(NULL,
			sizeof(DataFilter));

	_DataFilter->newDataFilter(_FilterOrder);

	_DataFilter->setBuffer1(_EntradasX1);
	_DataFilter->setBuffer2(_EntradasX2);

	_DataFilter->setArrayCoefficientsB(_CoeficientesB);
	_DataFilter->setArrayInitialConditionsX(_CondicionesInicialesX);
	_DataFilter->setArrayInputsX(_EntradasX1);

	_DataFilter->setArrayIntervalOutput(_IntervaloResultado);

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
