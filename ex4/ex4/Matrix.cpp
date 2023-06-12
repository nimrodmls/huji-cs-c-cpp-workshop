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
	Matrix(matrix.get_rows(), matrix.get_cols())
{
	_copy_matrix(matrix);
}

Matrix::~Matrix()
{
	_destroy_matrix(_rmatrix, _rows);
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
	auto new_matrix = _allocate_matrix(_columns, _rows);
	for (int row_index = 0; row_index < _rows; row_index++)
	{
		for (int column_index = 0;
			column_index < _columns;
			column_index++)
		{
			new_matrix[column_index][row_index] =
				_rmatrix[row_index][column_index];
		}
	}

	// Transferring ownership over to the new transposed matrix
	_destroy_matrix(_rmatrix, _rows);
	auto temp_new_cols = _rows;
	_rows = _columns;
	_columns = temp_new_cols;
	_rmatrix = new_matrix;

	return *this;
}

Matrix& Matrix::vectorize()
{
	int new_matrix_index = 0;
	auto new_matrix = _allocate_matrix(_rows * _columns, 1);
	for (int row_index = 0; row_index < _rows; row_index++)
	{
		for (int column_index = 0;
			column_index < _columns;
			column_index++)
		{
			new_matrix[new_matrix_index][0] =
				_rmatrix[row_index][column_index];
			new_matrix_index++;
		}
	}

	// Transferring ownership over to the new vectorized matrix
	_destroy_matrix(_rmatrix, _rows);
	_rows = _columns * _rows;
	_columns = 1;
	_rmatrix = new_matrix;

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
			std::cout << _rmatrix[row_index][column_index] << " ";
		}

		std::cout << std::endl;
	}
}

Matrix Matrix::dot(Matrix& in)
{
	if (!_validate_dimensions(in))
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
				_rmatrix[row_index][column_index];
		}
	}

	return dot_matrix;
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

float& Matrix::operator()(int row, int col)
{
	return _rmatrix[row][col];
}

float& Matrix::operator[](int index)
{
	float a = 0.0f;
	return a;
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
		delete[] matrix[row_index];
	}

	delete[] matrix;
}

bool Matrix::_validate_dimensions(const Matrix& other) const
{
	if ((_columns != other.get_cols()) || 
		(_rows != other.get_rows()))
	{
		return false;
	}
	return true;
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

std::ostream& operator<<(std::ostream& os, Matrix& obj)
{
	for (int row_index = 0; row_index < obj._rows; row_index++)
	{
		for (int column_index = 0; 
			 column_index < obj._columns; 
			 column_index++)
		{
			if (0.1 < obj(row_index, column_index))
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
			is >> obj._rmatrix[row_index][column_index];
		}
	}

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
