#include <cmath>

#include "Matrix.h"

constexpr float quadratic_power = 2.0F;
constexpr float matrix_value_threshold = 0.1F;

Matrix::Matrix() :
	Matrix(1, 1)
{}

Matrix::Matrix(int rows, int cols) :
	_rmatrix(new float[rows * cols]()),
	_rows(rows), 
	_columns(cols)
{
	if ((0 >= _rows) || (0 >= _columns))
	{
		throw std::length_error("Invalid Matrix Dimensions");
	}
}

Matrix::Matrix(const Matrix& matrix) :
	Matrix(matrix.get_rows(), matrix.get_cols())
{
	copy_matrix(matrix);
}

Matrix::~Matrix()
{
	delete[] _rmatrix;
}

int Matrix::get_rows() const
{
	return _rows;
}

int Matrix::get_cols() const
{
	return _columns;
}

Matrix& Matrix::transpose()
{
	float* new_matrix = new float[_columns * _rows]();
	for (int row_index = 0; row_index < _rows; row_index++)
	{
		for (int column_index = 0;
			column_index < _columns;
			column_index++)
		{
			new_matrix[
				coord_to_index(column_index, row_index, _rows)
			] = operator()(row_index, column_index);
		}
	}

	// Transferring ownership over to the new transposed matrix
	delete[] _rmatrix;
	auto temp_new_cols = _rows;
	_rows = _columns;
	_columns = temp_new_cols;
	_rmatrix = new_matrix;

	return *this;
}

Matrix& Matrix::vectorize()
{
	_rows = _columns * _rows;
	_columns = 1;
	return *this;
}

void Matrix::plain_print()
{
	for (int row_index = 0; row_index < _rows; row_index++)
	{
		for (int column_index = 0;
			column_index < _columns;
			column_index++)
		{
			std::cout << operator()(row_index, column_index) << " ";
		}

		std::cout << std::endl;
	}
}

Matrix Matrix::dot(Matrix& in)
{
	if (!validate_dimensions(in))
	{
		throw std::length_error("Dimensions incompatible");
	}
	
	Matrix dot_matrix(_rows, _columns);
	for (int row_index = 0; row_index < _rows; row_index++)
	{
		for (int column_index = 0;
			column_index < _columns;
			column_index++)
		{
			
			dot_matrix(row_index, column_index) =
				in(row_index, column_index) * 
				operator()(row_index, column_index);
		}
	}

	return dot_matrix;
}

float Matrix::norm()
{
	float quadratic_sum = 0;
	for (int index = 0; index < _rows * _columns; index++)
	{
		quadratic_sum += std::pow(_rmatrix[index], quadratic_power);
	}

	return std::sqrt(quadratic_sum);
}

Matrix Matrix::rref()
{
	Matrix a;
	return a;
}

int Matrix::argmax()
{
	int current_max = 0;
	for (int index = 0; index < _rows * _columns; index++)
	{
		if (_rmatrix[current_max] < _rmatrix[index])
		{
			current_max = index;
		}
	}

	return current_max;
}

float Matrix::sum() const
{
	float matrix_sum = 0;
	for (int index = 0; index < _rows * _columns; index++)
	{
		matrix_sum += _rmatrix[index];
	}

	return matrix_sum;
}

Matrix& Matrix::operator+=(const Matrix& rhs)
{
	if (!validate_dimensions(rhs))
	{
		throw std::length_error("Dimensions incompatible");
	}

	for (int index = 0; index < _rows * _columns; index++)
	{
		_rmatrix[index] += rhs[index];
	}

	return *this;
}

Matrix& Matrix::operator=(const Matrix& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	_rows = rhs.get_rows();
	_columns = rhs.get_cols();
	_rmatrix = new float[_rows * _columns]();
	copy_matrix(rhs);

	return *this;
}

float Matrix::operator()(int row, int col) const
{
	int raw_index = coord_to_index(row, col, _columns);
	if (is_out_of_range(raw_index, _rows * _columns))
	{
		throw std::out_of_range("Invalid matrix index");
	}

	return _rmatrix[raw_index];
}

float& Matrix::operator()(int row, int col)
{
	int raw_index = coord_to_index(row, col, _columns);
	if (is_out_of_range(raw_index, _rows * _columns))
	{
		throw std::out_of_range("Invalid matrix index");
	}

	return _rmatrix[raw_index];
}

float Matrix::operator[](int index) const
{
	if (is_out_of_range(index, _rows * _columns))
	{
		throw std::out_of_range("Invalid matrix index");
	}

	return _rmatrix[index];
}

float& Matrix::operator[](int index)
{
	if (is_out_of_range(index, _rows * _columns))
	{
		throw std::out_of_range("Invalid matrix index");
	}

	return _rmatrix[index];
}

int Matrix::coord_to_index(int row, int col, int col_count)
{
	return (row * col_count) + col;
}

bool Matrix::is_out_of_range(int index, int size)
{
	return (0 > index) || (index >= size);
}

bool Matrix::validate_dimensions(const Matrix& other) const
{
	return (_columns == other.get_cols()) || 
		   (_rows == other.get_rows());
}

void Matrix::copy_matrix(const Matrix& source)
{
	for (int row_index = 0; row_index < _rows; row_index++)
	{
		for (int column_index = 0;
			 column_index < _columns;
			 column_index++)
		{
			operator()(row_index, column_index) =
				source(row_index, column_index);
		}
	}
}

std::ostream& operator<<(std::ostream& os, Matrix& obj)
{
	for (int row_index = 0; row_index < obj._rows; row_index++)
	{
		for (int column_index = 0; 
			 column_index < obj._columns; 
			 column_index++)
		{
			if (matrix_value_threshold < 
				obj(row_index, column_index))
			{
				os << "**";
			}
			else
			{
				os << "  ";
			}
		}

		os << std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, Matrix& obj)
{
	for (int row_index = 0; row_index < obj._rows; row_index++)
	{
		for (int column_index = 0;
			column_index < obj._columns;
			column_index++)
		{
			float input = 0;
			is.read(reinterpret_cast<char*>(&input), sizeof(input));

			if (!is)
			{
				throw std::runtime_error("Failed to read sufficient data");
			}

			obj(row_index, column_index) = input;
		}
	}

	return is;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
	Matrix addition_matrix(lhs);
	addition_matrix += rhs;
	return addition_matrix;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
	if (lhs.get_cols() != rhs.get_rows())
	{
		throw std::length_error("Dimensions incompatible");
	}

	Matrix mult_matrix(lhs.get_rows(), rhs.get_cols());

	for (int lhs_index = 0; lhs_index < lhs.get_rows(); lhs_index++)
	{
		for (int rhs_index = 0; 
			 rhs_index < rhs.get_cols(); 
			 rhs_index++)
		{
			// At this point, we're modifying the i,j index
			// of the multiplication matrix
			float mult_sum = 0;

			// Now for the actual multiplication
			for (int mult_index = 0; 
				 mult_index < lhs.get_cols(); 
				 mult_index++)
			{
				mult_sum += 
					lhs(lhs_index, mult_index) * 
					rhs(mult_index, rhs_index);
			}

			mult_matrix(lhs_index, rhs_index) = mult_sum;
		}
	}
	
	return mult_matrix;
}

Matrix operator*(float scalar, const Matrix& rhs)
{
	return operator*(rhs, scalar);
}

Matrix operator*(const Matrix& lhs, float scalar)
{
	Matrix mult_matrix(lhs);
	for (int index = 0; index < lhs._rows * lhs._columns; index++)
	{
		mult_matrix._rmatrix[index] *= scalar;
	}

	return mult_matrix;
}
