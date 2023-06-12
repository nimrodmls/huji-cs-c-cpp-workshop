#include "Activation.h"

Matrix activation::softmax(const Matrix& input)
{
	if (1 < input.get_cols())
	{
		throw std::length_error("Vector is a not single-column");
	}

	Matrix softmax_matrix(input.get_rows(), 1);
	
	float variable_sum = 0;
	for (
		int row_index = 0; row_index < input.get_rows(); row_index++)
	{
		variable_sum += std::exp(input[row_index]);
	}

	for (
		int row_index = 0; row_index < input.get_rows(); row_index++)
	{
		softmax_matrix[row_index] = 
			std::exp(input[row_index]) / variable_sum;
	}

	return softmax_matrix;
}

Matrix activation::relu(const Matrix& input)
{
	if (1 < input.get_cols())
	{
		throw std::length_error("Vector is a not single-column");
	}

	Matrix relu_matrix(input);

	for (int index = 0; 
		 index < input.get_cols() * input.get_rows(); 
		 index++)
	{
		if (relu_matrix[index] < 0)
		{
			relu_matrix[index] = 0;
		}
	}

	return relu_matrix;
}