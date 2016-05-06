/*
 * FIR.h
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */

#ifndef HEADER_FIR_H_
#define HEADER_FIR_H_

#include "DataFilter.h"

class FIR {
public:

	/**
	 * @brief Construction of class
	 */
	FIR();

	/**
	 * @brief Method that implements the Direct Form I of a FIR Filter
	 * @param pDataFilter
	 */
	DataFilter* directFormI(DataFilter *pDataFilter);

	/**
	 * @brief Destruction of class
	 */
	virtual ~FIR();
private:

};

#endif /* HEADER_FIR_H_ */
