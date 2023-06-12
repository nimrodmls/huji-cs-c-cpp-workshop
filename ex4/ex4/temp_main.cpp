#include <iostream>
#include "Matrix.h"

int wmain()
{
	Matrix a(3, 3);
	std::cout << "Insert Matrix:" << std::endl;
	std::cin >> a;
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