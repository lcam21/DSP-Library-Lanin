/*
 * Filter.cpp
 *
 *  Created on: 08/05/2016
 *      Author: Luis
 */

#include "header/Filter.h"

Filter::Filter() {
	ContBuffer = 0;
}

Filter::~Filter() {
	chHeapFree(MathOperation);
	chHeapFree(InitialDataFilter);
}

void Filter::setInitialDataFilter(DataFilter *pInitialDataFilter) {
	// Create class that do the sum
	MathOperation = (FuntionsMath*) chHeapAlloc(NULL, sizeof(FuntionsMath));
	InitialDataFilter = pInitialDataFilter;
	ContBuffer = 0;
}

