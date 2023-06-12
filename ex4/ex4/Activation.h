#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

namespace activation
{
	Matrix softmax(const Matrix& input);

	Matrix relu(const Matrix& input);

	using ActivationPfn = decltype(&relu);
}

#endif //ACTIVATION_H