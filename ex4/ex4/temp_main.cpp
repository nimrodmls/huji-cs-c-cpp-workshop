#include <iostream>
#include "Matrix.h"

int wmain()
{
	Matrix a(3, 2);
	std::cout << "Insert Matrix:" << std::endl;
	std::cin >> a;
	Matrix b(a);
	b.transpose();
	a.plain_print();
	std::cout << std::endl;
	b.plain_print();
	std::cout << std::endl;
	auto c = a.dot(b);
	c.plain_print();
	return 0;
}