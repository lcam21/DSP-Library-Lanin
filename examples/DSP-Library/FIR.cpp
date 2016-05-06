/*
 * FIR.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */

#include <ch.h>
#include <hal.h>
#include <stdio.h>

#include "header/FIR.h"
#include "header/FuntionsMath.h"

FIR::FIR() {

	// TODO Auto-generated constructor stub

}

FIR::~FIR() {
	// TODO Auto-generated destructor stub
}

DataFilter* FIR::directFormI(DataFilter *pDataFilter) {

	FuntionsMath *_MathOperation = (FuntionsMath*) chHeapAlloc(NULL,
				sizeof(FuntionsMath));

	pDataFilter->createArrayInputFIR();

	int _Cont;
	int _FilterOrder = pDataFilter->getFilterOrder();
	int _NumbOutput = pDataFilter->getNumbOutput();
	float *_ArrayResult = pDataFilter->getArrayResult();
	float *_ArrayCoefficients =  pDataFilter->getArrayCoefficientsB();
	float *_ArrayInputs = pDataFilter->getArrayInputsX();

	for (_Cont = 0; _Cont < _NumbOutput; _Cont++) {
		_ArrayResult[_Cont] = _MathOperation->sum(_FilterOrder, 0,
				_ArrayCoefficients, _ArrayInputs, _Cont);
	}

	return pDataFilter;
}


