#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H

#include "sort_bus_lines.h"

/**
 * All the possible tests results
 */
typedef enum TestResult
{
    TEST_RESULT_FAILURE = 0,
    TEST_RESULT_SUCCESS,

} TestResult;

// Macros

/**
 * Checking if the test had failed or not.
 */
#define TEST_FAILED(result) (TEST_RESULT_SUCCESS != (result))

/**
 * Checking if the given bus list is sorted by distance
 * @param start - Ptr to the start of the bus list.
 * @param end - Ptr to the end of the bus list.
 * @return TestResult - Whether test passed or not.
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * Checking if the given bus list is sorted by duration
 * @param start - Ptr to the start of the bus list.
 * @param end - Ptr to the end of the bus list.
 * @return TestResult - Whether test passed or not.
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);

/**
 * Checking if the given bus list is sorted by name.
 * @param start - Ptr to the start of the bus list.
 * @param end - Ptr to the end of the bus list.
 * @return TestResult - Whether test passed or not.
 */
int is_sorted_by_name (BusLine *start, BusLine *end);

/**
 * Checking if the given bus lists are equal
 * Lists are considered equal when the names and size are the same.
 * Duration/distance is not taken into account, neither order.
 * @param start_sorted - Ptr to the start of the sorted bus list.
 * @param end_sorted - Ptr to the end of the sorted bus list.
 * @param start_original - Ptr to the start of the original bus list.
 * @param end_original - Ptr to the end of the original bus list.
 * @return TestResult - Whether test passed or not.
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, 
              BusLine *start_original,
              BusLine *end_original);

#endif //EX2_REPO_TESTBUSLINES_H
