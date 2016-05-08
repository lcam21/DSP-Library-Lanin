/*
 * FIR.h
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */

#ifndef HEADER_FIR_H_
#define HEADER_FIR_H_

#include "DataFilter.h"
#include "FuntionsMath.h"

class FIR {
public:

	/**
	 * @brief Method that implements the Direct Form I of a FIR Filter
	 * @param pDataFilter
	 */
	float directFormI(float pData);

	/**
	 * @brief Destruction of class
	 */
	virtual ~FIR();

	void setInitialDataFilter(DataFilter *pInitialDataFilter);
private:
	FuntionsMath *MathOperation;
	DataFilter *InitialDataFilter;
	int ContBuffer;
};

#endif /* HEADER_FIR_H_ */
