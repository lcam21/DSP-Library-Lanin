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

	for (Cont = pLowerLimit; Cont <= pUpperLimit; Cont++) {

		_PosInitCond = pCont + pUpperLimit;
		_Result += pCoefficients[Cont] * pArray[_PosInitCond];

		pCont--;
	}
	return _Result;
}

