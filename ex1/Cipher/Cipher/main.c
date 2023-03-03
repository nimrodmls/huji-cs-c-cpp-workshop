#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cipher.h"
#include "tests.h"

#define ENCODE_COMMAND "encode"
#define DECODE_COMMAND "decode"

void get_file_data(
	__in const char * file_path, __deref_out char ** out_data)
{
	FILE* file_handle = NULL;
	char* file_data = NULL;
	unsigned long file_size = 0;
	size_t read_bytes = 0;
	
	if (NULL == file_path || NULL == out_data)
	{
		goto lblCleanup;
	}

	file_handle = fopen(file_path, "r");
	if (NULL == file_handle)
	{
		goto lblCleanup;
	}

	if (0 != fseek(file_handle, 0, SEEK_END))
	{
		goto lblCleanup;
	}

	file_size = ftell(file_handle);
	rewind(file_handle);

	// Allocating +1 for the file data in order to make space
	// for the null-terminator
	file_data = calloc(file_size + 1, sizeof(char));
	if (NULL == file_data)
	{
		goto lblCleanup;
	}

	read_bytes = fread(file_data, sizeof(char), file_size, file_handle);
	if (file_size != read_bytes)
	{
		goto lblCleanup;
	}

	if (ferror(file_handle))
	{
		goto lblCleanup;
	}

	// Transferring ownership over the data
	*out_data = file_data;
	file_data = NULL;

lblCleanup:
	if (NULL != file_handle)
	{
		(void)fclose(file_handle);
	}

	if (NULL != file_data)
	{
		free(file_data);
	}
}

void encode_decode(char * cmd, 
				   int shift_count, 
				   char * in_file, 
				   char * out_file)
{
	
}

int main (int argc, char *argv[])
{
	char* out = NULL;
	get_file_data("C:\\temp\\test.txt", &out);
	printf(out);
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
