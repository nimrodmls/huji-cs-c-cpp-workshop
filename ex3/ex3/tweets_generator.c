#include <string.h>
#include <assert.h>

#include "markov_chain.h"

#define USAGE_PROMPT \
	("Usage: ex3 (rand_seed) (tweet_count) (in_file) [max_word_count]\n")

// Minimal argument count, defined outside the enum on purpose
#define MIN_ARGUMENTS (4)

#define INFINITE_WORD_COUNT (0)

/**
 * Command line arguments indices
 */
typedef enum CommandArguments
{
	ARGUMENT_EXECUTABLE = 0,
	ARGUMENT_SEED,
	ARGUMENT_TWEET_COUNT,
	ARGUMENT_TEXT_FILE_INPUT,
	ARGUMENT_WORD_READ_COUNT,

	MAX_ARGUMENTS

} CommandArguments;

/**
 * Exit statuses for error indication
 */
typedef enum ProgramStatus
{
	PROGRAM_STATUS_SUCCESS = EXIT_SUCCESS,
	PROGRAM_STATUS_FAILED = EXIT_FAILURE

} ProgramStatus;

// Macros

// Checks if the given status indicates of a failure
#define STATUS_FAILED(status) (PROGRAM_STATUS_SUCCESS != (status))

// Function declarations

ProgramStatus str_to_uint(char* str, unsigned int* out);

ProgramStatus parse_command_line(
	char** argv,
	int argc,
	unsigned int* seed,
	unsigned int* tweet_count,
	unsigned int* word_count
);

// Function definitions

// See documentation at function declaration
ProgramStatus str_to_uint(char* str, unsigned int* out)
{
	unsigned int result = 0;

	assert(NULL != out);

	if (1 != sscanf(str, "%u", &result))
	{
		return PROGRAM_STATUS_FAILED;
	}

	*out = result;

	return PROGRAM_STATUS_SUCCESS;
}

// See documentation at function declaration
ProgramStatus parse_command_line(
	char** argv,
	int argc,
	unsigned int* seed,
	unsigned int* tweet_count,
	unsigned int* word_count)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;

	assert(NULL != argv);
	assert(NULL != seed);
	assert(NULL != tweet_count);
	assert(NULL != word_count);

	*word_count = INFINITE_WORD_COUNT;

	// Either in case of minimal / maximal arguments, the seed and
	// tweet count are mandatory
	status = str_to_uint(argv[ARGUMENT_SEED], seed);
	if (STATUS_FAILED(status))
	{
		return status;
	}

	status = str_to_uint(argv[ARGUMENT_TWEET_COUNT], tweet_count);
	if (STATUS_FAILED(status))
	{
		return status;
	}

	// Now we deal with the special case of word count,
	// if minimal arguments received, word count is set to inifite
	if (MAX_ARGUMENTS == argc)
	{
		status = str_to_uint(argv[ARGUMENT_WORD_READ_COUNT], word_count);
		if (STATUS_FAILED(status))
		{
			return status;
		}
	}

	status = PROGRAM_STATUS_SUCCESS;

	return status;
}

int main(int argc, char** argv)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	unsigned int seed = 0;
	unsigned int tweet_count = 0;
	unsigned int word_count = 0;

	assert(NULL != argv);

	if ((MIN_ARGUMENTS > argc) || (MAX_ARGUMENTS < argc))
	{
		(void)fprintf(stdout, USAGE_PROMPT);
		goto cleanup;
	}

	status = parse_command_line(
		argv, argc, &seed, &tweet_count, &word_count);
	if (STATUS_FAILED(status))
	{
		goto cleanup;
	}

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	return status;
}

int mainz(int argc, char* argv)
{
	MarkovChain* chain = NULL;
	create_markov_chain(&chain);
	Node* retval1 = NULL;
	Node* retval2 = NULL;
	Node* retval3 = NULL;
	char a[] = "name1";
	retval1 = add_to_database(chain, a);
	char b[] = "name2";
	retval2 = add_to_database(chain, b);
	char c[] = "name3";
	retval3 = add_to_database(chain, c);
	add_node_to_frequencies_list(retval1->data, retval2->data);
	add_node_to_frequencies_list(retval1->data, retval3->data);
	add_node_to_frequencies_list(retval1->data, retval3->data);
	add_node_to_frequencies_list(retval1->data, retval3->data);
	add_node_to_frequencies_list(retval1->data, retval2->data);
	free_database(&chain);
	
	return 0;
}