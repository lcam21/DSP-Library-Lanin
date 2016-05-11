/*
 * DataFilter.cpp
 *
 *  Created on: Apr 17, 2016
 *      Author: luis
 */

#include "header/DataFilter.h"

#include <stdio.h>
#include <serial.h>
#include <chprintf.h>

void DataFilter::newDataFilter(int pFilterOrder, float *pArrayInputsX,
		float *pArrayCoefficientsB, typeOfDirectForm pTypeOfDirectForm) {

	//allocate memory
	ArrayCoefficientsB = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + 1));
	ArrayInputsX = (float*) chHeapAlloc(NULL,
			sizeof(float) * (FilterOrder + BUFFER_SIZE));

	setFilterOrder(pFilterOrder);
	setTypeOfDirectForm(pTypeOfDirectForm);
	setArrayInitialConditionsX(pArrayInputsX);
	setArrayCoefficientsB(pArrayCoefficientsB);

}

void DataFilter::newDataFilter(int pFilterOrder, float *pArrayInputsX,
		float *pArrayCoefficientsB, float *pArrayInputsY,
		float *pArrayCoefficientsA, typeOfDirectForm pTypeOfDirectForm) {

	//allocate memory
	ArrayCoefficientsB = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + 1));
	ArrayInputsX = (float*) chHeapAlloc(NULL,
			sizeof(float) * (FilterOrder + BUFFER_SIZE));
	ArrayCoefficientsA = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder));
	ArrayInputsY = (float*) chHeapAlloc(NULL,
			sizeof(float) * (FilterOrder + BUFFER_SIZE));

	//set variables
	setFilterOrder(pFilterOrder);
	setTypeOfDirectForm(pTypeOfDirectForm);
	setArrayInitialConditionsX(pArrayInputsX);
	setArrayCoefficientsB(pArrayCoefficientsB);
	setArrayInitialConditionsY(pArrayInputsY);
	setArrayCoefficientsA(pArrayCoefficientsA);
}

void DataFilter::newDataFilter(int pFilterOrder, float *pArrayInputsX,
		float *pArrayCoefficientsB, float *pArrayCoefficientsA,
		typeOfDirectForm pTypeOfDirectForm) {

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
	setTypeOfDirectForm(pTypeOfDirectForm);
	setArrayInitialConditionsX(pArrayInputsX);
	setArrayCoefficientsB(pArrayCoefficientsB);
	setArrayCoefficientsA(pArrayCoefficientsA);

	createArrayAux();
}

DataFilter::~DataFilter() {
	//free the memory
	chHeapFree(ArrayCoefficientsB);
	chHeapFree(ArrayCoefficientsA);
	chHeapFree(ArrayInputsY);
	chHeapFree(ArrayInputsX);
}

void DataFilter::createArrayAux() {

	MathOperation = (FuntionsMath*) chHeapAlloc(NULL, sizeof(FuntionsMath));

	float _NumSum;
	float _NumX;
	float _NumResult;

	ArrayInputsY[0] = ArrayInputsX[0];
	chprintf((BaseSequentialStream *) &SD1, "%f ", ArrayInputsY[0]); //print result

	for (Cont = 0; Cont < (FilterOrder - 1); Cont++) {
		_NumSum = MathOperation->sum(Cont, 0, ArrayCoefficientsA, ArrayInputsY,
				Cont);
		_NumX = ArrayInputsX[Cont + 1];
		_NumResult = _NumX - _NumSum;
		ArrayInputsY[Cont + 1] = _NumResult;
		chprintf((BaseSequentialStream *) &SD1, "%f ", _NumResult); //print result
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

int DataFilter::getTypeOfDirectForm() const {
	return TypeOfDirectForm;
}

void DataFilter::setTypeOfDirectForm(int typeOfDirectForm) {
	TypeOfDirectForm = typeOfDirectForm;
}
