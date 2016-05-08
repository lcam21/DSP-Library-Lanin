/*
 * IIR.h
 *
 *  Created on: Apr 18, 2016
 *      Author: lcalvare
 */

#ifndef HEADER_IIR_H_
#define HEADER_IIR_H_

#include "DataFilter.h"
#include "FuntionsMath.h"

class IIR {
public:

	/**
	 * @brief Method that implements the Direct Form I of a IIR Filter
	 * @param pData - input data for apply the filter
	 * @return variable with data filter
	 */
	float directFormI(float pData);

	/**
	 * @brief Destruction of class
	 */
	virtual ~IIR();

	/**
	 * @brief Method that implements for set the initial data of filter
	 * @param pInitialDataFilter - class input that get the data
	 */
	void setInitialDataFilter(DataFilter *pInitialDataFilter);

private:
	FuntionsMath *MathOperation;
	DataFilter *InitialDataFilter;
	int ContBuffer;
};

#endif /* HEADER_IIR_H_ */
