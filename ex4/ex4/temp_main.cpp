#include <iostream>
#include "Matrix.h"

int wmain()
{
	Matrix a(3, 2);
	std::cout << "Insert Matrix:" << std::endl;
	std::cin >> a;
	std::cout << a << std::endl;
	a.transpose();
	std::cout << a << std::endl;
	return 0;
}