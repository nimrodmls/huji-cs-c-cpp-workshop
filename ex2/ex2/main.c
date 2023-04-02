#include <stdio.h>
#include <stdlib.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"

// Constants
#define DECIMAL_BASE (10)
#define INPUT_BUFFER_SIZE_BYTES (61)
#define READ_UNTIL_NEW_LINE ("%[^\n]")

// Prompt strings for the user
#define LINE_COUNT_PROMPT ("Enter number of lines. Then enter\n")
#define LINE_INFO_PROMPT ("Enter line info. Then enter\n")
#define USAGE_PROMPT ("USAGE: sort_lines [by_name/by_duration/by_distance/test]\n")
#define ERROR_INVALID_LINE_COUNT ("ERROR: Expected positive integer. Try again.")

// Constants for command type string identifier
#define COMMAND_BY_DURATION_STR ("by_duration")
#define COMMAND_BY_DISTANCE_STR ("by_distance")
#define COMMAND_BY_NAME_STR ("by_name")
#define COMMAND_TEST_STR ("test")

//
typedef enum ProgramStatus
{
	PROGRAM_STATUS_SUCCESS = EXIT_SUCCESS,
	PROGRAM_STATUS_FAILED = EXIT_FAILURE

} ProgramStatus, *pProgramStatus;

//
typedef enum Arguments
{
	ARGUMENT_COMMAND = 1,
	ARGUMENT_MAX_ARGS

} Arguments, *pArguments;

// 
typedef enum CommandType
{
	COMMAND_INVALID = 0,
	COMMAND_BY_DURATION,
	COMMAND_BY_DISTANCE,
	COMMAND_BY_NAME,
	COMMAND_TEST

} CommandType, *pCommandType;

//
typedef ProgramStatus(*InputTester) (
	char* input, void* user_input);

// Macros
//
#define STATUS_FAILED(status) (PROGRAM_STATUS_SUCCESS != status)

//
#define FREE_MEMORY(ptr) \
{						 \
	if (NULL != ptr)	 \
	{					 \
		free(ptr);		 \
	}					 \
}
 
// Function declarations

ProgramStatus convert_str_to_long(const char* in, long* out);

ProgramStatus get_user_input(
	char* prompt, InputTester tester, void* user_input);

ProgramStatus bus_line_count_tester(
	char* input, unsigned long* line_count);

ProgramStatus get_bus_lines_from_input(
	unsigned long line_count, BusLine* bus_lines);

ProgramStatus get_bus_lines(BusLine** bus_lines);

/**
 * Resolving the command type from the given string.
 * @param in - The input string to resolve.
 * @param cmd - The output command type, once resolved.
 * @return Whether the resolution has been successful or not.
 */
ProgramStatus resolve_command_type(char* in, pCommandType cmd);

// Fuction definitions

// See documentation at declaration
ProgramStatus convert_str_to_long(const char* in, long* out)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
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
	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	return status;
}

// See documentation at declaration
ProgramStatus get_user_input(
	char* prompt, InputTester tester, void* user_input)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	char input_local[INPUT_BUFFER_SIZE_BYTES] = { 0 };

	if ((NULL == user_input) || (NULL == tester))
	{
		return status;
	}

	do
	{
		(void)fprintf(stdout, prompt);
		if (NULL == fgets(input_local, sizeof(input_local), stdin))
		{
			return status;
		}

		if (EOF == sscanf(
				input_local, READ_UNTIL_NEW_LINE, input_local))
		{
			return status;
		}

	} while (STATUS_FAILED(tester(input_local, user_input)));

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}

// See documentation at declaration
ProgramStatus bus_line_count_tester(
	char* input, unsigned long* line_count)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	long count = 0;

	if ((NULL == input) || (NULL == line_count))
	{
		return status;
	}
	
	status = convert_str_to_long(input, &count);
	if (STATUS_FAILED(status))
	{
		(void)fprintf(stdin, ERROR_INVALID_LINE_COUNT);
		return status;
	}

	if (count <= 0)
	{
		(void)fprintf(stdin, ERROR_INVALID_LINE_COUNT);
		return status;
	}

	*line_count = (unsigned long)count;

	status = PROGRAM_STATUS_SUCCESS;

	return status;
}

ProgramStatus bus_line_input_tester(
	char* input, BusLine* line_info
)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;

	if ((NULL == input) || (NULL == line_info))
	{
		return status;
	}

	(void)sscanf(input, "%20[^A-Z,],%d,%d", line_info->name, &(line_info->distance), &(line_info->duration));

	status = PROGRAM_STATUS_SUCCESS;

	return status;
}

// See documentation at declaration
ProgramStatus get_bus_lines_from_input(
	unsigned long line_count, BusLine* bus_lines)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	unsigned long index = 0;
	
	if (NULL == bus_lines)
	{
		return status;
	}

	for (index = 0; index < line_count; index++)
	{
		get_user_input(LINE_INFO_PROMPT, bus_line_input_tester, (void*)(bus_lines + index));
	}

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}

// See documentation at declaration
ProgramStatus get_bus_lines(BusLine** bus_lines)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	BusLine* lines_local = NULL;
	unsigned long line_count = 0;

	if (NULL == bus_lines)
	{
		goto cleanup;
	}

	status = get_user_input(
		LINE_COUNT_PROMPT, bus_line_count_tester, &line_count);
	if (STATUS_FAILED(status))
	{
		goto cleanup;
	}

	lines_local = (BusLine *)calloc(line_count, sizeof(*lines_local));
	if (NULL == lines_local)
	{
		goto cleanup;
	}

	status = get_bus_lines_from_input(line_count, lines_local);
	if (STATUS_FAILED(status))
	{
		goto cleanup;
	}

	// Ownership transfer
	*bus_lines = lines_local;
	lines_local = NULL;

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	FREE_MEMORY(lines_local);

	return status;
}

// See documentation at declaration
ProgramStatus resolve_command_type(char* in, pCommandType cmd)
{
	CommandType out_cmd = COMMAND_INVALID;

	if (NULL == in)
	{
		return PROGRAM_STATUS_FAILED;
	}

	if (0 == strcmp(in, COMMAND_BY_DURATION_STR))
	{
		out_cmd = COMMAND_BY_DURATION;
	}
	else if (0 == strcmp(in, COMMAND_BY_DISTANCE_STR))
	{
		out_cmd = COMMAND_BY_DISTANCE;
	}
	else if (0 == strcmp(in, COMMAND_BY_NAME_STR))
	{
		out_cmd = COMMAND_BY_NAME;
	}
	else if (0 == strcmp(in, COMMAND_TEST_STR))
	{
		out_cmd = COMMAND_TEST;
	}

	*cmd = out_cmd;

	return PROGRAM_STATUS_SUCCESS;
}

/**
 * TODO add documentation
 */
int main (int argc, char *argv[])
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	CommandType cmd = COMMAND_INVALID;
	BusLine* bus_lines = NULL;

	if (ARGUMENT_MAX_ARGS != argc)
	{
		goto cleanup;
	}

	status = resolve_command_type(argv[ARGUMENT_COMMAND], &cmd);
	if (STATUS_FAILED(status))
	{
		(void)fprintf(stdout, USAGE_PROMPT);
		goto cleanup;
	}

	get_bus_lines(&bus_lines);

cleanup:
	return status;
}
