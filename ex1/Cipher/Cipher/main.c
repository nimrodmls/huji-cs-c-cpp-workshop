#include <stdio.h>
#include <stdlib.h>

#include "cipher.h"
#include "tests.h"

int main (int argc, char *argv[])
{
	char * a = "Hello, world!";
	encode(a, -26);
	//printf(a);
	return EXIT_FAILURE;
}
