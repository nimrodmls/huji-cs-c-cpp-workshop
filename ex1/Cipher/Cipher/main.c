#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cipher.h"
#include "tests.h"

#ifndef __fallthrough
#define __fallthrough
#endif

#define READ_BUFFER_SIZE (1024)

// File opening modes
#define FILE_WRITE ("w")
#define FILE_READ ("r")

// Command strings
#define ENCODE_COMMAND_STR ("encode")
#define DECODE_COMMAND_STR ("decode")
#define TEST_COMMAND_STR ("test")

// Prompt strings
#define FILE_ERROR_PROMPT ("The given file is invalid.\n")
#define INVALID_INPUT_PROMPT ("The program receives 1 or 4 arguments only.\n")
#define INVALID_COMMAND_PROMPT ("The given command is invalid.\n")
#define INVALID_TEST_PROMPT ("cipher test\n")
#define INVALID_SHIFT_PROMPT ("The given shift value is invalid,\n")

// Valid command types
typedef enum Command
{
	COMMAND_INVALID = 0,
	COMMAND_ENCODE,
	COMMAND_DECODE,
	COMMAND_TEST

} Command, *pCommand;

// Argument indices for the command line arguments
typedef enum ArgumentIndex
{
	ARGUMENT_COMMAND = 1,
	ARGUMENT_SHIFT,
	ARGUMENT_IN_FILE,
	ARGUMENT_OUT_FILE,

	ARGUMENT_MAX_ARGS

} ArgumentIndex, *pArgumentIndex;

int check_file_handle(const FILE* file_handle)
{
	int result = 0;
	result += ferror(file_handle);
	result += feof(file_handle);
	return result;
}

void encode_decode_file(Command cmd,
						 int shift_count,
						 const FILE* in_file,
						 const FILE* out_file)
{
	char read_buffer[READ_BUFFER_SIZE] = { 0 };
	char* read_result = NULL;
	do
	{
		read_result = fgets(read_buffer, sizeof(read_buffer), in_file);
		if (NULL == read_result)
		{
			return;
		}

		switch (cmd)
		{
		case COMMAND_ENCODE:
			encode(read_result, shift_count);
			break;
		case COMMAND_DECODE:
			decode(read_result, shift_count);
			break;
		}

		if (EOF == fputs(read_result, out_file))
		{
			return;
		}

	} while ((0 == check_file_handle(in_file)) &&
			 (0 == check_file_handle(out_file)));
}

/**
 * Executes encode/decode operation depending on the input command
 * @param cmd - The Command to execute (decode/encode are acceptable)
 * @param shift - The amount of shift to perform in cipher
 * @param in_file_path - The file to encrypt
 * @param out_file_path - The file to decrypt
 * @return 0 on failure, 1 on success.
 */
int encode_decode(Command cmd, 
				  const char * shift,
				  const char * in_file_path,
				  const char * out_file_path)
{
	int is_successful = 0;
	int shift_count = 0;
	FILE* in_file_handle = NULL;
	FILE* out_file_handle = NULL;

	errno = 0; // Setting errno to 0 to make sure strstol failed
	shift_count = strtol(shift, NULL, 10);
	if ((0 == shift_count) && (errno != 0))
	{
		fprintf(stderr, INVALID_SHIFT_PROMPT);
		goto cleanup;
	}

	in_file_handle = fopen(in_file_path, FILE_READ);
	out_file_handle = fopen(out_file_path, FILE_WRITE);
	if ((NULL == in_file_handle) || (NULL == out_file_handle))
	{
		fprintf(stderr, FILE_ERROR_PROMPT);
		goto cleanup;
	}
	
	encode_decode_file(cmd, shift_count, in_file_handle, out_file_handle);

	is_successful = 1;

cleanup:
	if (NULL != in_file_handle)
	{
		(void)fclose(in_file_handle);
	}
	if (NULL != out_file_handle)
	{
		(void)fclose(out_file_handle);
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

	if ((ARGUMENT_COMMAND != argc) && (ARGUMENT_MAX_ARGS != argc))
	{
		fprintf(stderr, INVALID_INPUT_PROMPT);
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
			fprintf(stderr, INVALID_TEST_PROMPT);
			goto cleanup;
		}
		else if (ARGUMENT_MAX_ARGS == argc)
		{
			fprintf(stderr, INVALID_COMMAND_PROMPT);
			goto cleanup;
		}
	}

	exit_value = EXIT_SUCCESS;
cleanup:
	return exit_value;
}
