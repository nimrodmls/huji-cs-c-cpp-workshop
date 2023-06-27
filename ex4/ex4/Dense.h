#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

/**
 * @class Dense
 * @brief Represents a layer of a neural network
 */
class Dense
{
public:

	/**
	* Constructs a layer.
	* @param weights - The weights matrix.
	* @param bias - The bias matrix.
	* @param activation_func - The activation to perform
	*/
	Dense(Matrix weights, 
		  Matrix bias, 
		  activation::ActivationPfn activation_func);

	// Explicitly defining behavior to prevent implicit behavior
	Dense() = delete;
	Dense(const Dense&) = delete;
	Dense& operator=(const Dense&) = delete;
	~Dense() = default;

	/**
	* Gets the weights matrix
	* @return The weights matrix.
	*/
	Matrix get_weights() const;

	/**
	* Gets the bias matrix
	* @return The bias matrix.
	*/
	Matrix get_bias() const;

	/**
	* Gets the activation function.
	* @return The activation function.
	*/
	activation::ActivationPfn get_activation() const;

	/**
	* Executes the activation function with the given input matrix.
	* @return The result matrix from the activation.
	*/
	Matrix operator()(const Matrix& input) const;

private:
	// Activation function
	const activation::ActivationPfn _activation_func;
	// Weights matrix
	const Matrix _weights;
	// Bias matrix
	const Matrix _bias;
};

#endif //DENSE_H
