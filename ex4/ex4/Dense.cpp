#include "Dense.h"

Dense::Dense(Matrix weights,
			 Matrix bias,
			 activation::ActivationPfn activation_func) :
	_activation_func(activation_func), _weights(weights), _bias(bias)
{}

Matrix Dense::get_weights() const
{
	return _weights;
}

Matrix Dense::get_bias() const
{
	return _bias;
}

activation::ActivationPfn Dense::get_activation() const
{
	return _activation_func;
}

// Activation
Matrix Dense::operator()(const Matrix& input) const
{
	return _activation_func((_weights * input) + _bias);
}
