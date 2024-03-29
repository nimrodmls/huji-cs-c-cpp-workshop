#include <string.h>

#include "tests.h"

// Valid shift values used for the different tests
typedef enum ShiftValues
{
	SHIFT_N_3 = -3,
	SHIFT_N_1 = -1,
	SHIFT_2 = 2,
	SHIFT_3,
	SHIFT_29 = 29,

} ShiftValues, * pShiftValues;

// See full documentation in header file
int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "simplestring";
  const char out[] = "vlpsohvwulqj";

  encode(in, SHIFT_3);
  return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_encode_cyclic_lower_case_special_char_positive_k ()
{
	char in[] = "zhis iz zyclic! and zpecial!\n";
	const char out[] = "bjku kb baenke! cpf brgekcn!\n";

	encode(in, SHIFT_2);
	return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
	char in[] = "simple string!\n";
	const char out[] = "rhlokd rsqhmf!\n";

	encode(in, SHIFT_N_1);
	return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_encode_cyclic_lower_case_negative_k ()
{
	char in[] = "cyclicyclic";
	const char out[] = "zvzifzvzifz";

	encode(in, SHIFT_N_3);
	return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_encode_cyclic_upper_case_positive_k ()
{
	char in[] = "CYCLICYCLIC";
	const char out[] = "FBFOLFBFOLF";

	encode(in, SHIFT_29);
	return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_positive_k ()
{
	char in[] = "vlpsohvwulqj";
	const char out[] = "simplestring";

	decode(in, SHIFT_3);
	return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_decode_cyclic_lower_case_special_char_positive_k ()
{
	char in[] = "bjku kb baenke! cpf brgekcn!\n";
	const char out[] = "zhis iz zyclic! and zpecial!\n";

	decode(in, SHIFT_2);
	return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
	char in[] = "rhlokd rsqhmf!\n";
	const char out[] = "simple string!\n";

	decode(in, SHIFT_N_1);
	return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_decode_cyclic_lower_case_negative_k ()
{
	char in[] = "zvzifzvzifz";
	const char out[] = "cyclicyclic";

	decode(in, SHIFT_N_3);
	return 0 != strcmp(in, out);
}

// See full documentation in header file
int test_decode_cyclic_upper_case_positive_k ()
{
	char in[] = "FBFOLFBFOLF";
	const char out[] = "CYCLICYCLIC";

	decode(in, SHIFT_29);
	return 0 != strcmp(in, out);
}
