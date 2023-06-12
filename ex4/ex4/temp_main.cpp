#include <iostream>
#include "Matrix.h"

int wmain()
{
	Matrix a(3, 2);
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
	a.transpose();
	a.plain_print();
	std::cout << std::endl;
	a.vectorize();
	a.plain_print();
	std::cout << std::endl;
	return 0;
}