#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

namespace activation
{
	/**
	* Applies a Softmax filter to a matrix. 
	* @param input - The matrix to apply to.
	* @return The matrix after application
	*/
	Matrix softmax(const Matrix& input);

	/**
	* Applies a relu filter to a matrix.
	* @param input - The matrix to apply to.
	* @return The matrix after application
	*/
	Matrix relu(const Matrix& input);

	// Generic activation function pointer definition
	using ActivationPfn = decltype(&relu);
}

#endif //ACTIVATION_H