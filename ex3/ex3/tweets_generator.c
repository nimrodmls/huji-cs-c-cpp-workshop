#include <string.h>
#include <assert.h>

#include "markov_chain.h"

#define USAGE_PROMPT \
	("Usage: ex3 (rand_seed) (tweet_count) (in_file) [max_word_count]\n")
#define OPEN_FILE_ERROR_PROMPT \
	("Error: Failed to open input text corpus file")

// Minimal argument count, defined outside the enum on purpose
#define MIN_ARGUMENTS (4)

#define MAX_SENTENCE_LENGTH (1000)
#define MAX_WORD_LENGTH (100)
#define INFINITE_WORD_COUNT (0)

#define SPACE_DELIMITER (" ")

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

// Safe macro for closing an open file
#define CLOSE_FILE(file_handle) \
{								\
	if (NULL != file_handle)	\
	{							\
		fclose(file_handle);	\
		file_handle = NULL;		\
	}							\
}

// Function declarations

ProgramStatus str_to_uint(char* str, unsigned int* out);

ProgramStatus database_process_sentence(
	char* sentence,
	MarkovChain* markov_chain,
	unsigned int* words_count);

int fill_database(
	FILE* fp, int words_to_read, MarkovChain* markov_chain);

ProgramStatus open_file(char* file_path, FILE** handle);

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
ProgramStatus database_process_sentence(
	char* sentence,
	MarkovChain* markov_chain,
	unsigned int* words_count)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	Node* previous_node = NULL;
	Node* current_node = NULL;
	char* word = NULL;
	unsigned int word_count = 0;

	assert(NULL != sentence);
	assert(NULL != markov_chain);
	assert(NULL != words_count);

	// Iterating on all words in the sentence
	word = strtok(sentence, SPACE_DELIMITER);
	while (NULL != word)
	{
		previous_node = current_node;
		current_node = add_to_database(markov_chain, word);
		if (NULL == current_node)
		{
			return status;
		}
		if (NULL != previous_node)
		{
			if (!add_node_to_frequencies_list(
					current_node->data, previous_node->data))
			{
				return status;
			}
		}
		word_count++;
	}

	*words_count = word_count;

	status = PROGRAM_STATUS_SUCCESS;

	return status;
}

// See documentation at function declaration
int fill_database(
	FILE* fp, int words_to_read, MarkovChain* markov_chain)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	char sentence[MAX_SENTENCE_LENGTH] = { 0 };
	unsigned int words_read = 0;
	
	// Reading from the file as long as we didn't reach the end,
	// and the word count hasn't been surpassed (or we received
	// to read inifinitely until file end)
	while ((NULL != fgets(sentence, sizeof(sentence), fp) && 
		   ((words_to_read < (int)words_read) || 
			   (INFINITE_WORD_COUNT == words_to_read))))
	{
		status = database_process_sentence(
			sentence, markov_chain, &words_read);
		if (STATUS_FAILED(status))
		{
			return 1;
		}
	}

	return 0;
}

// See documentation at function declaration
ProgramStatus open_file(char* file_path, FILE** handle)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	FILE* file_handle = NULL;

	assert(NULL != file_path);
	assert(NULL != handle);

	file_handle = fopen(file_path, "r");
	if (NULL == file_handle)
	{
		(void)fprintf(stdout, OPEN_FILE_ERROR_PROMPT);
		goto cleanup;
	}
	
	*handle = file_handle;
	file_handle = NULL;

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	CLOSE_FILE(file_handle);

	return status;
	
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
	MarkovChain* markov_db = NULL;
	FILE* input_file = NULL;
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

	status = open_file(argv[ARGUMENT_TEXT_FILE_INPUT], &input_file);
	if (STATUS_FAILED(status))
	{
		goto cleanup;
	}

	create_markov_chain(&markov_db);
	if (NULL == markov_db)
	{
		goto cleanup;
	}

	if (1 == fill_database(input_file, word_count, markov_db))
	{
		goto cleanup;
	}

	// The input file is no longer required at this stage
	CLOSE_FILE(input_file);

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	FREE_DATABASE(markov_db);
	CLOSE_FILE(input_file);

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