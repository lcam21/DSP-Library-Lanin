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
	 * @brief modify the array of input of add the initial conditions for filter FIR
	 */
	void createArrayInputFIR();

	/**
	 * @brief modify the array of input of add the initial conditions for filter IIR
	 */
	void createArrayInputIIR();

	/**
	 * @brief modify the array of input of add the initial conditions
	 */
	void createArrayInputY();

	/**
	 * @brief move the data of array and add pData in te position [n-1]
	 */
	void moveArray(float *pArray);

	void copyArray(float *pArray1, float *pArray2);

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
	 * @brief set the value of array of initial inputs
	 */
	void setArrayInputsX(float* pArrayInputs);

	/**
	 * @brief return the pointer of array that have the interval of output
	 * @return pointer double
	 */
	int* getArrayIntervalOutput() const;

	/**
	 * @brief set the value of array of interval of output
	 */
	void setArrayIntervalOutput(int* pArrayIntervalOutput);

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
	 * @brief return the pointer of array that have the array result
	 * @return pointer float
	 */
	float* getArrayResult() const;

	/**
	 * @brief set the value of array of result
	 */
	void setArrayResult(float* arrayResult);

	/**
	 * @brief return the number of output
	 * @return integer variable with contains the number of output
	 */
	int getNumbOutput() const;

	/**
	 * @brief set the value of number of output
	 */
	void setNumbOutput(int numbOutput);
	float* getArrayCoefficientsA() const;
	void setArrayCoefficientsA(float* arrayCoefficientsA);
	float* getArrayInitialConditionsY() const;
	void setArrayInitialConditionsY(float* arrayInitialConditionsY);
	float* getArrayInputsY() const;
	float* getBuffer1() const;
	void setBuffer1(float* buffer1);
	float* getBuffer2() const;
	void setBuffer2(float* buffer2);
	bool isBufferReady() const;
	void setBufferReady(bool bufferReady);

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

	//Variable of output data in axis Y
	float *ArrayResult; //Array that contains the end result y[n] - For FIR
	int *ArrayIntervalOutput; //Array that contains the interval of output - For FIR
	int NumbOutput; //It get of ArrayIntervalOutput- For FIR
	float Result; //Get the result of filter - For IIR

	int Cont; //generic cont for use in FOR

	/**
	 * @brief funtion of get the number of output
	 * @param pArrayOfInterval double pointer array
	 */
	void numbOutput(int *pArrayOfInterval);

};

#endif /* DATAFILTER_H_ */
