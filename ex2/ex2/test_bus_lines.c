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
	return TEST_RESULT_SUCCESS;
}

int is_equal(BusLine* start_sorted, BusLine* end_sorted, BusLine* start_original, BusLine* end_original)
{
	return TEST_RESULT_SUCCESS;
}
