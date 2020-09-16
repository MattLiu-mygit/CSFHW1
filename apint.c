/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 * Matthew Liu
 * mliu78@jh.edu
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "apint.h"
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>

/**
 * Creates a new arbitrary precision integer (ApInt) with an unsigned 64-bit integer.
 * 
 * Parameters:
 * 	val - A uint64_t used to make an ApInt.
 * 
 * Returns:
 * 	out - A pointer to the newly created ApInt.
 */
ApInt *apint_create_from_u64(uint64_t val)
{
	ApInt *out = malloc(sizeof(val) + 12);
	out->apintVal[0] = val;
	out->apintLength = 1;
	return out;
}

/**
 * Creates a new arbitrary precision integer (ApInt) with a hex value.
 * 
 * Parameters:
 * 	hex - A constant hex string used to make an ApInt.
 * 
 * Returns:
 * 	out - A pointer to the newly created ApInt.
 */
ApInt *apint_create_from_hex(const char *hex)
{
	// Number of fragments equals to the max string length over 16 (4 bits per 16-system character, so 16 characters in a 64-bit integer)
	// +1 or +2 based on rounding and taking into account that length starts with 1 rather than 0. Each fragment is a 64-bit unsigned integer
	// in the ApInt.
	int lenMax = strlen(hex);
	uint64_t fragNum = lenMax / 16;
	fragNum = fragNum % 1 != 0 ? fragNum + 2 : fragNum + 1;

	ApInt *out = calloc(1, sizeof(uint64_t) * (fragNum + 1) * 100 + 16);
	out->apintLength = fragNum;

	// Loops through each fragment, inserting numbers into the ApInt array.
	int size = 0;
	for (int i = 0; i < (int)fragNum; i++)
	{
		// This is to help determine which fragment we're looking at.
		int mod = 16 * i;

		// Makes a substring for the hex fragment to be converted to uint64_t
		char *hexFrag = malloc((lenMax - mod > 16 ? 10 * 16 + 1 : 10 * lenMax - mod + 1));
		int strLoc = lenMax > 16 + mod ? lenMax - mod - 16 : 0;
		size = (lenMax > 16 + mod ? 16 : lenMax - mod);
		strncpy(hexFrag, hex + strLoc, size);
		hexFrag[(lenMax - mod > 16 ? 16 : lenMax - mod)] = '\0';

		// Determines where to start converting hex to integer.
		int start = (lenMax > 16 + mod ? 15 : (lenMax - mod - 1));
		out->apintVal[i] = 0;

		// Loops through the frament converting hex (16 system) to integer (10 system).
		for (int n = start; n >= 0; n--)
		{
			int power = start - n;
			uint64_t val = 0.0;
			if (hexFrag[n] >= '0' && hexFrag[n] <= '9')
			{
				val = hexFrag[n] - 48.0;
			}
			else if (hexFrag[n] >= 'a' && hexFrag[n] <= 'f')
			{
				val = hexFrag[n] - 97 + 10.0;
			}
			else if (hexFrag[n] >= 'A' && hexFrag[n] <= 'F')
			{
				val = hexFrag[n] - 65 + 10.0;
			}
			out->apintVal[i] += val * (uint64_t)pow(16, power);
		}
		free(hexFrag);
	}
	return out;
}

/**
 * Frees all alocated memory from an ApInt.
 * 
 * Parameters:
 * 	ap - An ApInt to be freed.
 */
void apint_destroy(ApInt *ap)
{
	free(ap);
}

/**
 * Gets the bit value at the nth index of the ApInt array.
 * 
 * Parameters:
 * 	ap - An ApInt for which we want to get the nth index value of.
 * 	n - the index position in the ApInt array where we want to get the bit value of.
 * 
 * Returns:
 * 	ap->apintVal[n] - The uint64_t at the nth position in the ApInt array.
 */
uint64_t apint_get_bits(ApInt *ap, unsigned n)
{
	if ((int)n >= ap->apintLength)
	{
		return 0;
	}
	return ap->apintVal[n];
}

int apint_highest_bit_set(ApInt *ap)
{
	uint64_t toFind = ap->apintVal[ap->apintLength - 1];
	int count = -1;
	while (toFind != 0)
	{
		count++;
		toFind = toFind >> 1;
	}
	return count + 64 * (ap->apintLength - 1);
}

/**
 * Shifts the ApInt left by 1.
 * 
 * Parameters:
 * 	ap - An ApInt for which we want to shift left once.
 * 
 * Returns:
 * 	output - the shifted ApInt.
 */
ApInt *apint_lshift(ApInt *ap)
{
	ApInt *output = malloc(sizeof(ap->apintLength) * 100000 + sizeof(uint64_t) * (ap->apintLength + 1) * 100000);
	output->apintLength = ap->apintLength;

	for (int i = 0; i < ap->apintLength; i++)
	{
		output->apintVal[i] = ap->apintVal[i];
	}

	// Loops over the ApInt array, and keeps track of if a 1 needs to be brought over.
	int bringOver = 0;
	for (int i = 0; i < output->apintLength; i++)
	{
		uint64_t currentVal = output->apintVal[i];
		int brought = bringOver;

		// Bringover occurs when the first digit is a 1, or when 1 when shifted 63 times to the right.
		if (currentVal >> 63 >= 1)
		{
			bringOver = 1;
		}

		output->apintVal[i] = (currentVal << 1) + brought;
	}

	// If there is still bringover after looping, it's brought over to a new value in the array.
	if (bringOver)
	{
		output->apintVal[output->apintLength] = 1UL;
		output->apintLength = output->apintLength + 1;
	}
	return output;
}

/**
 * Shifts the ApInt left n times.
 * 
 * Parameters:
 * 	ap - An ApInt for which we want to shift left.
 * 	n - the amount of times we want to shift the ApInt left.
 * 
 * Returns:
 * 	out - the shifted ApInt.
 */
ApInt *apint_lshift_n(ApInt *ap, unsigned n)
{
	// Local values created to prevent memory loss later.
	int outLength = ap->apintLength;
	uint64_t outArray[outLength];

	for (int i = 0; i < outLength; i++)
	{
		outArray[i] = ap->apintVal[i];
	}

	// Loops through the ApInt array, copied into a local array, and shifts it n times in a loop.
	for (int g = 0; g < (int)n; g++)
	{
		// A temporary ApInt is made to hold shift the values.
		ApInt *temp = malloc(sizeof(outArray) * 100000 + sizeof(outLength) * 100000);
		temp->apintLength = outLength;
		for (int i = 0; i < outLength; i++)
		{
			temp->apintVal[i] = outArray[i];
		}

		// Loops through the entire ApInt, shifting it.
		int carry = 0;
		for (int i = 0; i < outLength; i++)
		{
			uint64_t sum = outArray[i] * 2 + (uint64_t)carry; // *2 to is the same as shifting it left once.
			carry = outArray[i] > UINT64_MAX - outArray[i];		// if doubling the array exceeds a uint64 max value, a 1 is carried over.
			temp->apintVal[i] = sum;
		}

		// If there's still a carry after looping, a new value is created in the array with the carried 1.
		if (carry)
		{
			temp->apintVal[temp->apintLength] = 1UL;
			temp->apintLength = temp->apintLength + 1;
		}

		// Temporary variables are updated based on the shifted array, and the temporary ApInt is freed.
		outLength = temp->apintLength;
		for (int i = 0; i < outLength; i++)
		{
			outArray[i] = temp->apintVal[i];
		}
		free(temp);
	}

	ApInt *out = malloc(sizeof(outLength) * 1000000 + sizeof(outArray) * 100000);
	out->apintLength = outLength;

	for (int i = 0; i < outLength; i++)
	{
		out->apintVal[i] = outArray[i];
	}
	return out;
}

/**
 * Formats an ApInt value as a hex string
 * 
 * Parameters:
 * 	ap - An ApInt for which we want to reformat.
 * 
 * Returns:
 * 	hex - The formatted hex string.
 */
char *apint_format_as_hex(ApInt *ap)
{
	char *hex = malloc(ap->apintLength * 16 * 100000 + 1);
	int len = 0;

	// The ApInt is looped through, where each character digit is converted to a character and put in a string.
	for (int i = 0; i < ap->apintLength; i++)
	{
		uint64_t val = ap->apintVal[i];
		int count = 0;
		do
		{
			uint64_t adder = val % 16;
			if (adder < 10)
			{
				hex[len] = 48 + adder;
			}
			else if (adder >= 10)
			{
				hex[len] = 87 + adder;
			}
			val = val >> 4;
			count++;
			len++;
		} while (val != 0 || (i < ap->apintLength - 1 && count < 16));
	}

	// The string is actually reversed, so we reverse it to make it unreversed.
	for (int i = 0; i < len / 2; i++)
	{
		char temp = hex[i];
		hex[i] = hex[len - i - 1];
		hex[len - i - 1] = temp;
	}

	hex[len] = '\0';
	return hex;
}

/**
 * Adds to ApInts
 * 
 * Parameters:
 * 	a - The first ApInt for which we want to sum.
 *  b - The second ApInt for which we want to sum.
 * 
 * Returns:
 * 	out - The ApInt sum of the two ApInts.
 */
ApInt *apint_add(const ApInt *a, const ApInt *b)
{
	// The size of the output is first set to the max of the two numbers.
	int max = a->apintLength > b->apintLength ? a->apintLength : b->apintLength;
	ApInt *out = malloc(sizeof(a) * 100000);
	out->apintLength = max;

	// The numbers are looped through and added.
	// A number is carried if the a value and b value are greater than the max of uint64.
	int carry = 0;
	for (int i = 0; i < max; i++)
	{
		// There are 3 cases: when one number is larger by a factor of uint64s, which one number is smaller
		// by a factor uint64s, and when they can both add with each other.
		if (i >= a->apintLength)
		{
			// If b is larger by at least a one uint64, we only sum up the b portion
			uint64_t bVal = b->apintVal[i];
			out->apintVal[i] = bVal + carry;
			carry = bVal > (UINT64_MAX - carry);
		}
		else if (i >= b->apintLength)
		{
			// If a is larger by at least a one uint64, we only sum up the a portion
			uint64_t aVal = a->apintVal[i];
			out->apintVal[i] = aVal + carry;
			carry = aVal > (UINT64_MAX - carry);
		}
		else
		{
			// If both are within one uint64 of each other, they can both be added together.
			uint64_t aVal = a->apintVal[i];
			uint64_t bVal = b->apintVal[i];
			uint64_t sum = aVal + bVal + carry;
			carry = aVal > (UINT64_MAX - bVal);
			out->apintVal[i] = sum;
		}
	}

	// If there's still a carry after the summing, a new value is made with the carried 1.
	if (carry)
	{
		out->apintVal[out->apintLength] = 1UL;
		out->apintLength = out->apintLength + 1;
	}

	return out;
}

/**
 * Subtracts two ApInts: the left ApInt by the right.
 * 
 * Parameters:
 * 	a - The first ApInt for which we want to subtract.
 *  b - The second ApInt for which we want to subtract.
 * 
 * Returns:
 * 	out - The ApInt difference of the left by the right.
 */
ApInt *apint_sub(const ApInt *a, const ApInt *b)
{
	ApInt *out = malloc(sizeof(a->apintLength) * 1000 + sizeof(uint64_t) * 1000);
	out->apintLength = a->apintLength;

	// If the right is greater than the left by at least a factor of uint64, the difference must be
	// negative and illegal, so a NULL is returned.
	if (a->apintLength < b->apintLength)
	{
		free(out);
		return NULL;
	}

	// Otherwise, the two numbers are within a uint64 of each other or the left is greater than the right,
	// And are looped and subtracted from each other.
	int carry = 0;
	for (int i = 0; i < a->apintLength; i++)
	{
		// If a reach
		uint64_t aVal = a->apintVal[i] - carry;
		uint64_t bVal = b->apintVal[i];
		if (i == a->apintLength - 1 && aVal < bVal)
		{
			out->apintVal[i] = bVal - carry;
			carry = 1; // a < b, so we return null.
			break;
		}
		else if (i == a->apintLength - 1 && aVal >= bVal)
		{
			out->apintVal[i] = aVal - bVal; // a > b, so we set carry = 0 and return the difference.
			carry = 0;
		}
		else
		{
			out->apintVal[i] = aVal - bVal;
			carry = bVal > aVal; // if bVal is larger than aVal, it carries over to the next number.
		}
	}

	// If there's still a carry at the end, we return NULL.
	if (carry)
	{
		free(out);
		return NULL;
	}

	return out;
}

/**
 * Compares two ApInts: the left ApInt by the right.
 * 
 * Parameters:
 * 	left - The first ApInt for which we want to compare.
 *  right - The second ApInt for which we want to compare.
 * 
 * Returns:
 * 	1 if the left is greater than the right, -1 if vice versa, and 0 if they're the same.
 */
int apint_compare(const ApInt *left, const ApInt *right)
{
	if (left->apintLength > right->apintLength)
	{
		return 1;
	}
	else if (left->apintLength < right->apintLength)
	{
		return -1;
	}

	for (int i = left->apintLength - 1; i >= 0; i--)
	{
		if (left->apintVal[i] > right->apintVal[i])
		{
			return 1;
		}
		if (left->apintVal[i] < right->apintVal[i])
		{
			return -1;
		}
	}
	return 0;
}
