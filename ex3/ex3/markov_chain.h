#ifndef _MARKOV_CHAIN_H_
#define _MARKOV_CHAIN_H_

#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool

#include "linked_list.h"

// Constants

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"
#define SENTENCE_END_CHAR ('.')

// Macros

/**
 * Macro for freeing memory allocations safetly
 */
#define FREE_MEMORY(ptr)	\
{							\
	if (NULL != (ptr))		\
	{						\
		free(ptr);			\
		(ptr) = NULL;			\
	}						\
}
 /**
  * Macro for freeing the markov DB safetly
  */
#define FREE_DATABASE(db_ptr)	\
{								\
	if (NULL != (db_ptr))			\
	{							\
		free_database(&(db_ptr));	\
		(db_ptr) = NULL;			\
	}							\
}

#define GET_FREQUENCY_NODE(node, index) (node->frequencies_list[index])

// Typedefs

/**
 * MarkovChain object struct
 */
typedef struct MarkovChain
{
	LinkedList* database;

} MarkovChain;

// Foward declaration for MarkovNode
typedef struct MarkovNodeFrequency MarkovNodeFrequency;

/**
 * A single node in the Markov Chain database
 */
typedef struct MarkovNode
{
	char* data;
	MarkovNodeFrequency* frequencies_list;
	// The number of entries in the list
	unsigned int list_len;
	// Counts the total amount of words and their occurances
	// in the frequencies list
	unsigned int total_occurances;

} MarkovNode;

/**
 * The frequencies list of a single Markov Node
 */
typedef struct MarkovNodeFrequency
{
	MarkovNode* markov_node;
	unsigned long frequency;

} MarkovNodeFrequency;

// Functions

int is_str_endswith(char* str, char ch);

void create_markov_chain(MarkovChain** chain);

/**
 * If data_ptr in markov_chain, return it's node. Otherwise, create
 * new node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's
 *	database, returns NULL in case of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr);

/**
 * Check if data_ptr is in database.
 * If so, return the markov_node wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state,
 *	NULL if state not in database.
 */
Node* get_node_from_database(
	MarkovChain *markov_chain, char *data_ptr);

Node* get_node_from_database_index(
	MarkovChain* markov_chain, unsigned int index);

/**
 * Add the second markov_node to the frequency list
 *	of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node
 * @param second_node
 * @return success/failure: true if the process was successful,
 *	false if in case of allocation error.
 */
bool add_node_to_frequencies_list(
	MarkovNode *first_node, MarkovNode *second_node);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain - markov chain to free
 */
void free_database(MarkovChain ** ptr_chain);

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain);

/**
 * Choose randomly the next state,
 *	depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_tweet(
	MarkovChain* markov_chain, 
	MarkovNode* first_node, 
	int max_length
);


#endif /* _MARKOV_CHAIN_H_ */
