/*
 * DataFilter.h
 *
 *  Created on: Apr 17, 2016
 *      Author: luis
 */

#include <ch.h>
#include <hal.h>
#include "FuntionsMath.h"

#ifndef DATAFILTER_H_
#define DATAFILTER_H_

#define BUFFER_SIZE 10

typedef int typeOfDirectForm;

class DataFilter {
public:

	/**
	 * @brief Constructor of data for filter FIR
	 */
	void newDataFilter(int pFilterOrder, float *pArrayInputsX,
			float *pArrayCoefficientsB, typeOfDirectForm pTypeOfDirectForm);

	/**
	 * @brief Constructor of data for filter IIR using the form direct I
	 */
	void newDataFilter(int pFilterOrder, float *pArrayInputsX,
			float *pArrayCoefficientsB, float *pArrayInputsY,
			float *pArrayCoefficientsA, typeOfDirectForm pTypeOfDirectForm);

	/**
	 * @brief Constructor of data for filter IIR using the form direct II
	 */
	void newDataFilter(int pFilterOrder, float *pArrayInputsX,
			float *pArrayCoefficientsB, float *pArrayCoefficientsA,
			typeOfDirectForm pTypeOfDirectForm);

	/**
	 * @brief Destructor of class
	 */
	virtual ~DataFilter();

	/**
	 * @brief move the data of array and add pData in te position [n-1]
	 */
	void moveArray(float *pArray);

	/**
	 * @brief modify the array aux for add the initial conditions
	 */
	void createArrayAux();

	/**
	 * @brief return the pointer of array that have the coefficients
	 * @return pointer double
	 */
	float* getArrayCoefficientsB() const;

	/**
	 * @brief set the value of array of coefficients
	 */
	void setArrayCoefficientsB(float* pArrayCoefficients);

	/**
	 * @brief return the pointer of array that have the initial conditions
	 * @return pointer double
	 */
	float* getArrayInitialConditionsX() const;

	/**
	 * @brief set the value of array of initial conditions
	 */
	void setArrayInitialConditionsX(float* pArrayInitialConditions);

	/**
	 * @brief return the pointer of array that have the inputs
	 * @return pointer double
	 */
	float* getArrayInputsX() const;

	/**
	 * @brief set the value of array of initial inputs - Only for filter FIR
	 */
	void setArrayInputsX(float* pArrayInputs, int pSizeInputs);

	/**
	 * @brief return order of filter
	 * @return integer variable with contains the value of filter order
	 */
	int getFilterOrder() const;

	/**
	 * @brief set the value of filter order
	 */
	void setFilterOrder(int pFilterOrder);

	/**
	 * @brief get the value of coefficients A
	 */
	float* getArrayCoefficientsA() const;

	/**
	 * @brief set the value of coefficients A
	 */
	void setArrayCoefficientsA(float* arrayCoefficientsA);

	/**
	 * @brief get the value of InitialConditions Y
	 */
	float* getArrayInitialConditionsY() const;

	/**
	 * @brief set the value of InitialConditions Y
	 */
	void setArrayInitialConditionsY(float* arrayInitialConditionsY);

	/**
	 * @brief get the value of Inputs Y
	 */
	float* getArrayInputsY() const;

	int getTypeOfDirectForm() const;

	void setTypeOfDirectForm(int typeOfDirectForm);

private:
	//Variable generic of filter
	int FilterOrder; //Number of the order of filter
	int TypeOfDirectForm; //Type of form filter

	// Variable of input data in axis X
	float *ArrayCoefficientsB; //Array that contains the coefficients b sub k
	float *ArrayInputsX; //Array that contains the inputs x[n-k]

	//Variable of input data in axis Y
	float *ArrayCoefficientsA; //Array that contains the coefficients a sub k
	float *ArrayInputsY; //Array that contains the inputs y[n-k]  //For the from direct II is used with auxiliary

	//Variable of output data
	float Result; //Get the result of filter - For IIR

	int Cont; //generic cont for use in FOR

	FuntionsMath *MathOperation;

};

#endif /* DATAFILTER_H_ */
