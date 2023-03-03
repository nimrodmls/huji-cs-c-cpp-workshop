#include <ctype.h>
#include "cipher.h"

#define LETTER_COUNT 26

char manipulate_character(const char in, int shift)
{
	int new_char = in;
	int new_shift = 0;
	int is_upper = 0;

	// Validating the character is in the alphabetic, otherwise ignore
	if (!isalpha(in))
	{
		return in;
	}

	is_upper = isupper(new_char);
	new_char = tolower(new_char);
	new_shift = (new_char - 'a'  + shift) % LETTER_COUNT;

	// If the shift is negative, complete it to positive integer
	if (new_shift < 0)
	{
		new_shift += LETTER_COUNT;
	}

	// The new character is taken
	new_char = 'a' + new_shift;

	if (is_upper)
	{
		return toupper(new_char);
	}
	return new_char;
}

// See full documentation in header file
void encode (char s[], int k)
{
	unsigned long index = 0;
	for (index = 0; s[index] != 0; index++)
	{
		s[index] = (char)manipulate_character(s[index], k);
	}
}

// See full documentation in header file
void decode (char s[], int k)
{
	encode(s, -k);
}
