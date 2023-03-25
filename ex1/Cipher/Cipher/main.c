#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cipher.h"
#include "tests.h"

#ifndef __fallthrough
#define __fallthrough
#endif

#define INVALID_ARGUMENT_COUNT 1

#define ENCODE_COMMAND_STR "encode"
#define DECODE_COMMAND_STR "decode"
#define TEST_COMMAND_STR "test"

typedef enum _Command
{
	COMMAND_INVALID = 0,
	COMMAND_ENCODE,
	COMMAND_DECODE,
	COMMAND_TEST

} Command, *pCommand;

typedef enum _ArgumentIndex
{
	ARGUMENT_COMMAND = 1,
	ARGUMENT_SHIFT,
	ARGUMENT_IN_FILE,
	ARGUMENT_OUT_FILE,

	ARGUMENT_MAX_ARGS

} ArgumentIndex, *pArgumentIndex;

int get_file_data(
	const char * file_path, char ** out_data)
{
	FILE* file_handle = NULL;
	char* file_data = NULL;
	unsigned long file_size = 0;
	size_t read_bytes = 0;
	int is_successful = 0;
	
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
	is_successful = 1;

lblCleanup:
	if (NULL != file_handle)
	{
		(void)fclose(file_handle);
		file_handle = NULL;
	}

	if (NULL != file_data)
	{
		free(file_data);
		file_data = NULL;
	}

	return is_successful;
}

int write_file_data(
	const char * file_path, const char * data, size_t data_size
)
{
	FILE* file_handle = NULL;
	size_t written_data = 0;
	int is_successful = 0;

	if (NULL == file_path || NULL == data)
	{
		goto lblCleanup;
	}

	file_handle = fopen(file_path, "w");
	if (NULL == file_handle)
	{
		goto lblCleanup;
	}

	written_data = fwrite(data, sizeof(char), data_size, file_handle);
	if (written_data != data_size)
	{
		goto lblCleanup;
	}

	is_successful = 1;

lblCleanup:

	if (NULL != file_handle)
	{
		(void)fclose(file_handle);
		file_handle = NULL;
	}

	return is_successful;
}

/**
 * Executes encode/decode operation depending on the input cmd
 * @param cmd - The Command to execute (either decode/encode are acceptable)
 * @param shift - The amount of shift to perform in cipher (as numerical string)
 * @param in_file - The file to encrypt
 * @param out_file - The file to decrypt
 * @return 0 on failure, 1 on success.
 */
int encode_decode(Command cmd, 
				   const char * shift,
				   const char * in_file, 
				   const char * out_file)
{
	int is_successful = 0;
	char* input = NULL;
	int shift_count = 0;

	errno = 0; // Setting errno to 0 to make sure strstol failed
	shift_count = strtol(shift, NULL, 10);
	if ((0 == shift_count) && (errno != 0))
	{
		fprintf(stderr, "The given shift value is invalid,\n");
		goto cleanup;
	}

	if (!get_file_data(in_file, &input))
	{
		fprintf(stderr, "The given file is invalid.\n");
		goto cleanup;
	}

	if (COMMAND_ENCODE == cmd)
	{
		encode(input, shift_count);
	}
	else if (COMMAND_DECODE == cmd)
	{
		decode(input, shift_count);
	}

	if (!write_file_data(out_file, input, strlen(input)))
	{
		fprintf(stderr, "The given file is invalid.\n");
		goto cleanup;
	}

	is_successful = 1;

cleanup:

	if (NULL != input)
	{
		free(input);
		input = NULL;
	}

	return is_successful;
}

Command get_command_type(const char * cmd)
{
	Command out_cmd = COMMAND_INVALID;

	if (0 == strcmp(cmd, DECODE_COMMAND_STR))
	{
		out_cmd = COMMAND_DECODE;
		goto lblCleanup;
	}

	if (0 == strcmp(cmd, ENCODE_COMMAND_STR))
	{
		out_cmd = COMMAND_ENCODE;
		goto lblCleanup;
	}

	if (0 == strcmp(cmd, TEST_COMMAND_STR))
	{
		out_cmd = COMMAND_TEST;
		goto lblCleanup;
	}

lblCleanup:

	return out_cmd;
}

int run_tests(void)
{
	int test_val = 0;
	test_val += 
		test_encode_non_cyclic_lower_case_positive_k();
	test_val += 
		test_encode_cyclic_lower_case_special_char_positive_k();
	test_val += 
		test_encode_non_cyclic_lower_case_special_char_negative_k();
	test_val += 
		test_encode_cyclic_lower_case_negative_k();
	test_val += 
		test_encode_cyclic_upper_case_positive_k();
	test_val +=
		test_decode_non_cyclic_lower_case_positive_k();
	test_val +=
		test_decode_cyclic_lower_case_special_char_positive_k();
	test_val +=
		test_decode_non_cyclic_lower_case_special_char_negative_k();
	test_val +=
		test_decode_cyclic_lower_case_negative_k();
	test_val +=
		test_decode_cyclic_upper_case_positive_k();
	return !test_val;
}

int main (int argc, char * argv[])
{
	int exit_value = EXIT_FAILURE;
	Command cmd_type = COMMAND_INVALID;
	int shift_count = 0;

	if ((ARGUMENT_COMMAND > argc) || (ARGUMENT_MAX_ARGS < argc))
	{
		fprintf(stderr, "The program receives 1 or 4 arguments only.\n");
		goto cleanup;
	}

	cmd_type = get_command_type(argv[ARGUMENT_COMMAND]);
	switch (cmd_type)
	{
	case COMMAND_TEST:
		if (!run_tests())
		{
			goto cleanup;
		}

		break;
	case COMMAND_DECODE:
		__fallthrough;
	case COMMAND_ENCODE:
		if (!encode_decode(cmd_type,
						   argv[ARGUMENT_SHIFT],
						   argv[ARGUMENT_IN_FILE],
						   argv[ARGUMENT_OUT_FILE]))
		{
			goto cleanup;
		}

		break;
	default:
		// If the program received only 1 argument and it's not test command
		if (ARGUMENT_COMMAND == argc)
		{
			fprintf(stderr, "cipher test\n");
		}
	}

	exit_value = EXIT_SUCCESS;

cleanup:
	return exit_value;
}
