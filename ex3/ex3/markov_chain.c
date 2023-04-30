#include <assert.h>
#include <string.h>

#include "markov_chain.h"

void create_markov_chain(MarkovChain** chain)
{
	MarkovChain* new_chain = NULL;
	LinkedList* inner_list = NULL;

	assert(NULL != chain);

	new_chain = (MarkovChain*)calloc(1, sizeof(MarkovChain));
	if (NULL == new_chain)
	{
		(void)fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		goto cleanup;
	}

	inner_list = 
		(LinkedList*)calloc(1, sizeof(*new_chain->database));
	if (NULL == inner_list)
	{
		(void)fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		goto cleanup;
	}

	// Transferring ownership
	new_chain->database = inner_list;
	inner_list = NULL;
	*chain = new_chain;
	new_chain = NULL;

cleanup:
	FREE_MEMORY(inner_list);
	FREE_MEMORY(new_chain);

	return new_chain;
}
// See documentation at header file
Node* add_to_database(MarkovChain* markov_chain, char* data_ptr)
{
	Node* current_node = NULL;
	MarkovNode* new_node = NULL;

	assert(NULL != markov_chain);
	assert(NULL != data_ptr);

	current_node = get_node_from_database(markov_chain, data_ptr);
	// The data is IN the database, we just return the relevant node
	if (NULL != current_node)
	{
		goto cleanup;
	}

	// The data is NOT in the database, we need to add it
	new_node = (MarkovNode*)calloc(1, sizeof(*new_node));
	if (NULL == new_node)
	{
		(void)fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		goto cleanup;
	}

	new_node->data = data_ptr;
	if (0 != add(markov_chain->database, new_node))
	{
		goto cleanup;
	}
	// Ownership transferred
	new_node = NULL;

	current_node = markov_chain->database->last;

cleanup:
	FREE_MEMORY(new_node);

	return current_node;
}

// See documentation at header file
Node* get_node_from_database(
	MarkovChain* markov_chain, char* data_ptr)
{
	Node* current_node = NULL;

	assert(NULL != markov_chain);
	assert(NULL != data_ptr);

	current_node = markov_chain->database->first;
	while ((NULL != current_node) &&
		   (0 != strcmp(current_node->data->data, data_ptr)))
	{
		current_node = current_node->next;
	}

	return current_node;
}

// See documentation at header file
bool add_node_to_frequencies_list(
	MarkovNode* first_node, MarkovNode* second_node)
{
	
}

// See documentation at header file
void free_database(MarkovChain** ptr_chain)
{
	
}

// See documentation at header file
MarkovNode* get_first_random_node(MarkovChain* markov_chain)
{
	
}

// See documentation at header file
MarkovNode* get_next_random_node(MarkovNode* state_struct_ptr)
{
	
}

// See documentation at header file
void generate_tweet(
	MarkovChain* markov_chain,
	MarkovNode* first_node,
	int max_length
)
{
	
}