/*
 * DataFilter.cpp
 *
 *  Created on: Apr 17, 2016
 *      Author: luis
 */

#include <ch.h>
#include <hal.h>
#include <stdio.h>
#include <serial.h>
#include <chprintf.h>

#include "header/DataFilter.h"

void DataFilter::newDataFilter(int pFilterOrder) {

	//allocate memory
	ArrayInitialConditionsX = (float*) chHeapAlloc(NULL,
			sizeof(float) * pFilterOrder);
	ArrayCoefficientsB = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + 1));
	ArrayCoefficientsA = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder));
	ArrayInputsY = (float*) chHeapAlloc(NULL,
			sizeof(float) * (FilterOrder + BUFFER_SIZE));
	setFilterOrder(pFilterOrder);
}
DataFilter::~DataFilter() {
	//free the memory
	chHeapFree(ArrayInitialConditionsX);
	chHeapFree(ArrayCoefficientsB);
	chHeapFree(ArrayCoefficientsA);
	chHeapFree(ArrayInputsY);
	chHeapFree(ArrayInputsX);
}

void DataFilter::createArrayInput() {
	ArrayInputsX = (float*) chHeapAlloc(NULL,
			sizeof(float) * (FilterOrder + BUFFER_SIZE));
	for (Cont = 0; Cont < FilterOrder; Cont++) {
		ArrayInputsX[Cont] = ArrayInitialConditionsX[Cont];
	}
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
		ArrayInitialConditionsX[Cont] = pArrayInitialConditions[Cont];
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

float* DataFilter::getArrayInitialConditionsY() const {
	return ArrayInitialConditionsY;
}

void DataFilter::setArrayInitialConditionsY(float* arrayInitialConditionsY) {
	for (Cont = 0; Cont < FilterOrder; Cont++) {
		ArrayInputsY[Cont] = arrayInitialConditionsY[Cont];
	}
}

float* DataFilter::getArrayInputsY() const {
	return ArrayInputsY;
}

