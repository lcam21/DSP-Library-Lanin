/*
 * @class DataFilter
 * @brief Class responsible for containing the filter data
 * @file DataFilter.h
 * @autor Luis Alvarez Mata
 */

#include <ch.h>
#include <hal.h>
#include "FuntionsMath.h"

#ifndef DATAFILTER_H_
#define DATAFILTER_H_

#define BUFFER_SIZE 10 /**< Buffer size of create array */

class DataFilter {
public:

	/**
	 * @brief Constructor of data for filter FIR
	 * @param pFilterOrder Filter Order
	 * @param pArrayInputsX Initial conditions for X
	 * @param pArrayCoefficientsB Coefficients transfer function (zeros)
	 */
	void newDataFilter(int pFilterOrder, float *pArrayInputsX,
			float *pArrayCoefficientsB);

	/**
	 * @brief Constructor of data for filter IIR using the form direct I
	 * @param pFilterOrder Filter Order
	 * @param pArrayInputsX Initial conditions for X (input)
	 * @param pArrayCoefficientsB Coefficients transfer function (zeros)
	 * @param pArrayInputsY Initial condition for y (output)
	 * @param pArrayCoefficientsA Coefficients transfer function (polos)
	 */
	void newDataFilter(int pFilterOrder, float *pArrayInputsX,
			float *pArrayCoefficientsB, float *pArrayInputsY,
			float *pArrayCoefficientsA);

	/**
	 * @brief Constructor of data for filter IIR using the form direct II
	 * @param pFilterOrder Filter Order
	 * @param pArrayInputsX Initial conditions for X (input)
	 * @param pArrayCoefficientsB Coefficients transfer function (zeros)
	 * @param pArrayCoefficientsA Coefficients transfer function (polos)
	 */
	void newDataFilter(int pFilterOrder, float *pArrayInputsX,
			float *pArrayCoefficientsB, float *pArrayCoefficientsA);

	/**
	 * @brief Destroyer of class
	 */
	virtual ~DataFilter();

	/**
	 * @brief Move the data of array and add pData in the position [n-1]
	 * @param pArray Array pointer to move
	 */
	void moveArray(float *pArray);

	/**
	 * @brief Modify the array aux for add the initial conditions
	 */
	void createArrayAux();

	/**
	 * @brief Return the pointer of array that have the coefficients
	 * @return Pointer double
	 */
	float* getArrayCoefficientsB() const;

	/**
	 * @brief Set the value of array of coefficients
	 */
	void setArrayCoefficientsB(float* pArrayCoefficients);

	/**
	 * @brief Set the value of array of initial conditions
	 */
	void setArrayInitialConditionsX(float* pArrayInitialConditions);

	/**
	 * @brief Return the pointer of array that have the inputs
	 * @return Pointer with the inputs
	 */
	float* getArrayInputsX() const;

	/**
	 * @brief Set the value of array of initial inputs - Only for filter FIR
	 */
	void setArrayInputsX(float* pArrayInputs, int pSizeInputs);

	/**
	 * @brief Return order of filter
	 * @return Integer variable with contains the value of filter order
	 */
	int getFilterOrder() const;

	/**
	 * @brief Set the value of filter order
	 */
	void setFilterOrder(int pFilterOrder);

	/**
	 * @brief Get the value of coefficients A
	 * @return Pointer with the array of coefficients of polos
	 */
	float* getArrayCoefficientsA() const;

	/**
	 * @brief Set the value of coefficients A
	 */
	void setArrayCoefficientsA(float* arrayCoefficientsA);

	/**
	 * @brief Set the value of InitialConditions Y
	 */
	void setArrayInitialConditionsY(float* arrayInitialConditionsY);

	/**
	 * @brief Get the value of Inputs Y
	 * @return Pointer of array inputs Y
	 */
	float* getArrayInputsY() const;

private:
	//Variable generic of filter
	int FilterOrder; /** @brief Number of the order of filter*/

	// Variable of input data in axis X
	float *ArrayCoefficientsB; /** @brief Array that contains the coefficients b sub k*/
	float *ArrayInputsX; /** @brief Array that contains the inputs x[n-k] */

	//Variable of input data in axis Y
	float *ArrayCoefficientsA; /** @brief Array that contains the coefficients a sub k */
	float *ArrayInputsY; /** @brief Array that contains the inputs y[n-k]  //For the from direct II is used with auxiliary */

	//Variable of output data
	float Result; /** @brief Get the result of filter - For IIR */

	int Cont; /** @brief generic cont for use in FOR */

	FuntionsMath *MathOperation; /** @brief Pointer of MathOperation using in create initial conditions */

};

#endif /* DATAFILTER_H_ */
