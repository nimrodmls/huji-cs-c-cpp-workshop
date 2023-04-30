#include <string.h>
#include "markov_chain.h"

int main(int argc, char* argv)
{
	MarkovChain* chain = NULL;
	create_markov_chain(&chain);
	Node* retval1 = NULL;
	Node* retval2 = NULL;
	char a[] = "name";
	retval1 = add_to_database(chain, a);
	retval2 = get_node_from_database(chain, a);
	
	return 0;
}