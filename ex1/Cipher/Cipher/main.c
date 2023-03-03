#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cipher.h"
#include "tests.h"

int main (int argc, char *argv[])
{
	char * str = (char *)calloc(30, 1);
	char* str2 = (char*)calloc(30, 1);
	char * a = "Hello, world!\n";
	strcpy(str, a);
	strcpy(str2, a);

	int shift = 20;
	encode(str, shift);
	printf(str);
	encode(str2, -shift);
	printf(str2);
	//decode(str, -1);
	//printf(str);
	return EXIT_FAILURE;
}
