#ifndef _MARKOV_CHAIN_H_
#define _MARKOV_CHAIN_H_

#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool

#include "linked_list.h"

// Constants

// Allocation error message prompt
#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"
// Character which marks the end of the sentence
#define SENTENCE_END_CHAR ('.')

// Macros

#define GENERIC_FREE(func, ptr)	\
{								\
	if (NULL != (ptr))			\
	{							\
		func(ptr);				\
		(ptr) = NULL;			\
	}							\
}

/**
 * Macro for freeing memory allocations safetly
 */
#define FREE_MEMORY(ptr)		\
{								\
	GENERIC_FREE(free, (ptr));	\
}

 /**
  * Macro for freeing the markov DB safetly
  */
#define FREE_DATABASE(db_ptr)	\
{								\
	if (NULL != (db_ptr))		\
	{							\
		free_database(&db_ptr);	\
		(db_ptr) = NULL;		\
	}							\
}

/**
 * Compact way to access a particular frequency element at index
 */
#define GET_FREQUENCY_NODE(node, index) \
	(node->frequencies_list[index])

// Typedefs

// Function callback types
typedef void (* print_pfn)(void *);
typedef int (* comp_pfn)(void*, void*);
typedef void (* free_pfn)(void *);
typedef void* (* copy_pfn)(void *);
typedef bool (* is_last_pfn)(void *);

/**
 * MarkovChain object struct
 */
typedef struct MarkovChain
{
	LinkedList* database;

    // pointer to a func that receives data from a 
	// generic type and prints it. returns void.
    print_pfn print_func;

    // pointer to a func that gets 2 pointers of generic 
	// data type(same one) and compare between them
    // returns: - a positive value if the first is bigger
    //          - a negative value if the second is bigger
    //          - 0 if equal
    comp_pfn comp_func;

    // a pointer to a function that gets a pointer of 
	// generic data type and frees it. returns void.
    free_pfn free_data;

    // a pointer to a function that  gets a pointer of generic data
	// type and returns a newly allocated copy of it 
	// returns a generic pointer.
    copy_pfn copy_func;

    //  a pointer to function that gets a pointer of generic 
	//  data type and returns:
    //      - true if it's the last state.
    //      - false otherwise.
    is_last_pfn is_last;
} MarkovChain;

// Foward declaration for MarkovNode
typedef struct MarkovNodeFrequency MarkovNodeFrequency;

/**
 * A single node in the Markov Chain database
 */
typedef struct MarkovNode
{
	void* data;
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

/**
 * Creating a blank Markov Chain
 * Must call FREE_DATABASE after upon valid ptr returned 
 * in chain param
 * @param chain - Out param for the created chain
 */
void create_markov_chain(MarkovChain** chain);

/**
 * If data_ptr in markov_chain, return it's node. Otherwise, create
 * new node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's
 *	database, returns NULL in case of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, void* data_ptr);

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
	MarkovChain *markov_chain, void* data_ptr);

/**
 * Getting a node from the linked list of the database
 * based on its index
 * @param markov_chain - The chain to iterate
 * @param index - The index to extract
 * @return The node at the corresponding index
 */
Node* get_node_from_database_index(
	MarkovChain* markov_chain, unsigned int index);

/**
 * Add the second markov_node to the frequency list
 *	of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node - The node to add the second node as frequency
 * @param second_node - The node added to the first node's frequency
 * @param markov_chain - The markov chain connected to the given node
 * @return success/failure: true if the process was successful,
 *	false if in case of allocation error.
 */
bool add_node_to_frequencies_list(
	MarkovNode* first_node, 
	MarkovNode* second_node,
	MarkovChain* markov_chain);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain - markov chain to free
 */
void free_database(MarkovChain ** ptr_chain);

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain - The chain to extract random node from
 * @return a random node from the chain
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