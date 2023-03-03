#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cipher.h"
#include "tests.h"

int main (int argc, char *argv[])
{
	char * str = (char *)calloc(30, 1);
	char * a = "Hello, world!";
	strcpy(str, a);
	printf(str);

	encode(str, -1);
	printf(str);
	decode(str, -1);
	printf(str);
	return EXIT_FAILURE;
}
