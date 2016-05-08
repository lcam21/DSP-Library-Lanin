/*
 * FIR.h
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */

#include "Filter.h"

#ifndef HEADER_FIR_H_
#define HEADER_FIR_H_

class FIR : public Filter {
public:

	/**
	 * @brief Method that implements the Direct Form I of a FIR Filter
	 * @param pDataFilter
	 * @return variable with get the filter data
	 */
	float directFormI(float pData);

};

#endif /* HEADER_FIR_H_ */
