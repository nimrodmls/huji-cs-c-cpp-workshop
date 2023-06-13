#include <iostream>
#include <fstream>

#include "Activation.h"
#include "Matrix.h"

bool readFileToMatrix(const std::string& filePath, Matrix& mat)
{
	std::fstream in_file(filePath, std::ios::binary | std::ios::in);
	in_file >> mat;
	return true;
}

int wmain()
{
	//Matrix file_mat(28, 28);
	//readFileToMatrix("C:\\Projects\\huji-cs-c-cpp-workshop\\ex4\\ex4\\images\\im0", file_mat);
	//file_mat.plain_print();

	Matrix m(1, 9);
	for (int i = 0; i < m.get_cols() * m.get_rows(); i++)
	{
		m[i] = (float)(i - 2);
	}

	activation::softmax(m);

	Matrix a(3, 3);
	std::cout << "Insert Matrix:" << std::endl;
	//std::cin >> a;
	Matrix b(a);
	a.plain_print();
	std::cout << std::endl;
	b.plain_print();
	std::cout << std::endl;
	auto c = a.dot(b);
	c.plain_print();
	std::cout << std::endl;

	std::cout << "Matrix Multiplication: " << std::endl;
	c = a * b;
	c.plain_print();
	std::cout << std::endl;

	a += b;
	a.plain_print();
	std::cout << std::endl;
	c = a + b;
	c.plain_print();
	std::cout << std::endl;

	a.transpose();
	a.plain_print();
	std::cout << std::endl;
	a.vectorize();
	a.plain_print();
	std::cout << std::endl;
	std::cout << a.norm() << std::endl;
	std::cout << a.argmax() << std::endl;
	std::cout << a.sum() << std::endl;
	return 0;
}