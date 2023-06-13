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
	Matrix(const Matrix& matrix);
	~Matrix();

	int get_rows() const;
	int get_cols() const;
	Matrix& transpose();
	Matrix& vectorize();
	void plain_print();
	Matrix dot(Matrix& in);
	float norm();
	Matrix rref();
	int argmax();
	float sum() const;

	
	// Addition operators
	Matrix& operator+=(const Matrix& rhs);

	// Multiplication operators
	friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
	friend Matrix operator*(float scalar, const Matrix& rhs);
	friend Matrix operator*(const Matrix& lhs, float scalar);

	// Assignment operator
	Matrix& operator=(const Matrix& rhs);

	// Access operators
	float operator()(int row, int col) const;
	float& operator()(int row, int col);
	float operator[](int index) const;
	float& operator[](int index);

	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, Matrix& obj);
	friend std::istream& operator>>(std::istream& is, Matrix& obj);

private:
	static int coord_to_index(int row, int col, int col_count);
	static bool is_out_of_range(int index, int size);

	bool validate_dimensions(const Matrix& other) const;
	void copy_matrix(const Matrix& source);

	float* _rmatrix;
	int _rows = 0;
	int _columns = 0;
};

Matrix operator+(const Matrix& lhs, const Matrix& rhs);

#endif //MATRIX_H