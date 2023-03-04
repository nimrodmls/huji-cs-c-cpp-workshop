#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cipher.h"
#include "tests.h"

#define INVALID_ARGUMENT_COUNT 1

#define ENCODE_COMMAND_STR "encode"
#define DECODE_COMMAND_STR "decode"
#define TEST_COMMAND_STR "test"

typedef enum _command
{
	COMMAND_INVALID = 0,
	COMMAND_ENCODE,
	COMMAND_DECODE,
	COMMAND_TEST

} command, *p_command;

typedef enum _argument_index
{
	ARGUMENT_COMMAND = 1,
	ARGUMENT_SHIFT,
	ARGUMENT_IN_FILE,
	ARGUMENT_OUT_FILE

} argument_index, *p_argument_index;

int get_file_data(
	__in const char * file_path, __deref_out char ** out_data)
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
	__in const char * file_path, __in const char * data, __in size_t data_size
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
 * @param shift_count - The amount of shift to perform in cipher
 * @param in_file - The file to encrypt
 * @param out_file - The file to decrypt
 * @return 0 on failure, 1 on success.
 */
int encode_decode(command cmd, 
				   int shift_count, 
				   const char * in_file, 
				   const char * out_file)
{
	int is_successful = 0;
	char* input = NULL;

	if ((COMMAND_ENCODE != cmd) && (COMMAND_DECODE != cmd))
	{
		goto lblCleanup;
	}

	if (!get_file_data(in_file, &input))
	{
		goto lblCleanup;
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
		goto lblCleanup;
	}

	is_successful = 1;

lblCleanup:

	if (NULL != input)
	{
		free(input);
		input = NULL;
	}

	return is_successful;
}

command get_command_type(const char * cmd)
{
	command out_cmd = COMMAND_INVALID;

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

int main (int argc, char *argv[])
{
	int exit_value = EXIT_FAILURE;
	command cmd_type = COMMAND_INVALID;
	int shift_count = 0;

	if (INVALID_ARGUMENT_COUNT == argc)
	{
		goto lblCleanup;
	}

	cmd_type = get_command_type(argv[ARGUMENT_COMMAND]);
	if (COMMAND_INVALID == cmd_type)
	{
		goto lblCleanup;
	}

	if (COMMAND_TEST == cmd_type)
	{
		
	}
	else // Encode / Decode Command
	{
		shift_count = strtol(argv[ARGUMENT_SHIFT], NULL, 10);
		if ((LONG_MAX == shift_count) || (LONG_MIN == shift_count))
		{
			goto lblCleanup;
		}

		if (!encode_decode(cmd_type,
						   shift_count,
					argv[ARGUMENT_IN_FILE],
				   argv[ARGUMENT_OUT_FILE]))
		{
			goto lblCleanup;
		}
	}

	exit_value = EXIT_SUCCESS;

lblCleanup:
	return exit_value;
}
