#include <stdio.h>
#include <stdlib.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"

// Constants
#define DECIMAL_BASE (10)
#define INPUT_BUFFER_SIZE_BYTES (61)
#define READ_UNTIL_NEW_LINE ("%[^\n]")
#define BUS_LINE_INPUT_FORMAT ("%20[^A-Z,\n],%d,%d")

// Prompt strings for the user
#define LINE_COUNT_PROMPT ("Enter number of lines. Then enter\n")
#define LINE_INFO_PROMPT ("Enter line info. Then enter\n")
#define USAGE_PROMPT ("USAGE: sort_lines [by_name/by_duration/by_distance/test]\n")
#define ERROR_INVALID_LINE_COUNT ("ERROR: Expected positive integer. Try again.\n")
#define ERROR_BUS_NAME_EXISTS ("ERROR: Bus name already exists!\n")
#define ERROR_BUS_NAME_INVALID ("ERROR: Bus name should contain only digits and small chars\n")
#define ERROR_DURATION_INVALID ("ERROR: Duration should be an integer between 10 and 100 (including)\n")
#define ERROR_DISTANCE_INVALID ("ERROR: Distance should be an integer between 0 and 1000 (including)\n")

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
typedef enum BusLineParams
{
	BUS_LINE_PARAM_NAME = 0,
	BUS_LINE_PARAM_DISTANCE,
	BUS_LINE_PARAM_DURATION,
	BUS_LINE_PARAM_COUNT

} BusLineParams, *pBusLineParams;

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

ProgramStatus get_user_input(char* prompt, char* user_input, int input_max_len);

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

ProgramStatus get_user_input(char* prompt, char* user_input, int input_max_len)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;

	if ((NULL == prompt) || (NULL == user_input))
	{
		return status;
	}

	(void)fprintf(stdout, prompt);
	if (NULL == fgets(user_input, input_max_len, stdin))
	{
		return status;
	}

	if (EOF == sscanf(
		user_input, READ_UNTIL_NEW_LINE, user_input))
	{
		return status;
	}

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

ProgramStatus get_bus_line_count(unsigned long* line_count)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	char user_input[INPUT_BUFFER_SIZE_BYTES] = { 0 };

	if (NULL == line_count)
	{
		return status;
	}

	do
	{
		status = get_user_input(
			LINE_COUNT_PROMPT, user_input, sizeof(user_input));
		if (STATUS_FAILED(status))
		{
			return status;
		}

	} while (STATUS_FAILED(
		bus_line_count_tester(user_input, line_count)));

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}

ProgramStatus parse_bus_line_input(char* input, BusLine* parsed)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	int params_read = 0;

	if ((NULL == input) || (NULL == parsed))
	{
		return status;
	}

	params_read = sscanf(
		input,
		BUS_LINE_INPUT_FORMAT,
		parsed->name,
		&(parsed->distance),
		&(parsed->duration));
	if (BUS_LINE_PARAM_NAME == params_read)
	{
		(void)fprintf(stdout, ERROR_BUS_NAME_INVALID);
		return status;
	}
	if ((BUS_LINE_PARAM_DISTANCE == params_read) || ((BUS_LINE_PARAM_DISTANCE < params_read) && (parsed->distance < 0 || parsed->distance > 1000)))
	{
		(void)fprintf(stdout, ERROR_DISTANCE_INVALID);
		return status;
	}
	if ((BUS_LINE_PARAM_DURATION == params_read) || ((BUS_LINE_PARAM_DURATION < params_read) && (parsed->duration < 10 || parsed->duration > 100)))
	{
		(void)fprintf(stdout, ERROR_DURATION_INVALID);
		return status;
	}

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}

ProgramStatus bus_line_input_tester(
	char* input, BusLine* all_lines, unsigned long current_line
)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	BusLine local_line_info = { 0 };

	if ((NULL == input) || 
		(NULL == all_lines))
	{
		return status;
	}

	status = parse_bus_line_input(input, &local_line_info);
	if (STATUS_FAILED(status))
	{
		return status;
	}

	memcpy(all_lines + current_line, &local_line_info, sizeof(local_line_info));

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}

// See documentation at declaration
ProgramStatus get_bus_lines_from_input(
	unsigned long line_count, BusLine* bus_lines)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	char user_input[INPUT_BUFFER_SIZE_BYTES] = { 0 };
	unsigned long index = 0;
	
	if (NULL == bus_lines)
	{
		return status;
	}

	for (index = 0; index < line_count; index++)
	{
		do
		{
			status = get_user_input(
						LINE_INFO_PROMPT,
						user_input,
						sizeof(user_input));
			if (STATUS_FAILED(status))
			{
				return status;
			}
	
		} while (STATUS_FAILED(
			bus_line_input_tester(user_input, bus_lines, index)));
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

	status = get_bus_line_count(&line_count);
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
	FREE_MEMORY(bus_lines);

	// TODO: REMOVE!
	if (STATUS_FAILED(status))
	{
		(void)fprintf(stdout, "ERROR IN PROGRAM?!");
	}

	return status;
}
