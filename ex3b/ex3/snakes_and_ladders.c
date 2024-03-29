#include <string.h> // For strlen(), strcmp(), strcpy()
#include <assert.h>

#include "markov_chain.h"

// Retrieves the maximum of 2 numbers
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

// Default for empty Snake/Ladder value
#define EMPTY -1
// Size of Snakes and Ladders Game Board
#define BOARD_SIZE 100
// Maximal length of a route on the board
#define MAX_GENERATION_LENGTH 60
// The maximal values for a dice roll
#define DICE_MAX 6
// Transition count on the board (Snake/Ladder)
#define NUM_OF_TRANSITIONS 20
// Prompt for the user, shown on invalid command line
#define USAGE_PROMPT ("Usage: ex3 (rand_seed) (route_count)\n")
// Initial prompt for a route
#define ROUTE_PROMPT ("Random Walk %lu: ")
// Print format for a ladder transition in a route
#define LADDER_PRINT_FORMAT ("[%d]-ladder to %d -> ")
// Print format for a snake transition in a route
#define SNAKE_PRINT_FORMAT ("[%d]-snake to %d -> ")
// Print format for regular board movement
#define NORMAL_PRINT_FORMAT ("[%d] -> ")
// Print format for the last movement on a board
#define END_PRINT_FORMAT ("[%d]")
// Simply a new line
#define NEW_LINE_FORMAT ("\n")
// Format for converstion from string to uint
#define UNSIGNED_INT_FORMAT ("%u")

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from 
 * x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

// Typedefs

/**
 * Command line arguments indices
 */
typedef enum CommandArguments
{
    ARGUMENT_EXECUTABLE = 0,
    ARGUMENT_SEED,
    ARGUMENT_ROUTE_COUNT,

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

// Checks if the given status indicates of a failure
#define STATUS_FAILED(status) (PROGRAM_STATUS_SUCCESS != (status))

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    // ladder_to represents the jump of the ladder 
    // in case there is one from this square
    int ladder_to;
    // snake_to represents the jump of the snake in
    // case there is one from this square
    int snake_to;
    //both ladder_to and snake_to should be -1 
    // if the Cell doesn't have them
} Cell;

// Function declarations

/**
 * Callback for the Markov Chain to handle printing of a route step
 * @param cell - The cell which we moved onto
 */
static void cell_print_callback(Cell* cell);

/**
 * Callback for the Markov Chain to handle comparison of 2 cells
 * @param cell_1 - Cell to compare to cell_2
 * @param cell_1 - Cell to compare to cell_1
 * @return 0 if equal
        > 1 if cell_1 is larger
        < 1 if cell_1 is smaller
 */
static int cell_compare_callback(Cell* cell_1, Cell* cell_2);

/**
 * Copying a cell to a new memory block
 * @param cell - The cell to copy
 * @return The new allocated cell, IT MUST BE FREED USING
 *      THE FREE CALLBACK!
 */
static Cell* cell_copy_callback(Cell* cell);

/**
 * Checking if the given cell is indicated as a last cell
 * @param cell - The cell to check
 * @return true on last, false otherwise
 */
static bool is_last_cell_callback(Cell* cell);

/**
 * Handles some errors idk
 * @param error_msg - Stuff
 * @param database - The markov chain database
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int handle_error(char* error_msg, MarkovChain** database);

/**
 * Creates a new empty game board
 * @param cells - The cells map
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int create_board(Cell* cells[BOARD_SIZE]);

/**
 * Initializes some stuff in the database with the board
 * @param markov_chain - The chain to initialize
 * @param cells - The cells map
 */
static void initialize_board(
    MarkovChain* markov_chain,
    Cell** cells);

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain* markov_chain);

/**
 * Converting a string of numericals to an unsigned integer.
 * @param str - The string to convert.
 * @param out - The converted output unsigned integer.
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus str_to_uint(char* str, unsigned int* out);

/**
 * Parsing the given command line and giving out the parameters
 * @param argv - The given parameters, expects AT LEAST 3 VALUES
 * @param seed - The seed value given in the command line
 * @param route_count - The amount of routes to generate given
 *      in the command line.
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus parse_command_line(
    char** argv,
    unsigned int* seed,
    unsigned int* route_count);

/**
 * Creates a blank new Markov Chain database and sets its callbacks
 * @param markov_chain - The created markov chain, must be released
 *      with FREE_DATABASE upon success.
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus create_database(MarkovChain** markov_chain);

/**
 * Runs the route generator for Snakes and Ladders game
 * @param seed - The randomization seed
 * @param route_count - Amount of routes to generate
 * @return Error/Success, check with STATUS_FAILED
 */
static ProgramStatus run_generator(
    unsigned int seed, unsigned int route_count);

// Function definitions

// See documentation at function declaration
static void cell_print_callback(Cell* cell)
{
    assert(NULL != cell);

    if (EMPTY != cell->ladder_to) // There is a ladder
    {
        (void)fprintf(
            stdout, LADDER_PRINT_FORMAT, 
            cell->number, cell->ladder_to);
    }
    else if (EMPTY != cell->snake_to) // There is a Snake
    {
        (void)fprintf(
            stdout, SNAKE_PRINT_FORMAT, 
            cell->number, cell->snake_to);
    }
    else // It's a normal cell
    {
        if (BOARD_SIZE == cell->number)
        {
            (void)fprintf(stdout, END_PRINT_FORMAT, cell->number);
        }
        else
        {
            (void)fprintf(stdout, NORMAL_PRINT_FORMAT, cell->number);
        }
    }
}

// See documentation at function declaration
static int cell_compare_callback(Cell* cell_1, Cell* cell_2)
{
    assert(NULL != cell_1);
    assert(NULL != cell_2);
    
    return cell_1->number - cell_2->number;
}

// See documentation at function declaration
static Cell* cell_copy_callback(Cell* cell)
{
    ProgramStatus status = PROGRAM_STATUS_FAILED;
    Cell* new_cell = NULL;

    assert(NULL != cell);

    new_cell = (Cell*)calloc(1, sizeof(*cell));
    if (NULL == new_cell)
    {
        (void)fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        goto cleanup;
    }

    memcpy(new_cell, cell, sizeof(*cell));

    status = PROGRAM_STATUS_SUCCESS;

cleanup:
    if (STATUS_FAILED(status))
    {
        FREE_MEMORY(new_cell);
    }

    return new_cell;
}

// See documentation at function declaration
static bool is_last_cell_callback(Cell* cell)
{
    assert(NULL != cell);

    return cell->number == BOARD_SIZE;
}

// See documentation at function declaration
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_database(database);
    }
    return EXIT_FAILURE;
}

// See documentation at function declaration
static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

// See documentation at function declaration
static void initialize_board(
    MarkovChain* markov_chain,
    Cell** cells)
{
    MarkovNode* from_node = NULL, * to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node =
            get_node_from_database(markov_chain, cells[i])->data;

        if (cells[i]->snake_to != EMPTY ||
            cells[i]->ladder_to != EMPTY)
        {
            index_to =
                MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(
                markov_chain, cells[index_to])->data;
            add_node_to_frequencies_list(
                from_node, to_node, markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to =
                    ((Cell*)(from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node =
                    get_node_from_database(
                        markov_chain, cells[index_to])->data;
                add_node_to_frequencies_list(
                    from_node, to_node, markov_chain);
            }
        }
    }
}

// See documentation at function declaration
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    initialize_board(markov_chain, cells);

    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
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
static ProgramStatus parse_command_line(
    char** argv,
    unsigned int* seed,
    unsigned int* route_count)
{
    ProgramStatus status = PROGRAM_STATUS_FAILED;

    assert(NULL != argv);
    assert(NULL != seed);
    assert(NULL != route_count);

    // Either in case of minimal / maximal arguments, the seed and
    // tweet count are mandatory
    status = str_to_uint(argv[ARGUMENT_SEED], seed);
    if (STATUS_FAILED(status))
    {
        return status;
    }

    status = str_to_uint(argv[ARGUMENT_ROUTE_COUNT], route_count);
    if (STATUS_FAILED(status))
    {
        return status;
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
    markov_db->copy_func = (copy_pfn)cell_copy_callback;
    markov_db->comp_func = (comp_pfn)cell_compare_callback;
    markov_db->free_data = (free_pfn)free;
    markov_db->is_last = (is_last_pfn)is_last_cell_callback;
    markov_db->print_func = (print_pfn)cell_print_callback;

    *markov_chain = markov_db;
    markov_db = NULL;

    status = PROGRAM_STATUS_SUCCESS;

    return status;
}

// See documentation at function declarations
static ProgramStatus run_generator(
    unsigned int seed, unsigned int route_count)
{
    ProgramStatus status = PROGRAM_STATUS_FAILED;
    unsigned long index = 0;
    MarkovChain* markov_db = NULL;

    status = create_database(&markov_db);
    if (STATUS_FAILED(status))
    {
        goto cleanup;
    }

    if (EXIT_FAILURE == fill_database(markov_db))
    {
        goto cleanup;
    }

    srand(seed); // Setting the seed before proceeding to 
    // the randomized actions
    for (index = 0; index < route_count; index++)
    {
        (void)fprintf(stdout, ROUTE_PROMPT, index + 1);
        generate_tweet(
            markov_db, 
            markov_db->database->first->data, 
            MAX_GENERATION_LENGTH);
        (void)fprintf(stdout, NEW_LINE_FORMAT);
    }

    status = PROGRAM_STATUS_SUCCESS;

cleanup:
    FREE_DATABASE(markov_db);

    return status;
}

/**
 * The main Snakes & Ladders program entrypoint
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    ProgramStatus status = PROGRAM_STATUS_FAILED;
    unsigned int seed = 0;
    unsigned int route_count = 0;

    if (MAX_ARGUMENTS != argc)
    {
        (void)fprintf(stdout, USAGE_PROMPT);
        goto cleanup;
    }

    status = parse_command_line(argv, &seed, &route_count);
    if (STATUS_FAILED(status))
    {
        goto cleanup;
    }

    status = run_generator(seed, route_count);
    if (STATUS_FAILED(status))
    {
        goto cleanup;
    }

    status = PROGRAM_STATUS_SUCCESS;

cleanup:
    return status;
}
