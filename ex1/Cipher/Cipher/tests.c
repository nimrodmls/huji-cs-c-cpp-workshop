#include <string.h>

#include "tests.h"

typedef enum _ShiftValues
{
	SHIFT_2 = 2,
	SHIFT_3,

} ShiftValues, * pShiftValues;

/**
 * Checks if the given strings are cyclic or not
 * @param in - The original string
 * @param out - The manipulated (shifted) string
 * @param size - The size of the string
 */
int is_non_cyclic(const char * in, const char * out, size_t size)
{
	size_t iter = 0;
	int non_cyclic = 1;

	for (iter = 0; iter < size; iter++)
	{
		if (0 > out[iter] - in[iter])
		{
			non_cyclic = 0;
			goto cleanup;
		}
	}
cleanup:
	return non_cyclic;

}

// See full documentation in header file
int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "simple string";
  const char out[] = "vlpsoh vwulqj";

  if (!is_non_cyclic(in, out, strlen(in)))
  {
	  return 1;
  }

  encode(in, SHIFT_3);
  return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_encode_cyclic_lower_case_special_char_positive_k ()
{
	char in[] = "";
}

// See full documentation in header file
int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
  // your code goes here
}

// See full documentation in header file
int test_encode_cyclic_lower_case_negative_k ()
{
  // your code goes here
}

// See full documentation in header file
int test_encode_cyclic_upper_case_positive_k ()
{
  // your code goes here
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "def";
  char out[] = "abc";
  decode (in, SHIFT_3);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_special_char_positive_k ()
{
  // your code goes here
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
  // your code goes here
}

// See full documentation in header file
int test_decode_cyclic_lower_case_negative_k ()
{
  // your code goes here
}

// See full documentation in header file
int test_decode_cyclic_upper_case_positive_k ()
{
  // your code goes here
}
