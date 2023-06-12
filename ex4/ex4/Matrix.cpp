#include "Matrix.h"

Matrix::Matrix() :
	Matrix(1, 1)
{}

Matrix::Matrix(int rows, int cols) :
	_rows(rows), 
	_columns(cols), 
	_rmatrix(new float[rows * cols]())
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
	auto new_matrix = new float[_columns * _rows]();
	for (int row_index = 0; row_index < _rows; row_index++)
	{
		for (int column_index = 0;
			column_index < _columns;
			column_index++)
		{
			new_matrix[
				_2d_index_to_1d(column_index, row_index, _rows)
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
				operator()(row_index, column_index);
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
	_rows = rhs.get_rows();
	_columns = rhs.get_cols();
	_rmatrix = new float[_rows * _columns]();
	_copy_matrix(rhs);

	return *this;
}

float Matrix::operator()(int row, int col) const
{
	return _rmatrix[_2d_index_to_1d(row, col, _columns)];
}

float& Matrix::operator()(int row, int col)
{
	return _rmatrix[_2d_index_to_1d(row, col, _columns)];
}

float Matrix::operator[](int index) const
{
	return _rmatrix[index];
}

float& Matrix::operator[](int index)
{
	return _rmatrix[index];
}

int Matrix::_2d_index_to_1d(int row, int col, int col_count)
{
	return (row * col_count) + col;
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

void Matrix::_copy_matrix(const Matrix& source)
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
			is >> obj(row_index, column_index);
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
