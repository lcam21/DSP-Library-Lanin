/*
 * @class Filter
 * @brief Parent class containing the generic functions for filters
 * @file Filter.h
 * @autor Luis Alvarez Mata
 * @see Class FuntionsMath
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
	 * @param Input data filter
	 * @return output data filtering
	 */
	float directFormI(float pData);

	/**
	 * @brief Method that implements the Direct Form II of a FIR Filter
	 * @param pDataFilter
	 * @retur output data filtering
	 */
	float directFormII(float pData);

	/**
	 * @brief Method that implements for set the initial data of filter
	 * @param pInitialDataFilter - class input that get the data
	 */
	void setInitialDataFilter(DataFilter *pInitialDataFilter);

protected:
	FuntionsMath *MathOperation; /** @brief Pointer type MathOperation, for call sum operation */
	DataFilter *InitialDataFilter; /** @brief Pointer for set data filter */
	int ContBuffer; /** @brief Variable use with general count */
};

#endif /* FILTER_H_ */
