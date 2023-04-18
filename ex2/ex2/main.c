#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"

// Constants
#define DECIMAL_BASE (10)
#define INPUT_BUFFER_SIZE_BYTES (61)
#define READ_UNTIL_NEW_LINE ("%[^\n]")
#define BUS_LINE_INPUT_FORMAT ("%20[0-9a-z],%d,%d")
#define BUS_LINE_PRINT_FORMAT ("%s,%d,%d\n")
#define INPUT_FORMAT ("%s")

#define MAX_DISTANCE (1000)
#define MIN_DISTANCE (0)
#define MAX_DURATION (100)
#define MIN_DURATION (10)

// Prompt strings for the user
#define LINE_COUNT_PROMPT ("Enter number of lines. Then enter\n")
#define LINE_INFO_PROMPT ("Enter line info. Then enter\n")
#define USAGE_PROMPT \
	("USAGE: sort_lines [by_name/by_duration/by_distance/test]\n")
#define ERROR_INVALID_LINE_COUNT \
	("ERROR: Expected positive integer. Try again.\n")
#define ERROR_BUS_NAME_INVALID \
	("ERROR: Bus name should contain only digits and small chars\n")
#define ERROR_DURATION_INVALID \
	("ERROR: Duration should be integer 10-100 (including)\n")
#define ERROR_DISTANCE_INVALID \
	("ERROR: Distance should be integer 0-1000 (including)\n")
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

// Typedefs

/**
 * Exit statuses for error indication
 */
typedef enum ProgramStatus
{
	PROGRAM_STATUS_SUCCESS = EXIT_SUCCESS,
	PROGRAM_STATUS_FAILED = EXIT_FAILURE

} ProgramStatus, *pProgramStatus;

/**
 * Command Line argument indices
 */
typedef enum Arguments
{
	ARGUMENT_COMMAND = 1,
	ARGUMENT_MAX_ARGS

} Arguments;

/**
 * Types of commands received in the command line
 */
typedef enum CommandType
{
	COMMAND_INVALID = 0,
	COMMAND_BY_DURATION,
	COMMAND_BY_DISTANCE,
	COMMAND_BY_NAME,
	COMMAND_TEST

} CommandType;

/**
 * Order of parameters received for a single bus line
 */
typedef enum BusLineParams
{
	BUS_LINE_PARAM_NAME = 0,
	BUS_LINE_PARAM_DISTANCE,
	BUS_LINE_PARAM_DURATION,
	BUS_LINE_PARAM_COUNT

} BusLineParams;

/**
 * Numeric identifier for each test
 */
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
 
// Checks if the given status indicates of a failure
#define STATUS_FAILED(status) (PROGRAM_STATUS_SUCCESS != (status))

// Frees a block of memory allocated via malloc,calloc,realloc
#define FREE_MEMORY(ptr)	\
{							\
	if (NULL != (ptr))		\
	{						\
		free(ptr);			\
		(ptr) = NULL;			\
	}						\
}

// Prints success or failure for a given test
#define TEST_SUCCESS_PROMPT(num, desc) \
	((void)fprintf(stdout, "TEST %d PASSED: %s\n", (num), (desc)))
#define TEST_FAILED_PROMPT(num, desc) \
	((void)fprintf(stdout, "TEST %d FAILED: %s\n", (num), (desc)))

// Runs a single test and prints accordingly
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

/**
 * Converting a string to long integer.
 * @param in - The integer as a string.
 * @param out - Dereferenced for the output integer.
 * @return success/failure status.
 */
ProgramStatus convert_str_to_long(const char* in, long* out);

/**
 * Gets a single line of input from the user.
 * @param prompt - The string to show to the user before receiving.
 * @param user_input - Block of memory for the user input.
 * @param input_max_len - The length of user input / max to receive.
 * @return success/failure status.
 */
ProgramStatus get_user_input(
	const char* prompt, char* user_input, int input_max_len);

/**
 * Checking if the line count received from the user is OK.
 * That is, it's a number & it's positive.
 * @param input - The string received from the user.
 * @param line_count - Dereferenced for the output count, on success.
 * @return success/failure status.
 */
ProgramStatus bus_line_count_tester(
	char* input, unsigned long* line_count);

/**
 * Receives the amount of lines from the user.
 * @param line_count - Deferenced for the line count from the user.
 * @return success/failure status.
 */
ProgramStatus get_bus_line_count(unsigned long* line_count);

/**
 * Parsing a single bus line received from the user, to a
 * bus line object.
 * @param input - The string received from the user.
 * @param parsed - The parsed bus line to recieve into, on success.
 * @return success/failure status.
 */
ProgramStatus parse_bus_line_input(char* input, BusLine* parsed);

/**
 * Checking if the bus line input received from the user
 * is up to the standards, if so, add it to the list.
 * @param input - The string received from the user.
 * @param all_lines - The list of lines to copy to, on success.
 * @param current_line - The current line index in the list.
 * @return success/failure status.
 */
ProgramStatus bus_line_input_tester(
	char* input, BusLine* all_lines, unsigned long current_line);

/**
 * Getting the bus lines from the user input, 
 * as per the amount asked. Function is persistent,
 * as long as the input is bad it will keep asking.
 * @param line_count - The amount of lines to receive.
 * @param bus_lines - Ptr to the start of the bus line list.
 *					  The list will be at least as long as
 *					  line_count.
 * @return success/failure status.
 */
ProgramStatus get_bus_lines_from_input(
	unsigned long line_count, BusLine* bus_lines);

/**
 * Receiving all the bus lines.
 * @param bus_lines - Dereferenced for the ptr to the line list.
 * @param count - The amount of lines in bus_lines list.
 * @return success/failure status.
 */
ProgramStatus get_bus_lines(
	BusLine** bus_lines, unsigned long* count);

/**
 * Printing the given bus line list.
 * @param bus_lines - The list of lines to print.
 * @param count - The amount of lines in bus_lines.
 */
void print_bus_lines(BusLine* bus_lines, unsigned long count);

/**
 * Sorting the bus lines according to command type.
 * If the command type is test - No sorting is done,
 * and tests will be run.
 * @param lines - The list of lines to sort.
 * @param count - The amount of lines in lines.
 * @param cmd - The sort / command type.
 */
void sort_bus_lines(
	BusLine* lines, unsigned long count, CommandType cmd);

/**
 * Resolving the command type from the given string.
 * @param in - The input string to resolve.
 * @param cmd - The output command type, once resolved.
 * @return Whether the resolution has been successful or not.
 */
ProgramStatus resolve_command_type(char* in, CommandType* cmd);

/**
 * Running the sorting-by-distance suite.
 * @param lines - The list of lines to test.
 * @param copy - The exact original copy of the given lines list.
 * @param count - Amount of lines in BOTH lines/copy.
 */
void test_distance(BusLine* lines, BusLine* copy, unsigned long count);

/**
 * Running the sorting-by-duration suite.
 * @param lines - The list of lines to test.
 * @param copy - The exact original copy of the given lines list.
 * @param count - Amount of lines in BOTH lines/copy.
 */
void test_duration(BusLine* lines, BusLine* copy, unsigned long count);

/**
 * Running the sorting-by-name suite.
 * @param lines - The list of lines to test.
 * @param copy - The exact original copy of the given lines list.
 * @param count - Amount of lines in BOTH lines/copy.
 */
void test_name(BusLine* lines, BusLine* copy, unsigned long count);

/**
 * Running all the test suites.
 * @param lines - The list of lines to test.
 * @param count - Amount of lines in the list.
 */
ProgramStatus run_tests(BusLine* lines, unsigned long count);

/**
 * Running the given command.
 * @param cmd - The requested command to run.
 */
ProgramStatus execute_command(CommandType cmd);

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

	(void)fprintf(stdout, INPUT_FORMAT, prompt);
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
		(void)fprintf(stdout, ERROR_INVALID_LINE_COUNT);
		return status;
	}

	if (count <= 0)
	{
		(void)fprintf(stdout, ERROR_INVALID_LINE_COUNT);
		return PROGRAM_STATUS_FAILED;
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
		status = PROGRAM_STATUS_FAILED;
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
ProgramStatus resolve_command_type(char* in, CommandType* cmd)
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

// See documentation at declaration
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

// See documentation at declaration
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

// See documentation at declaration
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

// See documentation at declaration
ProgramStatus run_tests(BusLine* lines, unsigned long count)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	BusLine* copy = NULL;

	// Saving the original, before altering via sorting
	copy = (BusLine*)calloc(count, sizeof(*lines));
	if (NULL == copy)
	{
		status = PROGRAM_STATUS_FAILED;
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

// See documentation at declaration
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
	default:
		goto cleanup;
	}

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	FREE_MEMORY(lines);
	return status;
}

/**
 * Executes the main entrypoint of the program.
 */
int main (int argc, char *argv[])
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	CommandType cmd = COMMAND_INVALID;	

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
