/*
 * FuntionsMath.h
 *
 *  Created on: Apr 13, 2016
 *      Author: luis
 */

#ifndef HEADER_FUNTIONSMATH_H_
#define HEADER_FUNTIONSMATH_H_

class FuntionsMath {
public:

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

private:

	int Cont; /** @brief Variable use with general count */

};

#endif /* HEADER_FUNTIONSMATH_H_ */
