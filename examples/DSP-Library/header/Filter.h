/*
 * Filter.h
 *
 *  Created on: 08/05/2016
 *      Author: Luis
 */

#include <ch.h>
#include <hal.h>

#include "DataFilter.h"
#include "FuntionsMath.h"

#ifndef FILTER_H_
#define FILTER_H_

class Filter {
public:

	/**
	 * @brief Free memory
	 */
	virtual ~Filter();

	/**
	 * @brief Method that implements the Direct Form I of a FIR Filter
	 * @param pDataFilter
	 */
	float directFormI(float pData);

	/**
	 * @brief Method that implements the Direct Form II of a FIR Filter
	 * @param pDataFilter
	 */
	float directFormII(float pData);

	/**
	 * @brief Method that implements for set the initial data of filter
	 * @param pInitialDataFilter - class input that get the data
	 */
	void setInitialDataFilter(DataFilter *pInitialDataFilter);

protected:
	FuntionsMath *MathOperation;
	DataFilter *InitialDataFilter;
	int ContBuffer;
};

#endif /* FILTER_H_ */
