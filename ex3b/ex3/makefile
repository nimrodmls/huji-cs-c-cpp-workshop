HEADERS = linked_list.h markov_chain.h
COMMON_SRC = linked_list.c markov_chain.c

tweets: tweets_generator.c $(HEADERS) $(COMMON_SRC)
	gcc -Wall -Wextra -Wvla -std=c99 tweets_generator.c markov_chain.c linked_list.c -o tweets

snakes: snakes_and_ladders.c $(HEADERS) $(COMMON_SRC)
	gcc -Wall -Wextra -Wvla -std=c99 snakes_and_ladders.c markov_chain.c linked_list.c -o snakes
