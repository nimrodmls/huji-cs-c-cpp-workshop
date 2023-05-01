#include <assert.h>
#include <string.h>

#include "markov_chain.h"

// Function declarations

bool update_frequencies_list(
	MarkovNode* destination, MarkovNode* new_node);

// Function definitions

// See documentation at declaration
bool update_frequencies_list(
	MarkovNode* destination, MarkovNode* new_node)
{
	MarkovNodeFrequency* new_freq_list = NULL;

	assert(NULL != destination);
	assert(NULL != new_node);

	// Allocating space for the new node in the frequencies list
	// If the list is empty, we need to allocate initial space
	// otherwise, we reallocate the list with extra space so
	// the list can be continuous in the memory
	if (NULL == destination->frequencies_list)
	{
		new_freq_list = (MarkovNodeFrequency*)calloc(
			1, sizeof(*new_freq_list));
	}
	else
	{
		new_freq_list = (MarkovNodeFrequency*)realloc(
			destination->frequencies_list,
			sizeof(*new_freq_list) * (destination->list_len + 1));
	}

	if (NULL == new_freq_list)
	{
		(void)fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		return false;
	}

	destination->frequencies_list = new_freq_list;
	// Since new_freq_list points to the beginning of the list,
	// and we want to update the last element, we reset the ptr
	new_freq_list = new_freq_list + destination->list_len;
	new_freq_list->frequency = 1;
	new_freq_list->markov_node = new_node;
	destination->list_len++;

	return true;
}

// See documentation at header file
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

	// Allocating memory for the word, including null terminator
	new_node->data = (char*)calloc(
		strlen(data_ptr)+1, sizeof(*data_ptr));
	if (NULL == new_node->data)
	{
		(void)fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		goto cleanup;
	}
	(void)memcpy(new_node->data, data_ptr, strlen(data_ptr));
	
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
	bool status = false;
	unsigned int found_entry = 0;
	MarkovNodeFrequency* new_freq_list = NULL;

	assert(NULL != first_node);
	assert(NULL != second_node);

	// First we look up the second node in the frequencies list
	while ((found_entry < first_node->list_len) &&
		(0 != strcmp(
			GET_FREQUENCY_NODE(
				first_node,
				found_entry).markov_node->data,
			second_node->data)))
	{
		found_entry++;
	}

	// We found the entry in the frequency list,
	// just need to increment the counter
	if (found_entry != first_node->list_len)
	{
		GET_FREQUENCY_NODE(first_node, found_entry).frequency++;
	}
	else // Otherwise, add it
	{
		return update_frequencies_list(first_node, second_node);
	}

	return true;
}

// See documentation at header file
void free_database(MarkovChain** ptr_chain)
{
	int index = 0;
	LinkedList* db = NULL;
	Node* current_node = NULL;
	Node* temp_node = NULL;

	assert(NULL != ptr_chain);
	assert(NULL != *ptr_chain);

	db = (*ptr_chain)->database;
	current_node = db->first;

	for (index = 0; index < db->size; index++)
	{

		if (NULL != current_node->data)
		{
			FREE_MEMORY(current_node->data->frequencies_list);
			FREE_MEMORY(current_node->data->data);
		}
		FREE_MEMORY(current_node->data);

		temp_node = current_node->next;
		FREE_MEMORY(current_node);
		
		current_node = temp_node;
	}

	FREE_MEMORY(db);
	FREE_MEMORY(*ptr_chain);
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