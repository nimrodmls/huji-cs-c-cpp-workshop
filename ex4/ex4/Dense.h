#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

class Dense
{
public:
	Dense(Matrix weights, 
		  Matrix bias, 
		  activation::ActivationPfn activation_func);

	// Explicitly defining behavior to prevent implicit behavior
	Dense() = delete;
	Dense(Dense&) = delete;
	Dense& operator=(const Dense&) = delete;
	~Dense() = default;

	Matrix get_weights();
	Matrix get_bias();
	activation::ActivationPfn get_activation();

	// Activation
	Matrix operator()(const Matrix& input);

private:
	activation::ActivationPfn _activation_func;
	Matrix _weights;
	Matrix _bias;
};

#endif //DENSE_H
