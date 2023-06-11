#include <iostream>
#include "Matrix.h"

int wmain()
{
	Matrix a(3, 2);
	std::cout << "Insert Matrix:" << std::endl;
	std::cin >> a;
	a.plain_print();
	a.transpose();
	a.plain_print();
	a.vectorize();
	a.plain_print();
	return 0;
}