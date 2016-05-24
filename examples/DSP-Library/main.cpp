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
	float _EntradasX[50] = { -13.69094, -11.99835, -10.15919, -8.19591,
			-6.13252, -3.99420, -1.80709, 0.40210, 2.60637, 4.77880, 6.89286,
			8.92271, 10.84355, 12.63193, 14.26599, 15.72577, 16.99344, 18.05351,
			18.89303, 19.50175, 19.87223, 19.99994, 19.88332, 19.52380,
			18.92577, 18.09654, 17.04623, 15.78768, 14.33627, 12.70971,
			10.92789, 9.01257, 6.98714, 4.87636, 2.70601, 0.50260, -1.70695,
			-3.89564, -6.03675, -8.10411, -10.07246, -11.91777, -13.61749,
			-15.15084, -16.49911, -17.64582, -18.57697, -19.28117, -19.74982,
			-19.97720 };

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
	float _CondicionesInicialesX[3] = { -17.69292, -16.55573, -15.21628 };
	// Orden: BO, B1, B2, B3, ....
	float _CoeficientesB[4] = { 0.0186074, -0.0180370, -0.0180370, 0.0186074 };

	// Orden: ...., y[-3], y[-2], y[-1]
	float _CondicionesInicialesY[3] = { -2.19900, -3.61179, -5.00955 };
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
