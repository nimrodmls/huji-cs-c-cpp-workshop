#include <string.h>
#include <assert.h>

#include "markov_chain.h"

// Constants

// Prompt which indicates of invalid command line
#define USAGE_PROMPT \
	("Usage: ex3 (rand_seed) (tweet_count) (in_file) [max_word_count]\n")
// Prompt which indicates of file reading failure
#define OPEN_FILE_ERROR_PROMPT \
	("Error: Failed to open input text corpus file")
// Format to print a tweet
#define TWEET_PROMPT ("Tweet %lu:")
// Format to print a single word in a tweet
#define TWEET_WORD_FORMAT (" %s")
// Simply a new line
#define NEW_LINE_FORMAT ("\n")
// Minimal argument count, defined outside the enum on purpose
#define MIN_ARGUMENTS (4)
// Maximal amount of characters found in a sentence
//	as defined in the exercise spec
#define MAX_SENTENCE_LENGTH (1000)
// Indicates to read from the file infinitely (until all is read)
#define INFINITE_WORD_COUNT (0)
// Max words in a tweet as defined in the exercise spec
#define TWEET_MAX_WORD_COUNT (20)
// All delimiters found in a sentence, 
//	indicates how sentence is separated
#define SENTENCE_DELIMITERS (" \n\r")
// Format for converstion from string to uint
#define UNSIGNED_INT_FORMAT ("%u")

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
	if (NULL != (file_handle))	\
	{							\
		fclose(file_handle);	\
		(file_handle) = NULL;	\
	}							\
}

// Function declarations

/**
 * Checking if the given string has the given char as
 * the last character in the string
 * @param str - The string to check
 * @param ch - The character to find at the end
 * @return 0 if it doesn't end with the given character, 1 otherwise
 */
static int is_str_endswith(char* str, char ch);

static char* word_copy_callback(char* word);

static bool is_last_word_callback(char* word);

static void word_print_callback(char* word);

/**
 * Converting a string of numericals to an unsigned integer.
 * @param str - The string to convert.
 * @param out - The converted output unsigned integer.
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus str_to_uint(char* str, unsigned int* out);

/**
 * Adding word along with its necessary information to the DB.
 * @param word - The word to add.
 * @param markov_chain - The database object, MUST BE INITIALIZED.
 * @param current_node - The node added to the database (deref out)
 * @param previous_node - The node of the previous word.
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus add_database_info(
	char* word,
	MarkovChain* markov_chain,
	Node** current_node,
	Node* previous_node
);

/**
 * Adding a single sentence to the database.
 * @param sentence - A sequence of words, words are 
 *		separated by space characters.
 * @param markov_chain - The database to add to.
 * @param current_word_count - The current count of 
 *		words added to the DB.
 * @param max_word_count - The maximal word count to add to the DB.
 *		If max is reached, no more words are read.
 *		Specify INFINITE_WORD_COUNT to read without any limit.
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus database_process_sentence(
	char* sentence,
	MarkovChain* markov_chain,
	unsigned int* current_word_count,
	unsigned int max_word_count);

/**
 * Filling the database with words from the given file.
 * @param fp - The file handle to read data from.
 * @param words_to_read - The maximal words to read from the file,
 *		if INFINITE_WORD_COUNT is given, the whole file is read.
 * @param markov_chain - Handle to the Markov Chain to data to.
 * @return Error/Success, check with STATUS_FAILED
 */
static int fill_database(
	FILE* fp, int words_to_read, MarkovChain* markov_chain);

/**
 * Filling the database with words from the given file.
 * @param file_path - Path to the file to open on the filesystem.
 * @param handle - The handle to the opened file.
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus open_file(char* file_path, FILE** handle);

/**
 * Filling the database with words from the given file.
 * @param argv - Arguments received from the command line
 * @param argc - Element count of argv
 * @param seed - Output seed value received
 * @param tweet_count - Output tweet count received
 * @param word_count - Output word count received
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus parse_command_line(
	char** argv,
	int argc,
	unsigned int* seed,
	unsigned int* tweet_count,
	unsigned int* word_count
);

static ProgramStatus create_database(MarkovChain** markov_chain);

/**
 * Filling the database with words from the given file.
 * @param seed - The seed value to generate tweets with
 * @param word_count - Maximal words to read from file
 * @param tweet_count - Amount of tweets to generate
 * @param file_path - Path to the file to read data from
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus run_generator(
	unsigned int seed,
	unsigned int word_count,
	unsigned int tweet_count,
	char* file_path
);

// Function definitions

// See documentation at function declaration
static int is_str_endswith(char* str, char ch)
{
	assert(NULL != str);

	if (0 == strlen(str))
	{
		return 0;
	}

	if (ch != str[strlen(str) - 1])
	{
		return 0;
	}

	return 1;
}

// See documentation at function declaration
static char* word_copy_callback(char* word)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	char* new_word = NULL;
	size_t word_len = 0;

	assert(NULL != word);

	word_len = strlen(word);

	new_word = (char*)calloc(word_len + 1, sizeof(*word));
	if (NULL == new_word)
	{
		(void)fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		goto cleanup;
	}

	memcpy(new_word, word, word_len);

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	if (STATUS_FAILED(status))
	{
		FREE_MEMORY(new_word);
	}

	return new_word;
}

// See documentation at function declaration
static bool is_last_word_callback(char* word)
{
	assert(NULL != word);

	return is_str_endswith(word, SENTENCE_END_CHAR);
}

// See documentation at function declaration
static void word_print_callback(char* word)
{
	assert(NULL != word);

	(void)fprintf(stdout, TWEET_WORD_FORMAT, word);
}

// See documentation at function declaration
static ProgramStatus str_to_uint(char* str, unsigned int* out)
{
	unsigned int result = 0;

	assert(NULL != out);

	if (1 != sscanf(str, UNSIGNED_INT_FORMAT, &result))
	{
		return PROGRAM_STATUS_FAILED;
	}

	*out = result;

	return PROGRAM_STATUS_SUCCESS;
}

// See documentation at function declaration
static ProgramStatus add_database_info(
	char* word,
	MarkovChain* markov_chain,
	Node** current_node,
	Node* previous_node
)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	Node* local_node = NULL;

	assert(NULL != word);
	assert(NULL != markov_chain);
	assert(NULL != current_node);
	
	local_node = add_to_database(markov_chain, word);
	if (NULL == local_node)
	{
		return status;
	}
	if (NULL != previous_node)
	{
		if (!add_node_to_frequencies_list(
			previous_node->data, local_node->data, markov_chain))
		{
			return status;
		}
	}

	*current_node = local_node;

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}

// See documentation at function declaration
static ProgramStatus database_process_sentence(
	char* sentence,
	MarkovChain* markov_chain,
	unsigned int* current_word_count,
	unsigned int max_word_count)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	Node* previous_node = NULL;
	Node* current_node = NULL;
	char* word = NULL;
	unsigned int word_count = 0;
	int end_reached = 0;

	assert(NULL != sentence);
	assert(NULL != markov_chain);
	assert(NULL != current_word_count);

	word_count = *current_word_count;

	word = strtok(sentence, SENTENCE_DELIMITERS);
	while ((NULL != word) && 
		   (word_count < max_word_count || 
			   INFINITE_WORD_COUNT == max_word_count))
	{
		if (end_reached)
		{
			end_reached = 0;
			previous_node = NULL;
		}
		else
		{
			previous_node = current_node;
		}

		status = add_database_info(
			word, markov_chain, &current_node, previous_node);
		if (STATUS_FAILED(status))
		{
			return status;
		}
		end_reached = is_str_endswith(word, SENTENCE_END_CHAR);
		word = strtok(NULL, SENTENCE_DELIMITERS);
		word_count++;
	}

	*current_word_count = word_count;

	status = PROGRAM_STATUS_SUCCESS;
	return status;
}

// See documentation at function declaration
static int fill_database(
	FILE* fp, int words_to_read, MarkovChain* markov_chain)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	char sentence[MAX_SENTENCE_LENGTH] = { 0 };
	unsigned int words_read = 0;
	
	// Reading from the file as long as we didn't reach the end,
	// and the word count hasn't been surpassed (or we received
	// to read inifinitely until file end)
	while ((NULL != fgets(sentence, sizeof(sentence), fp) && 
		   ((words_to_read > (int)words_read) || 
			   (INFINITE_WORD_COUNT == words_to_read))))
	{
		status = database_process_sentence(
			sentence,
			markov_chain,
			&words_read,
			words_to_read);
		if (STATUS_FAILED(status))
		{
			return 1;
		}
	}

	return 0;
}

// See documentation at function declaration
static ProgramStatus open_file(char* file_path, FILE** handle)
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
static ProgramStatus parse_command_line(
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

// See documentation at function declaration
static ProgramStatus create_database(MarkovChain** markov_chain)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	MarkovChain* markov_db = NULL;

	assert(NULL != markov_chain);

	create_markov_chain(&markov_db);
	if (NULL == markov_db)
	{
		return status;
	}

	// Assigning all function pointers
	markov_db->copy_func = (copy_pfn)word_copy_callback;
	markov_db->comp_func = (comp_pfn)strcmp;
	markov_db->free_data = (free_pfn)free;
	markov_db->is_last = (is_last_pfn)is_last_word_callback;
	markov_db->print_func = (print_pfn)word_print_callback;

	*markov_chain = markov_db;
	markov_db = NULL;

	status = PROGRAM_STATUS_SUCCESS;

	return status;
}

// See documentation at function declaration
static ProgramStatus run_generator(
	unsigned int seed, 
	unsigned int word_count, 
	unsigned int tweet_count, 
	char* file_path)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
	MarkovChain* markov_db = NULL;
	unsigned long index = 0;
	FILE* input_file = NULL;

	assert(NULL != file_path);

	status = open_file(file_path, &input_file);
	if (STATUS_FAILED(status))
	{
		goto cleanup;
	}

	status = create_database(&markov_db);
	if (STATUS_FAILED(status))
	{
		goto cleanup;
	}

	if (1 == fill_database(input_file, word_count, markov_db))
	{
		goto cleanup;
	}

	// The input file is no longer required at this stage
	CLOSE_FILE(input_file);

	srand(seed); // Setting the seed before proceeding to 
	// the randomized actions
	for (index = 0; index < tweet_count; index++)
	{
		(void)fprintf(stdout, TWEET_PROMPT, index + 1);
		generate_tweet(markov_db, NULL, TWEET_MAX_WORD_COUNT);
		(void)fprintf(stdout, NEW_LINE_FORMAT);
	}

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	FREE_DATABASE(markov_db);
	CLOSE_FILE(input_file);

	return status;
}

/**
 * The main tweets generator program entrypoint
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of tweets to generate
 *			   3) Input file path
 *			   4) Maximal words to read from file
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char** argv)
{
	ProgramStatus status = PROGRAM_STATUS_FAILED;
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

	status = run_generator(
		seed, word_count, tweet_count, argv[ARGUMENT_TEXT_FILE_INPUT]);
	if (STATUS_FAILED(status))
	{
		goto cleanup;
	}

	status = PROGRAM_STATUS_SUCCESS;

cleanup:
	CLOSE_FILE(input_file);

	return status;
}
