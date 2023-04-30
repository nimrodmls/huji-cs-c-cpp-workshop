#include <string.h>
#include "markov_chain.h"

int main(int argc, char* argv)
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
	
	return 0;
}