/*
 * FIR.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */

//#include <c++/4.8.3/iostream>

#include "header/FIR.h"
#include "header/FuntionsMath.h"

//using namespace std;

FIR::FIR() {

	// TODO Auto-generated constructor stub

}

FIR::~FIR() {
	// TODO Auto-generated destructor stub
}

DataFilter* FIR::directFormI(DataFilter *pDataFilter) {

	FuntionsMath _MathOperation;

	pDataFilter->createArrayInputX();

	int _Cont;
	int _FilterOrder = pDataFilter->getFilterOrder();
	int _NumbOutput = pDataFilter->getNumbOutput();
	float *_ArrayResult = pDataFilter->getArrayResult();
	float *_ArrayCoefficients =  pDataFilter->getArrayCoefficientsB();
	float *_ArrayInputs = pDataFilter->getArrayInputsX();

	for (_Cont = 0; _Cont < _NumbOutput; _Cont++) {
		_ArrayResult[_Cont] = _MathOperation.sum(_FilterOrder, 0,
				_ArrayCoefficients, _ArrayInputs, _Cont);
	}


	/*cout << "y[n]:";
	for (_Cont = 0; _Cont < _NumbOutput; _Cont++) {
		cout << " " << _ArrayResult[_Cont];
	}
	cout << "" << endl;*/

	return pDataFilter;
}


