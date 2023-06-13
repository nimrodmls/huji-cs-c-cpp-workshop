#include <cmath>

#include "Activation.h"

// See documentation at header file
Matrix activation::softmax(const Matrix& input)
{
	Matrix softmax_matrix(input);

	float variable_sum = 0;
	// Calculating the expontential sum of the matrix first
	for (
		int row_index = 0;
		row_index < input.get_rows() * input.get_cols();
		row_index++)
	{
		variable_sum += std::exp(input[row_index]);
	}

	for (int column_index = 0; 
		 column_index < input.get_cols(); 
		 column_index++)
	{
		for (
			int row_index = 0; 
			row_index < input.get_rows(); 
			row_index++)
		{
			softmax_matrix(row_index, column_index) =
				std::exp(input(row_index, column_index)) / variable_sum;
		}
	}

	return softmax_matrix;
}

// See documentation at header file
Matrix activation::relu(const Matrix& input)
{
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
