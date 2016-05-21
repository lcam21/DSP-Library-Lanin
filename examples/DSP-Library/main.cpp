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

// 0 - FIR Form Direct I
// 1 - IIR Form Direct I
// 2 - IIR Form Direct II
int TipoFiltro = 2;

static THD_WORKING_AREA(waThread1, 256);

static THD_FUNCTION(Thread1, pArg) { //Funtion that the thread execute
	(void) pArg;
	DataFilter *_DataFilter = (DataFilter *) pArg;

	chRegSetThreadName("Filtro");

	//data
	float _EntradasX[50] = { 0.00000, 2.20718, 4.38739, 6.51400, 8.56104,
			10.50349, 12.31763, 13.98129, 15.47415, 16.77797, 17.87683,
			18.75729, 19.40861, 19.82283, 19.99488, 19.92267, 19.60708,
			19.05196, 18.26409, 17.25310, 16.03134, 14.61374, 13.01760,
			11.26245, 9.36970, 7.36249, 5.26534, 3.10386, 0.90447, -1.30597,
			-3.50046, -5.65219, -7.73486, -9.72305, -11.59245, -13.32024,
			-14.88530, -16.26852, -17.45299, -18.42425, -19.17044, -19.68243,
			-19.95397, -19.98175, -19.76542, -19.30763, -18.61397, -17.69292,
			-16.55573, -15.21628 };

	float _Result;

	IIR *_ClassIIR = (IIR*) chHeapAlloc(NULL, sizeof(IIR));
	FIR *_ClassFIR = (FIR*) chHeapAlloc(NULL, sizeof(FIR));

	if (TipoFiltro == 1) {
		_ClassIIR->setInitialDataFilter(_DataFilter); //set data of filter

	} else if (TipoFiltro == 0) {
		_ClassFIR->setInitialDataFilter(_DataFilter);
	} else {
		_ClassIIR->setInitialDataFilter(_DataFilter);
	}

	chprintf((BaseSequentialStream *) &SD1, "\r\n\n\n\nResultado:");

	int _Cont;
	//input data of filter
	for (_Cont = 0; _Cont < 50; _Cont++) {
		chThdSleepMilliseconds(200);
		palTogglePad(GPIOG, GPIOG_LED1);

		if (TipoFiltro == 1) {
			_Result = _ClassIIR->directFormI(_EntradasX[_Cont]); //set data
		} else if (TipoFiltro == 0) {
			_Result = _ClassFIR->directFormI(_EntradasX[_Cont]);
		} else {
			_Result = _ClassIIR->directFormII(_EntradasX[_Cont]);
		}

		chprintf((BaseSequentialStream *) &SD1, "%f ", _Result); //print result

		chThdSleepMilliseconds(200);
		palTogglePad(GPIOG, GPIOG_LED1);
	}
	if (TipoFiltro == 1) {
		_ClassIIR->~Filter();

	} else {
		_ClassFIR->~Filter();
	}
}

int main() {

	halInit();
	chSysInit();

	SerialConfig uartCfg = { 9600, };

	sdStart(&SD1, &uartCfg);

	int _FilterOrder = 3;

	// Orden: ...., x[-3], x[-2], x[-1]
	float _CondicionesInicialesX[3] = { 0, 0, 0 };
	// Orden: BO, B1, B2, B3, ....
	float _CoeficientesB[4] = { 0.0186074, -0.0180370, -0.0180370, 0.0186074 };

	// Orden: ...., y[-3], y[-2], y[-1]
	float _CondicionesInicialesY[3] = { 0, 0, 0 };
	//Orden: A1, A2, A3, ...
	float _CoeficientesA[3] = { -2.8687468, 2.7565297, -0.8866419 };

	DataFilter *_DataFilter = (DataFilter*) chHeapAlloc(NULL,
			sizeof(DataFilter));

	//set data of filter
	typeOfDirectForm _TypeOfForm = 1;
	if (TipoFiltro == 1) {
		_DataFilter->newDataFilter(_FilterOrder, _CondicionesInicialesX,
				_CoeficientesB, _CondicionesInicialesY, _CoeficientesA,
				_TypeOfForm);
	} else if (TipoFiltro == 0) {

		_DataFilter->newDataFilter(_FilterOrder, _CondicionesInicialesX,
				_CoeficientesB, _TypeOfForm);
	} else {
		_DataFilter->newDataFilter(_FilterOrder, _CondicionesInicialesX,
				_CoeficientesB, _CoeficientesA, _TypeOfForm);
	}

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
