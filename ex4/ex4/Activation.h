#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

namespace activation
{
	Matrix softmax(const Matrix& input);

	Matrix relu(const Matrix& input);

	using ActivationPfn = decltype(&relu);
}

#endif //ACTIVATION_H