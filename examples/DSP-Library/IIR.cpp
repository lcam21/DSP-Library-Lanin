/*
 * IIR.cpp
 *
 *  Created on: Apr 18, 2016
 *      Author: lcalvare
 */

#include "header/IIR.h"


float IIR::directFormI(float pData) {

	//variable that need
	float *_ArrayInputsX = InitialDataFilter->getArrayInputsX();
	float *_ArrayInputsY = InitialDataFilter->getArrayInputsY();

	int _FilterOrder = InitialDataFilter->getFilterOrder();
	int _ContInputData;

	float _ResultX = 0;
	float *_ArrayCoefficientsB = InitialDataFilter->getArrayCoefficientsB();

	float _ResultY = 0;
	float *_ArrayCoefficientsA = InitialDataFilter->getArrayCoefficientsA();

	float _Result;

	//if the buffer is full, move the data
	if (ContBuffer == BUFFER_SIZE) {
		InitialDataFilter->moveArray(_ArrayInputsX);
		InitialDataFilter->moveArray(_ArrayInputsY);
		ContBuffer = 0;
	}

	_ContInputData = _FilterOrder + ContBuffer;

	//set the inputdata
	_ArrayInputsX[_ContInputData] = pData;

	// sum for X
	_ResultX = MathOperation->sum(_FilterOrder, 0, _ArrayCoefficientsB,
			_ArrayInputsX, ContBuffer);

	//sum for Y
	_ResultY = MathOperation->sum(_FilterOrder - 1, 0, _ArrayCoefficientsA,
			_ArrayInputsY, ContBuffer);

	// Final reusult
	_Result = _ResultX - _ResultY;

	//set the data for history
	_ArrayInputsY[_ContInputData] = _Result;

	ContBuffer++;

	return _Result;

}
