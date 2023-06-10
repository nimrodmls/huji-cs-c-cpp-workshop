// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
	int rows, cols;
} matrix_dims;

class Matrix
{
public:
	Matrix() = default;
	Matrix(int rows, int cols);
	Matrix(Matrix& matrix);
	~Matrix();

	int get_rows();
	int get_cols();
	Matrix& transpose();
	Matrix& vectorize();
	void plain_print();
	Matrix& dot(Matrix& in);
	float norm();
	Matrix& rref();
	int argmax();
	float sum();

	
	// Addition operators
	Matrix& operator+=(const Matrix& rhs);
	friend Matrix operator+(const Matrix& rhs);

	// Multiplication operators
	friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
	friend Matrix operator*(float scalar, const Matrix& rhs);
	friend Matrix operator*(const Matrix& lhs, float scalar);

	// Assignment operator
	Matrix& operator=(const Matrix& rhs);

	// Access operators
	float operator()(int row, int col);
	float operator[](int index);

	// Stream operators
	std::ostream& operator<<(std::ostream& os, const Matrix& obj);
	std::ostream& operator>>(std::istream& is, const Matrix& obj);

private:

};

#endif //MATRIX_H