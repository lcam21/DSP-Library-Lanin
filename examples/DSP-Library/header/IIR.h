/*
 * @class IIR
 * @brief Class for processing data for IIR filters
 * @file IIR.h
 * @autor Luis Alvarez Mata
 * @see Class Filter
 */

#include "Filter.h"

#ifndef HEADER_IIR_H_
#define HEADER_IIR_H_

class IIR: public Filter {

public:

	/**
	 * @brief Method that implements the Direct Form I of a IIR Filter
	 * @param pData - input data for apply the filter
	 * @return output data filtering
	 */
	float directFormI(float pData);

	/**
	 * @brief Method that implements the Direct Form II of a IIR Filter
	 * @param pData - input data for apply the filter
	 * @return output data filtering
	 */
	float directFormII(float pData);

	/**
	 * @brief Free memory
	 */
	virtual ~IIR();

};

#endif /* HEADER_IIR_H_ */
