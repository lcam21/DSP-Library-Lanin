/*
 * FuntionsMath.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */


#include "header/FuntionsMath.h"

FuntionsMath::FuntionsMath() {
	// TODO Auto-generated constructor stub
}

float FuntionsMath::sum(int pUpperLimit, int pLowerLimit, float* pCoefficients,
		float* pArray, int pCont) {

	float _Result = 0; //save the result
	int _PosInitCond = 0; //access to
	int _Cont; //cont

	for (_Cont = pLowerLimit; _Cont <= pUpperLimit; _Cont++) {

		_PosInitCond = pCont + pUpperLimit;
		_Result += pCoefficients[_Cont] * pArray[_PosInitCond];

		pCont--;
	}
	return _Result;
}


FuntionsMath::~FuntionsMath() {
	// TODO Auto-generated destructor stub
}

