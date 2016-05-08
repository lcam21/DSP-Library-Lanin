/*
 * DataFilter.h
 *
 *  Created on: Apr 17, 2016
 *      Author: luis
 */

#ifndef DATAFILTER_H_
#define DATAFILTER_H_

#define BUFFER_SIZE 10

class DataFilter {
public:
	/**
	 * @brief Constructor of class
	 * @param pFilterOrder number of filter order
	 */
	void newDataFilter(int pFilterOrder);

	/**
	 * @brief Destructor of class
	 */
	virtual ~DataFilter();

	/**
	 * @brief modify the array of input of add the initial conditions for filter
	 */
	void createArrayInput();

	/**
	 * @brief modify the array of input of add the initial conditions
	 */
	void createArrayInputY();

	/**
	 * @brief move the data of array and add pData in te position [n-1]
	 */
	void moveArray(float *pArray);

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

private:
	//Variable generic of filter
	int FilterOrder; //Number of the order of filter

	// Variable of input data in axis X
	float *ArrayInitialConditionsX; //Array that contains the initial conditions of x[n-k]
	float *ArrayCoefficientsB; //Array that contains the coefficients b sub k
	float *ArrayInputsX; //Array that contains the inputs x[n-k]

	//Variable of input data in axis Y
	float *ArrayInitialConditionsY; //Array that contains the initial conditions of y[n-k]
	float *ArrayCoefficientsA; //Array that contains the coefficients a sub k
	float *ArrayInputsY; //Array that contains the inputs y[n-k]

	//Variable of output data
	float Result; //Get the result of filter - For IIR

	int Cont; //generic cont for use in FOR

};

#endif /* DATAFILTER_H_ */
