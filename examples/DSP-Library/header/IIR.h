/*
 * IIR.h
 *
 *  Created on: Apr 18, 2016
 *      Author: lcalvare
 */

#include "Filter.h"

#ifndef HEADER_IIR_H_
#define HEADER_IIR_H_

class IIR: public Filter {

public:

	/**
	 * @brief Method that implements the Direct Form I of a IIR Filter
	 * @param pData - input data for apply the filter
	 * @return variable with data filter
	 */
	float directFormI(float pData);

};

#endif /* HEADER_IIR_H_ */
