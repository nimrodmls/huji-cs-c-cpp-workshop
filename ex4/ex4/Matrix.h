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

/**
* @class Matrix
 * @brief Matrix datatype of floating-point variables.
 *		  Supports elementary matrix operations.
 */
class Matrix
{
public:

	// Instance Construction / Destruction

	/**
	* Default Ctor, constructing a single-cell matrix, 
	* initialized to 0.
	*/
	Matrix();

	/**
	* Constructs matrix with the given dimensions.
	* All cells initialized to 0.
	* @param rows - The number of rows in the matrix.
	* @param cols - The number of columns in the matrix.
	*/
	Matrix(int rows, int cols);

	/**
	* Copy Constructor.
	* @param matrix - The matrix to copy.
	*/
	Matrix(const Matrix& matrix);

	/**
	* Destructor, freeing all dynamic matrix memory.
	*/
	~Matrix();

	// Generic operations

	/**
	* Getting the number of rows in the matrix.
	*/
	int get_rows() const;

	/**
	* Getting the number of columns in the matrix.
	*/
	int get_cols() const;

	/**
	* Transposing the matrix.
	* (switching rows with columns and vice versa)
	* Note: Transposing is done on the object, and NOT copied.
	* @return The same instance of this matrix, supports chaining.
	*/
	Matrix& transpose();

	/**
	* Vectorizing the matrix.
	* (converting matrix to single-column matrix)
	* Note: Vectorizing is done on the object, and NOT copied.
	* @return The same instance of this matrix, supports chaining.
	*/
	Matrix& vectorize();

	/**
	* Printing the matrix as-is
	*/
	void plain_print() const;

	/**
	* Calculating the dot product of the given matrix with the
	* instance matrix.
	* @param in - The matrix to calculate the dot product with.
	*			  This matrix should have the SAME dimensions as
	*			  of the instance matrix.
	* @throws std::length_error in case of incompatible dimensions
	* @return The dot product matrix.
	*/
	Matrix dot(Matrix& in) const;

	/**
	* Calculating the Frobenius Norm of the instance matrix.
	* @return The Frobenius Norm.
	*/
	float norm() const;

	Matrix rref();

	/**
	* Getting the maximal value in the matrix.
	* @return The maximal value.
	*/
	int argmax() const;

	/**
	* Calculating the sum of all values in the matrix.
	* @return The sum.
	*/
	float sum() const;

	// Operators overloading

	/**
	* Addition assignment operator to add a matrix to the
	* instance matrix.
	* @param rhs - The right-hand side matrix. The dimensions
	*			   of both matrices should be the same.
	* @throws std::length_error in case of incompatible dimensions.
	* @return The instance matrix.
	*/
	Matrix& operator+=(const Matrix& rhs);

	/**
	* Multiplication operator for multiplying 2 matrices.
	* Note: Function is friend since operator*= is not defined.
	* @param lhs - The left-hand side of the multiplication.
	* @param rhs - The right-hand side of the multiplication.
	* @throws std::length_error in case of incompatible lhs-rhs
	*		  dimensions (lhs column count != rhs row count)
	* @return The new matrix produced from multiplication.
	*/
	friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);

	/**
	* Multiplication operator for multiplying a matrix by a scalar,
	* from the left.
	* Note: Function is friend since operator*= is not defined.
	* @param scalar - The scalar to multiply by.
	* @param rhs - The right-hand side of the multiplication.
	* @return The new matrix produced from multiplication.
	*/
	friend Matrix operator*(float scalar, const Matrix& rhs);

	/**
	* Multiplication operator for multiplying a matrix by a scalar,
	* from the right.
	* Note: Function is friend since operator*= is not defined.
	* @param lhs - The left-hand side of the multiplication.
	* @param scalar - The scalar to multiply by.
	* @return The new matrix produced from multiplication.
	*/
	friend Matrix operator*(const Matrix& lhs, float scalar);

	/**
	* Assignment operator for copying a matrix by assignment.
	* @param rhs - The right hand side of the operator (the source).
	* @return Reference to the new, copied matrix.
	*/
	Matrix& operator=(const Matrix& rhs);

	/**
	* Access operator for accessing a cell by row,column coordinates.
	* @param row - The row to access.
	* @param col - The column to access.
	* @throws std::out_of_range in case of invalid coordinate.
	* @return The value in the cell.
	*/
	float operator()(int row, int col) const;

	/**
	* Access operator for accessing AND modifying a cell 
	* by row,column coordinates.
	* @param row - The row to access.
	* @param col - The column to access.
	* @throws std::out_of_range in case of invalid coordinate.
	* @return Ref to the value in the cell.
	*/
	float& operator()(int row, int col);

	/**
	* Access operator for accessing a cell by a raw index.
	* @param index - The index to access.
	* @throws std::out_of_range in case of invalid index.
	* @return The value in the cell.
	*/
	float operator[](int index) const;

	/**
	* Access operator for accessing AND modifying a cell
	* by a raw index.
	* @param index - The index to access.
	* @throws std::out_of_range in case of invalid index.
	* @return The value in the cell.
	*/
	float& operator[](int index);

	/**
	* Output stream for the matrix.
	*/
	friend std::ostream& operator<<(std::ostream& os, Matrix& obj);

	/**
	* Input stream for the matrix, reading binary floating-point
	* data from the given stream, until matrix is full.
	*/
	friend std::istream& operator>>(std::istream& is, Matrix& obj);

private:
	/**
	* Converting row-column coordinate to raw index.
	* @param row - The row to convert from.
	* @param col - The column to convert from.
	* @param col_count - Amount of columns in a single row.
	* @return The raw index value.
	*/
	static int coord_to_index(int row, int col, int col_count);

	/**
	* Checking if an index is out of bounds from the given size.
	* @param index - The index to validate.
	* @param size - The size of the array.
	* @return True on out of range, false otherwise.
	*/
	static bool is_out_of_range(int index, int size);

	/**
	* Validating the dimensions of another matrix
	* with the instance matrix.
	* @param other - The other matrix to check with.
	* @return True on valid dimensions, false otherwise.
	*/
	bool validate_dimensions(const Matrix& other) const;

	/**
	* Copying all cells from the given source matrix
	* to the instance matrix. NOT checking any dimensions
	* the function expects source matrix at-least as big
	* as the instance matrix.
	* @param source - The source matrix to copy from.
	*/
	void copy_matrix(const Matrix& source);

	// The raw matrix, represented as single-dimension array
	float* _rmatrix;
	// The row count of the matrix
	int _rows = 0;
	// The column count of the matrix
	int _columns = 0;
};

/**
	* Adding two matrices together.
	* @param lhs - The left-hand matrix.
	* @param rhs - The right hand matrix.
	* @throws std::length_error in case of incompatible
	*		  dimensions (lhs & rhs should be of the same dimension).
	* @return The new summation matrix.
	*/
Matrix operator+(const Matrix& lhs, const Matrix& rhs);

#endif //MATRIX_H