/*
 * @class FIR
 * @brief Class for processing data for FIR filters
 * @file FIR.h
 * @autor Luis Alvarez Mata
 * @see Class Filter
 */

#include "Filter.h"

#ifndef HEADER_FIR_H_
#define HEADER_FIR_H_

class FIR: public Filter {
public:

	/**
	 * @brief Free memory
	 */
	virtual ~FIR();

	/**
	 * @brief Method that implements the Direct Form I of a FIR Filter
	 * @param pData Input data filter
	 * @return output data filtering
	 */
	float directFormI(float pData);

	/**
	 * @brief Method that implements the Direct Form I of a FIR Filter
	 * @param pData Input data filter
	 * @return output data filtering
	 */
	float directFormII(float pData);

};

#endif /* HEADER_FIR_H_ */
