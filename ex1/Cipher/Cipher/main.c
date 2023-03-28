#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cipher.h"
#include "tests.h"

// Constants
#define DECIMAL_BASE (10)
// 1024 characters + one for newline character
#define READ_BUFFER_SIZE (1025)

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
#define INVALID_TEST_PROMPT ("Usage: cipher test\n")
#define INVALID_SHIFT_PROMPT ("The given shift value is invalid.\n")

// Possible exit values of the program
typedef enum CipherStatus
{
	CIPHER_STATUS_FAILED = EXIT_FAILURE,
	CIPHER_STATUS_SUCCESS = EXIT_SUCCESS

} CipherStatus, * pCipherStatus;

// Checks whether a failure status has been reached
#define CIPHER_STATUS_FAILURE(result) (result != CIPHER_STATUS_SUCCESS)

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
	ARGUMENT_MIN_ARGS = 2,

	ARGUMENT_COMMAND = 1,
	ARGUMENT_SHIFT,
	ARGUMENT_IN_FILE,
	ARGUMENT_OUT_FILE,

	ARGUMENT_MAX_ARGS

} ArgumentIndex, *pArgumentIndex;

// Function declarations

/**
 * Checks whether the given file handle has any error.
 * @param file_handle - The handle to the file to check.
 * @return Non-zero on error, 0 otherwise.
 */
int check_file_handle(FILE* file_handle);

/**
 * Converts a string to long (with validity checks)
 * @param in - The string to convert
 * @param out - The output converted value
 */
CipherStatus convert_str_to_long(const char* in, long* out);

/**
 * Executes encode/decode operation depending on the input command
 * The function expects valid files as input.
 * @param cmd - The Command to execute (decode/encode are acceptable)
 * @param shift_count - The amount of shift to perform in cipher
 * @param in_file - Handle of the file to encrypt
 * @param out_file - Handle of the file to decrypt
 * @return CIPHER_STATUS_SUCCESS on success, otherwise error
 */
void encode_decode_file(const Command cmd,
						const int shift_count,
						FILE* in_file,
						FILE* out_file);

/**
 * Executes encode/decode operation depending on the input command
 * @param cmd - The Command to execute (decode/encode are acceptable)
 * @param shift - The amount of shift to perform in cipher
 * @param in_file_path - Path to the file to encrypt
 * @param out_file_path - Path to the file to decrypt
 * @return CIPHER_STATUS_SUCCESS on success, otherwise error
 */
CipherStatus encode_decode(Command cmd,
						   const char* shift,
						   const char* in_file_path,
						   const char* out_file_path);


/**
 * Parsing and converting the command line parameter for the command
 * @param cmd - The string of the command
 * @return The command, COMMAND_INVALID if no command has been found
 */
Command get_command_type(const char* cmd);

/**
 * Running all the test suites.
 * @return if either one of the tests or more failed,
 *		   non-zero is retured.
 */
int run_tests(void);

/**
 * Crossing the argument count the program recieived with the
 * type of command, determines if they are correct and
 * prints prompt to the user accordingly.
 * @param argc - The argument count received by the program.
 * @param cmd_type - The type of command received by the program.
 * @return CIPHER_STATUS_SUCCESS on valid
 */
CipherStatus validate_parameters(int argc, Command cmd_type);

// Function definitions

// See documentation at declaration
int check_file_handle(FILE* file_handle)
{
	int result = 0;
	result += ferror(file_handle);
	result += feof(file_handle);
	return result;
}

// See documentation at declaration
CipherStatus convert_str_to_long(const char * in, long * out)
{
	CipherStatus status = CIPHER_STATUS_FAILED;
	long conv = 0;
	char* result = NULL;

	if (NULL == out)
	{
		goto cleanup;
	}

	errno = 0; // Setting errno to 0 to make sure strstol failed
	conv = strtol(in, &result, DECIMAL_BASE);
	// Dealing with all failure cases for conversion - 
	// if errno is set, or the string has non-numerical chars
	if (((0 == conv) && (errno != 0)) || 
		(0 != *result))
	{
		goto cleanup;
	}

	*out = conv;
	status = CIPHER_STATUS_SUCCESS;

cleanup:
	return status;
}

// See documentation at declaration
void encode_decode_file(const Command cmd,
						const int shift_count,
						FILE* in_file,
						FILE* out_file)
{
	char read_buffer[READ_BUFFER_SIZE] = { 0 };
	char* read_result = NULL;

	// Encoding/Decoding a single line in each iteration
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
		default:
			return;
		}

		if (EOF == fputs(read_result, out_file))
		{
			return;
		}

		// Going with the reading/writing as long as the files
		// don't have any error out of the sudden
	} while ((0 == check_file_handle(in_file)) &&
			 (0 == check_file_handle(out_file)));
}

// See documentation at declaration
CipherStatus encode_decode(Command cmd,
				  const char * shift,
				  const char * in_file_path,
				  const char * out_file_path)
{
	CipherStatus status = CIPHER_STATUS_FAILED;
	long shift_count = 0;
	FILE* in_file_handle = NULL;
	FILE* out_file_handle = NULL;

	if (CIPHER_STATUS_FAILURE(
		convert_str_to_long(shift, &shift_count)))
	{
		(void)fprintf(stderr, INVALID_SHIFT_PROMPT);
		goto cleanup;
	}

	in_file_handle = fopen(in_file_path, FILE_READ);
	out_file_handle = fopen(out_file_path, FILE_WRITE);
	if ((NULL == in_file_handle) || (NULL == out_file_handle))
	{
		(void)fprintf(stderr, FILE_ERROR_PROMPT);
		goto cleanup;
	}
	
	encode_decode_file(cmd, shift_count, in_file_handle, out_file_handle);

	status = CIPHER_STATUS_SUCCESS;

cleanup:
	if (NULL != in_file_handle)
	{
		(void)fclose(in_file_handle);
	}
	if (NULL != out_file_handle)
	{
		(void)fclose(out_file_handle);
	}

	return status;
}

// See documentation at declaration
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

// See documentation at declaration
int run_tests(void)
{
	int test_val = 0;
	// Mandatory tests
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

// See documentation at declaration
CipherStatus validate_parameters(int argc, Command cmd_type)
{
	CipherStatus status = CIPHER_STATUS_FAILED;

	// If the program received the minimal amount of arguments
	// but it's not the test command
	if ((cmd_type != COMMAND_TEST) && (ARGUMENT_MIN_ARGS == argc))
	{
		(void)fprintf(stderr, INVALID_TEST_PROMPT);
		goto cleanup;
	}
	// If the program received 4 arguments but the command is 
	// not encode/decode
	else if ((cmd_type != COMMAND_DECODE) && 
			 (cmd_type != COMMAND_ENCODE) &&
			 ARGUMENT_MAX_ARGS == argc)
	{
		(void)fprintf(stderr, INVALID_COMMAND_PROMPT);
		goto cleanup;
	}
	else if (((cmd_type == COMMAND_ENCODE) || 
			  (cmd_type == COMMAND_DECODE)) &&
			 ARGUMENT_MAX_ARGS > argc)
	{
		(void)fprintf(stderr, INVALID_INPUT_PROMPT);
		goto cleanup;
	}

	status = CIPHER_STATUS_SUCCESS;

cleanup:
	return status;
}

/** 
 * The executable entrypoint.
 * @param argc - Element count in argv.
 * @param argv - The parameters, this program requires the following:
 *				 > First parameter is either 
				   'encode', 'decode' or 'test'
 *					- 'test' launches the tests suite
 *					- 'encode' launches the encode mode
 *					- 'deocde' launches the decode mode
 *				 > If either 'encode' or 'decode' were entered, the
 *				   following 3 parameters are MANDATORY!
 *				   'test' does not require them, and will 
				   fail if any entered.
 *					- Second parameter is the shift value 
					  for the cipher.
 *					- Third parameter is the input file to 
					  encode/decode.
 *					- Forth parameter is the output file for the 
					  encoded/decoded input.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
 */
int main(int argc, char * argv[])
{
	CipherStatus exit_value = CIPHER_STATUS_FAILED;
	Command cmd_type = COMMAND_INVALID;

	if ((ARGUMENT_MIN_ARGS > argc) || (ARGUMENT_MAX_ARGS < argc))
	{
		(void)fprintf(stderr, INVALID_INPUT_PROMPT);
		goto cleanup;
	}

	cmd_type = get_command_type(argv[ARGUMENT_COMMAND]);
	exit_value = validate_parameters(argc, cmd_type);
	if (CIPHER_STATUS_FAILURE(exit_value))
	{
		goto cleanup;
	}

	switch (cmd_type)
	{
	case COMMAND_TEST:
		if (!run_tests())
		{
			goto cleanup;
		}
		break;
	case COMMAND_DECODE:
		// fallthrough
	case COMMAND_ENCODE:
		exit_value = encode_decode(cmd_type,
								   argv[ARGUMENT_SHIFT],
								   argv[ARGUMENT_IN_FILE],
								   argv[ARGUMENT_OUT_FILE]);
		if (CIPHER_STATUS_FAILURE(exit_value))
		{
			goto cleanup;
		}
		break;
	default:
		break;
	}

	exit_value = CIPHER_STATUS_SUCCESS;
cleanup:
	return (int)exit_value;
}
