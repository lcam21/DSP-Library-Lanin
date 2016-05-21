/*
 * FuntionsMath.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */


#include "header/FuntionsMath.h"


float FuntionsMath::sum(int pUpperLimit, int pLowerLimit, float* pCoefficients,
		float* pArray, int pCont) {

	float _Result = 0; //save the result
	int _PosInitCond = 0; //access to
	float _auxCoeficiente;
	float _auxArray;

	for (Cont = pLowerLimit; Cont <= pUpperLimit; Cont++) {

		_PosInitCond = pCont + pUpperLimit;

		_auxCoeficiente = pCoefficients[Cont];
		_auxArray = pArray[_PosInitCond];

		_Result += _auxCoeficiente * _auxArray;

		//_Result += pCoefficients[Cont] * pArray[_PosInitCond];

		pCont--;
	}
	return _Result;
}

