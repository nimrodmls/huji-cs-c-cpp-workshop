#include <stdio.h>
#include <stdlib.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"

// Constants
#define DECIMAL_BASE (10)
#define INPUT_BUFFER_SIZE_BYTES (61)
#define READ_UNTIL_NEW_LINE ("%[^\n]")
#define BUS_LINE_INPUT_FORMAT ("%20[^A-Z,\n],%d,%d")
#define BUS_LINE_PRINT_FORMAT ("%s,%d,%d\n")

#define MAX_DISTANCE (1000)
#define MIN_DISTANCE (0)
#define MAX_DURATION (100)
#define MIN_DURATION (10)

// Prompt strings for the user
#define LINE_COUNT_PROMPT ("Enter number of lines. Then enter\n")
#define LINE_INFO_PROMPT ("Enter line info. Then enter\n")
#define USAGE_PROMPT ("USAGE: sort_lines [by_name/by_duration/by_distance/test]\n")
#define ERROR_INVALID_LINE_COUNT ("ERROR: Expected positive integer. Try again.\n")
#define ERROR_BUS_NAME_INVALID ("ERROR: Bus name should contain only digits and small chars\n")
#define ERROR_DURATION_INVALID ("ERROR: Duration should be an integer between 10 and 100 (including)\n")
#define ERROR_DISTANCE_INVALID ("ERROR: Distance should be an integer between 0 and 1000 (including)\n")
#define TEST_DESCRIPTION_DISTANCE_SORT ("Distance Sort Test")
#define TEST_DESCRIPTION_DISTANCE_EQ ("Distance Equality Test")
#define TEST_DESCRIPTION_DURATION_SORT ("Duration Sort Test")
#define TEST_DESCRIPTION_DURATION_EQ ("Duration Equality Test")
#define TEST_DESCRIPTION_NAME_SORT ("Name Sort Test")
#define TEST_DESCRIPTION_NAME_EQ ("Name Equality Test")

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

typedef enum TestId
{
	TEST_ID_DISTANCE_SORT = 1,
	TEST_ID_DISTANCE_EQUAL,
	TEST_ID_DURATION_SORT,
	TEST_ID_DURATION_EQUAL,
	TEST_ID_NAME_SORT,
	TEST_ID_NAME_EQUAL

} TestId;

// Macros
//
#define STATUS_FAILED(status) (PROGRAM_STATUS_SUCCESS != (status))

//
#define FREE_MEMORY(ptr)	\
{							\
	if (NULL != (ptr))		\
	{						\
		free(ptr);			\
		(ptr) = NULL;			\
	}						\
}

#define TEST_SUCCESS_PROMPT(num, desc) \
	((void)fprintf(stdout, "TEST %d PASSED: %s\n", (num), (desc)))
#define TEST_FAILED_PROMPT(num, desc) \
	((void)fprintf(stdout, "TEST %d FAILED: %s\n", (num), (desc)))

#define RUN_TEST(test, num, desc)			\
{											\
	if (TEST_FAILED((test)))				\
	{										\
		TEST_FAILED_PROMPT((num), (desc));	\
	}										\
	else									\
	{										\
		TEST_SUCCESS_PROMPT((num), (desc));	\
	}										\
}
 
// Function declarations

ProgramStatus convert_str_to_long(const char* in, long* out);

ProgramStatus get_user_input(
	const char* prompt, char* user_input, int input_max_len);

ProgramStatus bus_line_count_tester(
	char* input, unsigned long* line_count);

ProgramStatus get_bus_line_count(unsigned long* line_count);

ProgramStatus parse_bus_line_input(char* input, BusLine* parsed);

ProgramStatus bus_line_input_tester(
	char* input, BusLine* all_lines, unsigned long current_line);

ProgramStatus get_bus_lines_from_input(
	unsigned long line_count, BusLine* bus_lines);

ProgramStatus get_bus_lines(
	BusLine** bus_lines, unsigned long* count);

void print_bus_lines(BusLine* bus_lines, unsigned long count);

void sort_bus_lines(
	BusLine* lines, unsigned long count, CommandType cmd);

/**
 * Resolving the command type from the given string.
 * @param in - The input string to resolve.
 * @param cmd - The output command type, once resolved.
 * @return Whether the resolution has been successful or not.
 */
ProgramStatus resolve_command_type(char* in, pCommandType cmd);

// Function definitions

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
	const char* prompt, char* user_input, int input_max_len)
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

// See documentation at declaration
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
			LINE_COUNT_PROMPT, 
			user_input, 
			sizeof(user_input));
		if (STATUS_FAILED(status))
		{
			return status;
		}

	} while (STATUS_FAILED(
		bus_line_count_tester(user_input, line_count)));

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}

// See documentation at declaration
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
	if ((BUS_LINE_PARAM_DISTANCE == params_read) || 
		((BUS_LINE_PARAM_DISTANCE < params_read) && 
			(parsed->distance < MIN_DISTANCE || 
				parsed->distance > MAX_DISTANCE)))
	{
		(void)fprintf(stdout, ERROR_DISTANCE_INVALID);
		return status;
	}
	if ((BUS_LINE_PARAM_DURATION == params_read) || 
		((BUS_LINE_PARAM_DURATION < params_read) && 
			(parsed->duration < MIN_DURATION || 
				parsed->duration > MAX_DURATION)))
	{
		(void)fprintf(stdout, ERROR_DURATION_INVALID);
		return status;
	}

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}

// See documentation at declaration
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

	(void)memcpy(
		all_lines + current_line, 
		&local_line_info, 
		sizeof(local_line_info));

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
ProgramStatus get_bus_lines(
	BusLine** bus_lines, unsigned long* count)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	BusLine* lines_local = NULL;
	unsigned long line_count = 0;

	if (NULL == bus_lines || NULL == count)
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
	*count = line_count;
	*bus_lines = lines_local;
	lines_local = NULL;

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	FREE_MEMORY(lines_local);

	return status;
}

// See documentation at declaration
void print_bus_lines(BusLine* bus_lines, unsigned long count)
{
	BusLine* current_line = NULL;
	unsigned long index = 0;

	for (index = 0; index < count; index++)
	{
		current_line = bus_lines + index;
		(void)fprintf(
			stdout, 
			BUS_LINE_PRINT_FORMAT, 
			current_line->name, 
			current_line->distance, 
			current_line->duration);
	}
}

// See documentation at declaration
void sort_bus_lines(
	BusLine* lines, unsigned long count, CommandType cmd)
{
	switch(cmd)
	{
	case COMMAND_BY_DISTANCE:
		quick_sort(
			lines, 
			BUS_LINES_LAST_ELEMENT(lines, count), 
			DISTANCE);
		break;
	case COMMAND_BY_DURATION:
		quick_sort(
			lines, 
			BUS_LINES_LAST_ELEMENT(lines, count), 
			DURATION);
		break;
	case COMMAND_BY_NAME:
		bubble_sort(
			lines, 
			BUS_LINES_LAST_ELEMENT(lines, count));
		break;
	default:
		return;
	}
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
	else
	{
		return PROGRAM_STATUS_FAILED;
	}

	*cmd = out_cmd;

	return PROGRAM_STATUS_SUCCESS;
}


void test_distance(BusLine* lines, BusLine* copy, unsigned long count)
{
	quick_sort(copy, BUS_LINES_LAST_ELEMENT(copy, count), DISTANCE);
	RUN_TEST(
		is_sorted_by_distance(
			copy, BUS_LINES_LAST_ELEMENT(copy, count)),
		TEST_ID_DISTANCE_SORT,
		TEST_DESCRIPTION_DISTANCE_SORT);

	RUN_TEST(is_equal(
		copy,
		BUS_LINES_LAST_ELEMENT(copy, count),
		lines,
		BUS_LINES_LAST_ELEMENT(lines, count)),
		TEST_ID_DISTANCE_EQUAL,
		TEST_DESCRIPTION_DISTANCE_EQ);
}

void test_duration(BusLine* lines, BusLine* copy, unsigned long count)
{
	quick_sort(copy, BUS_LINES_LAST_ELEMENT(copy, count), DURATION);
	RUN_TEST(
		is_sorted_by_duration(
			copy, BUS_LINES_LAST_ELEMENT(copy, count)),
		TEST_ID_DURATION_SORT,
		TEST_DESCRIPTION_DURATION_SORT);

	RUN_TEST(is_equal(
		copy,
		BUS_LINES_LAST_ELEMENT(copy, count),
		lines,
		BUS_LINES_LAST_ELEMENT(lines, count)),
		TEST_ID_DURATION_EQUAL,
		TEST_DESCRIPTION_DURATION_EQ);
}

void test_name(BusLine* lines, BusLine* copy, unsigned long count)
{
	bubble_sort(copy, BUS_LINES_LAST_ELEMENT(copy, count));
	RUN_TEST(
		is_sorted_by_name(
			copy, BUS_LINES_LAST_ELEMENT(copy, count)),
		TEST_ID_NAME_SORT,
		TEST_DESCRIPTION_NAME_SORT);

	RUN_TEST(is_equal(
		copy,
		BUS_LINES_LAST_ELEMENT(copy, count),
		lines,
		BUS_LINES_LAST_ELEMENT(lines, count)),
		TEST_ID_NAME_EQUAL,
		TEST_DESCRIPTION_NAME_EQ);
}

ProgramStatus run_tests(BusLine* lines, unsigned long count)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	unsigned long results = 0;
	BusLine* copy = NULL;

	// Saving the original, before altering via sorting
	copy = (BusLine*)calloc(count, sizeof(*lines));
	if (NULL == copy)
	{
		goto cleanup;
	}
	(void)memcpy(copy, lines, sizeof(*lines) * count);

	test_distance(lines, copy, count);
	test_duration(lines, copy, count);
	test_name(lines, copy, count);

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	FREE_MEMORY(copy);
	return status;
}

ProgramStatus execute_command(CommandType cmd)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	BusLine* lines = NULL;
	unsigned long count = 0;

	status = get_bus_lines(&lines, &count);
	if (STATUS_FAILED(status))
	{
		goto cleanup;
	}

	switch(cmd)
	{
	case COMMAND_BY_DURATION:
		// fallthrough
	case COMMAND_BY_NAME:
		// fallthrough
	case COMMAND_BY_DISTANCE:
		sort_bus_lines(lines, count, cmd);
		print_bus_lines(lines, count);
		break;
	case COMMAND_TEST:
		status = run_tests(lines, count);
		if (STATUS_FAILED(status))
		{
			goto cleanup;
		}
		break;
	}

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	FREE_MEMORY(lines);
	return status;
}

/**
 * TODO add documentation
 */
int main (int argc, char *argv[])
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	CommandType cmd = COMMAND_INVALID;	
	BusLine* bus_lines = NULL;
	unsigned long count = 0;

	if (ARGUMENT_MAX_ARGS != argc)
	{
		(void)fprintf(stdout, USAGE_PROMPT);
		return status;
	}

	status = resolve_command_type(argv[ARGUMENT_COMMAND], &cmd);
	if (STATUS_FAILED(status))
	{
		(void)fprintf(stdout, USAGE_PROMPT);
		return status;
	}

	status = execute_command(cmd);
	if (STATUS_FAILED(status))
	{
		return status;
	}

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}
