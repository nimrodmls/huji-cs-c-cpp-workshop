#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H

#include "sort_bus_lines.h"

typedef enum TestResult
{
    TEST_RESULT_FAILURE = 0,
    TEST_RESULT_SUCCESS,

} TestResult;

#define TEST_FAILED(result) (TEST_RESULT_SUCCESS != (result))

/**
 * TODO add documentation
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * TODO add documentation
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);

/**
 * TODO add documentation
 */
int is_sorted_by_name (BusLine *start, BusLine *end);

/**
 * TODO add documentation
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);

#endif //EX2_REPO_TESTBUSLINES_H
