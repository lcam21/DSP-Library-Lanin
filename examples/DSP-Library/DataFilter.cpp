#include "header/DataFilter.h"

void DataFilter::newDataFilter(int pFilterOrder, float *pArrayInputsX,
		float *pArrayCoefficientsB) {

	//allocate memory
	ArrayCoefficientsB = (float*) chHeapAlloc(NULL,
			sizeof(float) * (pFilterOrder + 1));
	ArrayInputsX = (float*) chHeapAlloc(NULL,
			sizeof(float) * (FilterOrder + BUFFER_SIZE));

	setFilterOrder(pFilterOrder);
	setArrayInitialConditionsX(pArrayInputsX);
	setArrayCoefficientsB(pArrayCoefficientsB);

}

void DataFilter::newDataFilter(int pFilterOrder, float *pArrayInputsX,
		float *pArrayCoefficientsB, float *pArrayInputsY,
		float *pArrayCoefficientsA) {

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
	setArrayInitialConditionsX(pArrayInputsX);
	setArrayCoefficientsB(pArrayCoefficientsB);
	setArrayInitialConditionsY(pArrayInputsY);
	setArrayCoefficientsA(pArrayCoefficientsA);
}

void DataFilter::newDataFilter(int pFilterOrder, float *pArrayInputsX,
		float *pArrayCoefficientsB, float *pArrayCoefficientsA) {

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

	for (Cont = 0; Cont < (FilterOrder - 1); Cont++) {
		_NumSum = MathOperation->sum(Cont, 0, ArrayCoefficientsA, ArrayInputsY,
				0);
		_NumX = ArrayInputsX[Cont + 1];
		_NumResult = _NumX - _NumSum;
		ArrayInputsY[Cont + 1] = _NumResult;
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
	for (Cont = 0; Cont < FilterOrder; Cont++) {
		ArrayCoefficientsA[Cont] = arrayCoefficientsA[Cont + 1]; ///CAMBIOOOOO
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
