/*
 * IIR.h
 *
 *  Created on: Apr 18, 2016
 *      Author: lcalvare
 */

#ifndef HEADER_IIR_H_
#define HEADER_IIR_H_

#include "DataFilter.h"

class IIR {
public:

	/**
	 * @brief Construction of class
	 */
	IIR();

	/**
	 * @brief Method that implements the Direct Form I of a FIR Filter
	 * @param pDataFilter
	 */
	void directFormI(DataFilter *pDataFilter);

	/**
	 * @brief Method responsible for carrying out a special sum
	 * @param pUpperLimit upper limit of the sum
	 * @param pLowerLimit lower limit of the sum
	 * @param pCoefficients array of filter coefficients
	 * @param pArray array containing the values to iterated
	 * @param pCont actual position
	 * @return variable type of float, it content the result of sum
	 */
	float sum(int pUpperLimit, int pLowerLimit, float* pCoefficients,
			float* arrayX, int pCont);

	/**
	 * @brief Destruction of class
	 */
	virtual ~IIR();

};

#endif /* HEADER_IIR_H_ */
