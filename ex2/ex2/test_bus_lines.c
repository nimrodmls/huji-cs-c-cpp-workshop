#include "test_bus_lines.h"

int is_monotonically_increasing(BusLine* start, BusLine* end, SortType field)
{
	size_t line_count = BUS_LIST_SIZE(start, end);
	size_t index = 0;
	int last_value = 0;
	int current_value = 0;

	for (index = 0; index <= line_count; index++)
	{
		current_value = 
			get_bus_line_data(BUS_FROM_INDEX(start, index), field);
		if (last_value > current_value)
		{
			return TEST_RESULT_FAILURE;
		}
		last_value = current_value;
	}

	return TEST_RESULT_SUCCESS;
}

int is_sorted_by_distance(BusLine* start, BusLine* end)
{
	return is_monotonically_increasing(start, end, DISTANCE);
}

int is_sorted_by_duration(BusLine* start, BusLine* end)
{
	return is_monotonically_increasing(start, end, DURATION);
}

int is_sorted_by_name(BusLine* start, BusLine* end)
{
	size_t line_count = BUS_LIST_SIZE(start, end);
	size_t index = 0;
	int compare_result = 0;

	for (index = 0; index < line_count; index++)
	{
		compare_result = strcmp(
			BUS_FROM_INDEX(start, index)->name, 
			BUS_FROM_INDEX(start, index + 1)->name);
		if (0 < compare_result)
		{
			return TEST_RESULT_FAILURE;
		}
	}

	return TEST_RESULT_SUCCESS;
}

int is_equal(
	BusLine* start_sorted, 
	BusLine* end_sorted, 
	BusLine* start_original, 
	BusLine* end_original)
{
	size_t line_count = BUS_LIST_SIZE(start_sorted, end_sorted);
	size_t outer_index = 0;
	size_t inner_index = 0;
	int compare_result = 0;

	// The sizes of both arrays are different, no point to continue
	if (line_count != (size_t)BUS_LIST_SIZE(start_original, end_original))
	{
		return TEST_RESULT_FAILURE;
	}

	for (outer_index = 0; outer_index <= line_count; outer_index++)
	{
		do
		{
			compare_result = strcmp(
				BUS_FROM_INDEX(start_sorted, outer_index)->name,
				BUS_FROM_INDEX(start_original, inner_index)->name);
			inner_index++;
		} while (0 != compare_result && inner_index <= line_count);

		if (0 != compare_result)
		{
			return TEST_RESULT_FAILURE;
		}

		inner_index = 0;
	}

	return TEST_RESULT_SUCCESS;
}
