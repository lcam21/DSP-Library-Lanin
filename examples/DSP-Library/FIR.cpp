/*
 * FIR.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */

#include "header/FIR.h"

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

