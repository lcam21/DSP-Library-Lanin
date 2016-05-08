/*
 * DataFilter.cpp
 *
 *  Created on: Apr 17, 2016
 *      Author: luis
 */

#include "header/DataFilter.h"

void DataFilter::newDataFilter(int pFilterOrder) {

	//allocate memory
	ArrayCoefficientsB = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + 1));
	ArrayInputsX = (float*) chHeapAlloc(NULL,
				sizeof(float) * (FilterOrder + BUFFER_SIZE));
	ArrayCoefficientsA = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder));
	ArrayInputsY = (float*) chHeapAlloc(NULL,
			sizeof(float) * (FilterOrder + BUFFER_SIZE));
	setFilterOrder(pFilterOrder);
}
DataFilter::~DataFilter() {
	//free the memory
	chHeapFree(ArrayCoefficientsB);
	chHeapFree(ArrayCoefficientsA);
	chHeapFree(ArrayInputsY);
	chHeapFree(ArrayInputsX);
}

void DataFilter::moveArray(float *pArray) {
	for (Cont = 0; Cont < FilterOrder; Cont++) {
		pArray[Cont] = pArray[Cont + BUFFER_SIZE];
	}
}

float* DataFilter::getArrayCoefficientsB() const {
	return ArrayCoefficientsB;
}

void DataFilter::setArrayCoefficientsB(float* pArrayCoefficientsB) {
	for (Cont = 0; Cont < (FilterOrder + 1); Cont++) {
		ArrayCoefficientsB[Cont] = pArrayCoefficientsB[Cont];
	}
}

void DataFilter::setArrayInitialConditionsX(float *pArrayInitialConditions) {
	for (Cont = 0; Cont < FilterOrder; Cont++) {
		ArrayInputsX[Cont] = pArrayInitialConditions[Cont];
	}
}

float* DataFilter::getArrayInputsX() const {
	return ArrayInputsX;
}

int DataFilter::getFilterOrder() const {
	return FilterOrder;
}

void DataFilter::setFilterOrder(int pFilterOrder) {
	FilterOrder = pFilterOrder;
}

float* DataFilter::getArrayCoefficientsA() const {
	return ArrayCoefficientsA;
}

void DataFilter::setArrayCoefficientsA(float* arrayCoefficientsA) {
	for (Cont = 0; Cont < (FilterOrder + 1); Cont++) {
		ArrayCoefficientsA[Cont] = arrayCoefficientsA[Cont];
	}
}

void DataFilter::setArrayInitialConditionsY(float* arrayInitialConditionsY) {
	for (Cont = 0; Cont < FilterOrder; Cont++) {
		ArrayInputsY[Cont] = arrayInitialConditionsY[Cont];
	}
}

float* DataFilter::getArrayInputsY() const {
	return ArrayInputsY;
}

