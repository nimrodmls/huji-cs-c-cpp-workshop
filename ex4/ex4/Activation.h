#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

namespace activation
{
	Matrix softmax(Matrix& input);

	Matrix relu(Matrix& input);

	using ActivationPfn = decltype(relu);
}

#endif //ACTIVATION_H