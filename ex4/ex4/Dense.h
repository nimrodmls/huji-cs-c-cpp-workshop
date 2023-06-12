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

	Matrix get_weights() const;
	Matrix get_bias() const;
	activation::ActivationPfn get_activation() const;

	// Activation
	Matrix operator()(const Matrix& input) const;

private:
	const activation::ActivationPfn _activation_func;
	const Matrix _weights;
	const Matrix _bias;
};

#endif //DENSE_H
