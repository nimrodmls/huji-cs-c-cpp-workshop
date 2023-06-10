#include "Matrix.h"

Matrix::Matrix() :
	Matrix(1, 1)
{}

Matrix::Matrix(int rows, int cols) :
	_rows(rows), 
	_columns(cols), 
	_rmatrix(_allocate_matrix(rows, cols))
{
	if ((0 >= _rows) || (0 >= _columns))
	{
		throw std::length_error("Invalid Matrix Dimensions");
	}
}

Matrix::Matrix(Matrix& matrix) :
	_rows(matrix.get_rows()), _columns(matrix.get_cols())
{
	_copy_matrix(matrix);
}

Matrix::~Matrix()
{
	_destroy_matrix(_rmatrix, _rows);
}

int Matrix::get_rows()
{
	return _rows;
}

int Matrix::get_cols()
{
	return _columns;
}

Matrix& Matrix::transpose()
{
	auto new_matrix = _allocate_matrix(_columns, _rows);
	for (int row_index = 0; row_index < _columns; row_index++)
	{
		for (int column_index = 0;
			column_index < _rows;
			column_index++)
		{
			new_matrix[row_index][column_index] =
				_rmatrix[column_index][row_index];
		}
	}
	return *this;
}

Matrix& Matrix::vectorize()
{
	Matrix a;
	return a;
}

void Matrix::plain_print()
{
}

Matrix& Matrix::dot(Matrix& in)
{
	Matrix a;
	return a;
}

float Matrix::norm()
{
	return 0.0f;
}

Matrix& Matrix::rref()
{
	Matrix a;
	return a;
}

int Matrix::argmax()
{
	return 0;
}

float Matrix::sum()
{
	return 0.0f;
}

Matrix& Matrix::operator+=(const Matrix& rhs)
{
	Matrix a;
	return a;
}

Matrix& Matrix::operator=(const Matrix& rhs)
{
	Matrix a;
	return a;
}

float Matrix::operator()(int row, int col)
{
	return 0.0f;
}

float Matrix::operator[](int index)
{
	return 0.0f;
}

float** Matrix::_allocate_matrix(int rows, int columns)
{
	float** matrix = new float* [rows];
	for (int index = 0; index < rows; index++)
	{
		matrix[index] = new float[columns];
	}

	return matrix;
}

void Matrix::_destroy_matrix(float** matrix, int rows)
{
	for (int row_index = 0; row_index < rows; row_index++)
	{
		delete matrix[row_index];
	}

	delete matrix;
}

void Matrix::_copy_matrix(Matrix& source)
{
	for (int row_index = 0; row_index < _rows; row_index++)
	{
		for (int column_index = 0;
			 column_index < _columns;
			 column_index++)
		{
			_rmatrix[row_index][column_index] = 
				source(row_index, column_index);
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Matrix& obj)
{
	return os;
}

std::istream& operator>>(std::istream& is, const Matrix& obj)
{
	return is;
}

Matrix operator+(const Matrix& rhs)
{
	return Matrix();
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
	return Matrix();
}

Matrix operator*(float scalar, const Matrix& rhs)
{
	return Matrix();
}

Matrix operator*(const Matrix& lhs, float scalar)
{
	return Matrix();
}
