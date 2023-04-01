#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H

#include <string.h>

#define NAME_LEN 21
/**
 * TODO add documentation
 */
typedef struct BusLine
{
    char name[NAME_LEN];
    int distance, duration;

} BusLine;

typedef enum SortType
{
    DISTANCE,
    DURATION

} SortType;

/**
 * TODO add documentation
 */
void bubble_sort (BusLine* start, BusLine* end);

/**
 * TODO add documentation
 */
void quick_sort (BusLine* start, BusLine* end, SortType sort_type);

/**
 * TODO add documentation
 */
BusLine *partition (BusLine* start, BusLine* end, SortType sort_type);

#endif //EX2_REPO_SORTBUSLINES_H
