#include "Dense.h"

// See documentation at header file
Dense::Dense(Matrix weights,
			 Matrix bias,
			 activation::ActivationPfn activation_func) :
	_activation_func(activation_func), _weights(weights), _bias(bias)
{}

// See documentation at header file
Matrix Dense::get_weights() const
{
	return _weights;
}

// See documentation at header file
Matrix Dense::get_bias() const
{
	return _bias;
}

// See documentation at header file
activation::ActivationPfn Dense::get_activation() const
{
	return _activation_func;
}

// See documentation at header file
Matrix Dense::operator()(const Matrix& input) const
{
	return _activation_func((_weights * input) + _bias);
}
