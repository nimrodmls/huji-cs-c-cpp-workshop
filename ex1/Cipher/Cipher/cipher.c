#include <ctype.h>
#include "cipher.h"

// Letter count in the english alphabet
#define LETTER_COUNT (26)

/**
 * Shifts the given character within the English Alphabet
 * @param in - The character to shift
 * @param shift - The amount of characters to shift
 */
char manipulate_character(const char in, const int shift)
{
	int new_char = in;
	int new_shift = shift % LETTER_COUNT;
	int is_upper = 0;

	// Validating the character is in the alphabetic, otherwise ignore
	if (!isalpha(in))
	{
		return in;
	}

	// If the character is upper-case when make it lower-case,
	// and shifts it within the lower-case portion of the ASCII
	is_upper = isupper(new_char);
	new_char = tolower(new_char) + (shift % LETTER_COUNT);
	
	// Checking if the shift caused a cycle-situation
	if (new_char < 'a')
	{
		new_char += LETTER_COUNT;
	}
	else if (new_char > 'z')
	{
		new_char -= LETTER_COUNT;
	}

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
	for (index = 0; 0 != s[index]; index++)
	{
		s[index] = manipulate_character(s[index], k);
	}
}

// See full documentation in header file
void decode (char s[], int k)
{
	// Encode does the same as decode,
	// but with "decoding" encryption key (the inverse)
	encode(s, -k);
}
