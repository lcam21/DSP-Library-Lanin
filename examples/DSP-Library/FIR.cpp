/*
 * FIR.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */

#include <ch.h>
#include <hal.h>
#include <stdio.h>
#include <serial.h>
#include <chprintf.h>

#include "header/FIR.h"
#include "header/FuntionsMath.h"

FIR::~FIR() {
	// TODO Auto-generated destructor stub
}

float FIR::directFormI(float pData) {

	//variable that need
	float *_ArrayInputsX = InitialDataFilter->getArrayInputsX();

	int _FilterOrder = InitialDataFilter->getFilterOrder();
	int _ContInputData;

	float *_ArrayCoefficientsB = InitialDataFilter->getArrayCoefficientsB();

	float _Result;

	//if the buffer is full, move the data
	if (ContBuffer == BUFFER_SIZE) {
		InitialDataFilter->moveArray(_ArrayInputsX);
		ContBuffer = 0;
	}

	_ContInputData = _FilterOrder + ContBuffer;

	//set the inputdata
	_ArrayInputsX[_ContInputData] = pData;

	//FInal result
	_Result = MathOperation->sum(_FilterOrder, 0, _ArrayCoefficientsB,
			_ArrayInputsX, ContBuffer);

	ContBuffer++;

	return _Result;
}

void FIR::setInitialDataFilter(DataFilter *pInitialDataFilter) {
	// Create class that do the sum
	MathOperation = (FuntionsMath*) chHeapAlloc(NULL, sizeof(FuntionsMath));
	InitialDataFilter = pInitialDataFilter;
	ContBuffer = 0;
	InitialDataFilter->createArrayInput();
}

