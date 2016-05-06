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
	ArrayInputsY = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder));
	ArrayInputsX = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + BUFFER_SIZE));
	ArrayResult = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + BUFFER_SIZE));

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
	for (Cont = BUFFER_SIZE - 1; Cont >= 0; Cont--) {
		ArrayInputsX[Cont + FilterOrder] = ArrayInputsX[Cont];
	}

	for (Cont = 0; Cont < FilterOrder; Cont++) {
		ArrayInputsX[Cont] = ArrayInitialConditionsX[Cont];
	}
}

void DataFilter::createArrayInputIIR() {
	for (Cont = 0; Cont < FilterOrder; Cont++) {
		ArrayInputsX[Cont] = ArrayInitialConditionsX[Cont];
	}
}

//CREO Q NO SE OCUPA
/*void DataFilter::createArrayInputY() { //****OJO**** Creo q no la necesito

 int _Cont;

 for (_Cont = NumbOutput; _Cont >= 0; _Cont--) {
 ArrayInputsY[_Cont + FilterOrder] = ArrayInputsY[_Cont];
 }

 for (_Cont = 1; _Cont < FilterOrder; _Cont++) {
 ArrayInputsY[_Cont - 1] = ArrayInitialConditionsY[_Cont];
 }
 }*/

void DataFilter::moveArray(float *pArray) {
	for (Cont = 0; Cont < FilterOrder; Cont++) {
		pArray[Cont] = pArray[Cont + BUFFER_SIZE];
	}
}

//CREO Q NO SE OCUPA
/*void DataFilter::copyArray(float *pArray1, float *pArray2) {
	for (Cont = 0; Cont < BUFFER_SIZE; Cont++) {
		pArray1[Cont + FilterOrder] = pArray2[Cont];
	}
}*/

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

void DataFilter::numbOutput(int *pArrayOfInterval) {
	NumbOutput = pArrayOfInterval[1] - pArrayOfInterval[0] + 1;
	ArrayResult = (float*) chHeapAlloc(NULL, sizeof(float) * NumbOutput);

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

void DataFilter::setArrayInputsX(float* pArrayInputs) {
	for (Cont = 0; Cont < BUFFER_SIZE; Cont++) {
		ArrayInputsX[Cont] = pArrayInputs[Cont];
	}
}

int* DataFilter::getArrayIntervalOutput() const {
	return ArrayIntervalOutput;
}

void DataFilter::setArrayIntervalOutput(int* pArrayIntervalOutput) {
	for (Cont = 0; Cont < 2; Cont++) {
		ArrayIntervalOutput[Cont] = pArrayIntervalOutput[Cont];
	}
	numbOutput(pArrayIntervalOutput);
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

