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

DataFilter::DataFilter(int pFilterOrder) {
//	ArrayInitialConditionsX = new float[pFilterOrder];
//	ArrayCoefficientsB = new float[pFilterOrder + 1];
//	ArrayIntervalOutput = new int[2];
//	ArrayInputsY = new float[pFilterOrder + 1]; //NO SE OCUPA
//	ArrayInputsX = new float[pFilterOrder + BUFFER_SIZE];
//	ArrayResult = new float[pFilterOrder + BUFFER_SIZE];
//	Buffer1 = new float[BUFFER_SIZE];
//	Buffer2 = new float[BUFFER_SIZE];
//	setFilterOrder(pFilterOrder);
}

void DataFilter::newDataFilter(int pFilterOrder) {

	ArrayInitialConditionsX = (float*) chHeapAlloc(NULL,
			sizeof(float) * pFilterOrder);
	ArrayCoefficientsB = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + 1));
	ArrayIntervalOutput = (int*) chHeapAlloc(NULL, sizeof(int) * 2);
	ArrayInputsY = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + 1));
	ArrayInputsX = (float*) chHeapAlloc(NULL, sizeof(float) * (pFilterOrder + BUFFER_SIZE));
	ArrayResult = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + BUFFER_SIZE));
	Buffer1 = (float*) chHeapAlloc(NULL, sizeof(float) * BUFFER_SIZE);
	Buffer2 = (float*) chHeapAlloc(NULL, sizeof(float) * BUFFER_SIZE);

	setFilterOrder(pFilterOrder);
}
DataFilter::~DataFilter() {
	/*delete ArrayInitialConditionsX;
	 delete ArrayCoefficientsB;
	 delete ArrayInputsX;
	 delete ArrayIntervalOutput;
	 delete ArrayResult;*/
}

void DataFilter::createArrayInputX() {

	int _Cont = 0;

	for (_Cont = BUFFER_SIZE - 1; _Cont >= 0; _Cont--) {
		ArrayInputsX[_Cont + FilterOrder] = ArrayInputsX[_Cont];
	}

	for (_Cont = 0; _Cont < FilterOrder; _Cont++) {
		ArrayInputsX[_Cont] = ArrayInitialConditionsX[_Cont];
	}
}

void DataFilter::createArrayInputY() { //****OJO**** Creo q no la necesito

	int _Cont;

	for (_Cont = NumbOutput; _Cont >= 0; _Cont--) {
		ArrayInputsY[_Cont + FilterOrder] = ArrayInputsY[_Cont];
	}

	for (_Cont = 1; _Cont < FilterOrder; _Cont++) {
		ArrayInputsY[_Cont - 1] = ArrayInitialConditionsY[_Cont];
	}
}

void DataFilter::moveArray(float *pArray) {
	int _Cont;

	for (_Cont = 0; _Cont < FilterOrder; _Cont++) {
		pArray[_Cont] = pArray[_Cont + BUFFER_SIZE];
	}
}

void DataFilter::copyArray(float *pArray1, float *pArray2) {
	int _Cont;

	for (_Cont = 0; _Cont < BUFFER_SIZE; _Cont++) {
		pArray1[_Cont + FilterOrder] = pArray2[_Cont];
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

float* DataFilter::getBuffer1() const {
	return Buffer1;
}

void DataFilter::setBuffer1(float* buffer1) {
	Buffer1 = buffer1;
	Buffer1Ready = 1;
}

float* DataFilter::getBuffer2() const {
	return Buffer2;
}

void DataFilter::setBuffer2(float* buffer2) {
	Buffer2 = buffer2;
	Buffer2Ready = 1;
}

void DataFilter::numbOutput(int *pArrayOfInterval) {
	NumbOutput = pArrayOfInterval[1] - pArrayOfInterval[0] + 1;
	ArrayResult = (float*) chHeapAlloc(NULL, sizeof(float) * NumbOutput);

}

float* DataFilter::getArrayCoefficientsB() const {
	return ArrayCoefficientsB;
}

void DataFilter::setArrayCoefficientsB(float* pArrayCoefficients) {
	ArrayCoefficientsB = pArrayCoefficients;
}


void DataFilter::setArrayInitialConditionsX(float *pArrayInitialConditions) {
	ArrayInitialConditionsX = pArrayInitialConditions;
}

float* DataFilter::getArrayInputsX() const {
	return ArrayInputsX;
}

void DataFilter::setArrayInputsX(float* pArrayInputs) {
	int _Cont;
	for (_Cont = 0; _Cont < BUFFER_SIZE; _Cont++) {
		ArrayInputsX[_Cont] = pArrayInputs[_Cont];
	}
}

int* DataFilter::getArrayIntervalOutput() const {
	return ArrayIntervalOutput;
}

void DataFilter::setArrayIntervalOutput(int* pArrayIntervalOutput) {
	ArrayIntervalOutput = pArrayIntervalOutput;
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
	int _Cont;

	for (_Cont = 0; _Cont < FilterOrder; _Cont++) {
		ArrayInputsY[_Cont] = arrayInitialConditionsY[_Cont];
	}

	//ArrayInitialConditionsY = arrayInitialConditionsY;
}

float* DataFilter::getArrayInputsY() const {
	return ArrayInputsY;
}

void DataFilter::setArrayInputsY(float* arrayInputsY) {
	ArrayInputsY = arrayInputsY;
}
