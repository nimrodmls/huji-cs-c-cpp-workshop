#include <stdio.h>

#include "sort_bus_lines.h"

// Internal function declarations

/**
 * Swapping 2 bus lines in the memory.
 * @param line1/line2 - The lines to swap.
 */
void swap_lines(BusLine* line1, BusLine* line2);

// Function definitions

// See documentation at declaration
void swap_lines(BusLine* line1, BusLine* line2)
{
	BusLine temp = *line1;
	memcpy(line1, line2, sizeof(*line1));
	memcpy(line2, &temp, sizeof(*line2));
}

// See documentation at header file
int get_bus_line_data(BusLine* bus_line, SortType data_type)
{
	switch (data_type)
	{
	case DISTANCE:
		return bus_line->distance;

	case DURATION:
		return bus_line->duration;

	default:
		return 0;
	}
}

// See documentation at header file
void bubble_sort(BusLine* start, BusLine* end)
{
	size_t line_count = BUS_LIST_SIZE(start, end);
	size_t outer_index = 0;
	size_t inner_index = 0;
	int compare_result = 0;

	if ((NULL == start) || (NULL == end))
	{
		return;
	}

	for (outer_index = 0; 
		 outer_index < line_count;
		 outer_index++)
	{
		for (inner_index = 0;
			 inner_index < line_count;
			 inner_index++)
		{
			compare_result = strcmp(
					BUS_FROM_INDEX(start, inner_index)->name, 
					BUS_FROM_INDEX(start, inner_index + 1)->name);
			// The ASCII value of inner_index is greater,
			// (means that it is further down the English Alphabet)
			// so we need to swap
			if (0 < compare_result)
			{
				swap_lines(BUS_FROM_INDEX(start, inner_index),
						   BUS_FROM_INDEX(start, inner_index + 1));
			}
		}
	}
}

// See documentation at header file
void quick_sort(BusLine* start, BusLine* end, SortType sort_type)
{
	if ((NULL == start) || (NULL == end))
	{
		return;
	}

	// Checking if we reached the end
	if (end < start)
	{
		return;
	}

	BusLine* part = partition(start, end, sort_type);
	if (NULL == part)
	{
		return;
	}

	// Sorting both ends of the partition
	quick_sort(start, part - 1, sort_type);
	quick_sort(part + 1, end, sort_type);
}

// See documentation at header file
BusLine* partition(BusLine* start, BusLine* end, SortType sort_type)
{
	size_t line_count = BUS_LIST_SIZE(start, end);
	size_t index = 0;
	long smaller_index = -1;
	BusLine* pivot_line = end;
	BusLine* current_line = NULL;
	int pivot_line_data = 0;

	if ((NULL == start) || (NULL == end))
	{
		return NULL;
	}

	// If the line count is 0 it means it's a single-item "array"
	// so it's obviously sorted already
	if ((NULL == start) || (NULL == end) || (0 == line_count))
	{
		return start;
	}

	pivot_line_data = get_bus_line_data(pivot_line, sort_type);

	// Dividing the elements to higher and lower than the pivot
	for (index = 0; index < line_count; index++)
	{
		current_line = BUS_FROM_INDEX(start, index);
		if (pivot_line_data >= get_bus_line_data(current_line, sort_type))
		{
			smaller_index++;
			swap_lines(current_line, BUS_FROM_INDEX(start, smaller_index));
		}
	}

	// Adjusting the pivot into its proper position
	smaller_index++;
	swap_lines(pivot_line, BUS_FROM_INDEX(start, smaller_index));

	return BUS_FROM_INDEX(start, smaller_index);
}
