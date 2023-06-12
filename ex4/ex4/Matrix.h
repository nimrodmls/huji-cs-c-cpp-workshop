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
	Matrix();
	Matrix(int rows, int cols);
	Matrix(Matrix& matrix);
	~Matrix();

	int get_rows() const;
	int get_cols() const;
	Matrix& transpose();
	Matrix& vectorize();
	void plain_print();
	Matrix dot(Matrix& in);
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
	float& operator()(int row, int col);
	float& operator[](int index);

	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, Matrix& obj);
	friend std::istream& operator>>(std::istream& is, Matrix& obj);

private:
	static float** _allocate_matrix(int rows, int columns);
	static void _destroy_matrix(float** matrix, int rows);

	bool _validate_dimensions(const Matrix& other) const;
	void _copy_matrix(Matrix& source);

private:
	float** _rmatrix;
	int _rows = 0;
	int _columns = 0;
};

#endif //MATRIX_H