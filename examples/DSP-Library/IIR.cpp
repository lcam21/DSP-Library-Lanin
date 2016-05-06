/*
 * IIR.cpp
 *
 *  Created on: Apr 18, 2016
 *      Author: lcalvare
 */

#include <ch.h>
#include <hal.h>
#include <stdio.h>
#include <serial.h>
#include <chprintf.h>

#include "header/IIR.h"
#include "header/FuntionsMath.h"

//using namespace std;

IIR::IIR() {
	// TODO Auto-generated constructor stub

}

IIR::~IIR() {
	// TODO Auto-generated destructor stub
}

void IIR::directFormI(DataFilter *pDataFilter) {

	FuntionsMath *_MathOperation = (FuntionsMath*) chHeapAlloc(NULL,
			sizeof(FuntionsMath));

	chprintf((BaseSequentialStream *) &SD1, "Entro directFormI\r\n");

	pDataFilter->createArrayInputX();

	int _Cont;

	int _FilterOrder = pDataFilter->getFilterOrder();
	int _NumbOutput = pDataFilter->getNumbOutput();

	float _ArrayResultX = 0;
	float *_ArrayCoefficientsB = pDataFilter->getArrayCoefficientsB();
	float *_ArrayInputsX = pDataFilter->getArrayInputsX();

	float _ArrayResultY = 0;
	float *_ArrayCoefficientsA = pDataFilter->getArrayCoefficientsA();
	float *_ArrayInputsY = pDataFilter->getArrayInputsY();

	float *_ArrayResult = pDataFilter->getArrayResult();

	int _ContBuffer = 0;
	bool _NumbBuffer = 1;

	chprintf((BaseSequentialStream *) &SD1, "y[n]:");

	// Se pueden hacer dos for y paralelizar
	for (_Cont = 0; _Cont < _NumbOutput; _Cont++) {
		if (_ContBuffer == BUFFER_SIZE) {
			if (_NumbBuffer == 0) {
				pDataFilter->moveArray(_ArrayInputsX);
				pDataFilter->moveArray(_ArrayInputsY);
				pDataFilter->copyArray(_ArrayInputsX,
						pDataFilter->getBuffer1());
				_NumbBuffer = 1;
			} else if (_NumbBuffer == 1) {
				pDataFilter->moveArray(_ArrayInputsX);
				pDataFilter->moveArray(_ArrayInputsY);
				pDataFilter->copyArray(_ArrayInputsX,
						pDataFilter->getBuffer2());
				_NumbBuffer = 0;
			}

			_ContBuffer = 0;
		}

		_ArrayResultX = _MathOperation->sum(_FilterOrder, 0,
				_ArrayCoefficientsB, _ArrayInputsX, _ContBuffer);

		_ArrayResultY = _MathOperation->sum(_FilterOrder - 1, 0,
				_ArrayCoefficientsA, _ArrayInputsY, _ContBuffer);

		_ArrayResult[_Cont] = _ArrayResultX - _ArrayResultY;
		_ArrayInputsY[_FilterOrder + _ContBuffer] = _ArrayResult[_Cont];

		_ContBuffer++;

		chprintf((BaseSequentialStream *) &SD1, "%f ", _ArrayResult[_Cont]);
	}

	chprintf((BaseSequentialStream *) &SD1, "\n\n\n");

	//return pDataFilter;
}
