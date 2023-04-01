#include <stdio.h>
#include <stdlib.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"

BusLine create_line(char* name, int distance, int duration)
{
	BusLine line = { 0 };
	memcpy(&line.name, name, sizeof(line.name));
	line.distance = distance;
	line.duration = duration;
	return line;

}

/**
 * TODO add documentation
 */
int main (int argc, char *argv[])
{
	char* name1 = "ahelloworld";
	char* name2 = "bhellobworld";
	char* name3 = "bhelloaworld";
	char* name4 = "dhelloworld";
	BusLine* a = (BusLine*)calloc(4, sizeof(BusLine));
	BusLine line1 = create_line(name1, 1, 1);
	BusLine line2 = create_line(name2, 1, 1);
	BusLine line3 = create_line(name3, 1, 1);
	BusLine line4 = create_line(name4, 1, 1);
	memcpy(a, &line1, sizeof(*a));
	memcpy(a+1, &line2, sizeof(*a));
	memcpy(a+2, &line3, sizeof(*a));
	memcpy(a+3, &line4, sizeof(*a));
	bubble_sort(a, a + 3);
	//quick_sort(a, a + 3, DISTANCE);
	printf("stahp");
}
