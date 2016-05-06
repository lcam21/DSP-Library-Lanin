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
	ArrayIntervalOutput = (int*) chHeapAlloc(NULL, sizeof(int) * 2);
	ArrayInputsY = (float*) chHeapAlloc(NULL, sizeof(float) * (pFilterOrder));

	setFilterOrder(pFilterOrder);
}
DataFilter::~DataFilter() {
	//free the memory
	chHeapFree(ArrayInitialConditionsX);
	chHeapFree(ArrayCoefficientsB);
	chHeapFree(ArrayIntervalOutput);
	chHeapFree(ArrayInputsY);
	chHeapFree(ArrayInputsX);
	chHeapFree(ArrayResult);
}

void DataFilter::createArrayInputFIR() {
	for (Cont = NumbOutput - 1; Cont >= 0; Cont--) {
		ArrayInputsX[Cont + FilterOrder] = ArrayInputsX[Cont];
	}

	for (Cont = 0; Cont < FilterOrder; Cont++) {
		ArrayInputsX[Cont] = ArrayInitialConditionsX[Cont];
	}
}

void DataFilter::createArrayInputIIR() {
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

float* DataFilter::getArrayResult() const {
	return ArrayResult;
}

void DataFilter::setArrayResult(float* arrayResult) {
	ArrayResult = arrayResult;
}

int DataFilter::getNumbOutput() const {
	return NumbOutput;
}

void DataFilter::setNumbOutput(int numbOutput) {
	NumbOutput = numbOutput;
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

void DataFilter::setArrayInputsX(float* pArrayInputs, int pSizeInputs) {
	NumbOutput = pSizeInputs;
	ArrayInputsX = (float*) chHeapAlloc(NULL,sizeof(float) * (FilterOrder + pSizeInputs));
	for (Cont = 0; Cont < pSizeInputs; Cont++) {
		ArrayInputsX[Cont] = pArrayInputs[Cont];
	}
	ArrayResult = (float*) chHeapAlloc(NULL, sizeof(float) * NumbOutput);
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
	ArrayCoefficientsA = arrayCoefficientsA;
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

