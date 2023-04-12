#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H

#include <string.h>

// Constants
#define NAME_LEN 21

// Macros

// Getting a bus line by index from a list
#define BUS_FROM_INDEX(start, index) ((BusLine *)(start + index))

// Calculating the size of the bus line list.
#define BUS_LIST_SIZE(start, end) (end - start)

// Getting the last line in a list of bus lines.
#define BUS_LINES_LAST_ELEMENT(first, count) ((first) + (count) - 1)

// Typedefs

/**
 * A bus line object.
 */
typedef struct BusLine
{
    char name[NAME_LEN];
    int distance, duration;

} BusLine;

/**
 * The types of sorting.
 */
typedef enum SortType
{
    DISTANCE,
    DURATION

} SortType;

/**
 * Extracting data from the bus line, according to the sort type.
 * @param bus_line - The bus line to extract data from.
 * @param data_type - The type of data, intertwined with the sort.
 * @return The requested data.
 */
int get_bus_line_data(BusLine* bus_line, SortType data_type);

/**
 * Sorting the given bus line list by names, 
 * via bubble sort algorithm
 * @param start - The beginning of the bus line list.
 * @param end - The end of the bus line list.
 */
void bubble_sort (BusLine* start, BusLine* end);

/**
 * Sorting the given bus line list by duration/distance,
 * via the quick sort algorithm
 * @param start - The beginning of the bus line list.
 * @param end - The end of the bus line list.
 * @param sort_type - The type of sorting (distance/duration).
 */
void quick_sort (BusLine* start, BusLine* end, SortType sort_type);

/**
 * Partitioning the list of bus lines for the quick sort algorithm
 * @param start - The beginning of the bus line list.
 * @param end - The end of the bus line list.
 * @param sort_type - The type of sorting (distance/duration).
 */
BusLine *partition (BusLine* start, BusLine* end, SortType sort_type);

#endif //EX2_REPO_SORTBUSLINES_H
