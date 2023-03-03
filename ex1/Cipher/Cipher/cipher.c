#include <stdio.h>
#include <ctype.h>
#include "cipher.h"

unsigned char manipulate_character(const unsigned char in, unsigned int shift)
{
	// Validating the character is in the alphabetic, otherwise ignore
	if (!isalpha(in))
	{
		return in;
	}

	unsigned int new_char = in;
	const int is_upper = isupper(new_char);

	new_char = tolower(new_char);
	new_char += shift;
	new_char %= 'z';

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
		
	}
}

// See full documentation in header file
void decode (char s[], int k)
{
  // your code goes here
}
