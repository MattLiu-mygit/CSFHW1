/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "apint.h"
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>
#include <math.h>

ApInt *apint_create_from_u64(uint64_t val)
{
	/* TODO: implement */
	ApInt *out = malloc(sizeof(uint64_t) + 16);
	out->apint_val[0] = val;
	out->apint_length = 1;
	//printf("\ndoing %lu\n", val);
	//assert(0);
	return out;
}

ApInt *apint_create_from_hex(const char *hex)
{
	/* TODO: implement */
	ApInt *out = malloc(sizeof(hex) + 16);
	out->apint_val[0] = hex;
	out->apint_length = 1;
	// assert(0);
	return out;
}

void apint_destroy(ApInt *ap)
{
	/* TODO: implement */
	// NOT WORKING
	printf("\nfreeing %lu at location %p\n", ap->apint_val[0], ap);
	// if (&ap->apint_val != NULL)
	// {
	// 	free(ap->apint_val);
	// }
	free(ap);
}

uint64_t apint_get_bits(ApInt *ap, unsigned n)
{
	/* TODO: implement */
	// assert(0);
	// return 0 if higher
	if (n >= ap->apint_length)
	{
		return 0;
	}
	return ap->apint_val[n];
}

int apint_highest_bit_set(ApInt *ap)
{
	uint64_t toFind = *ap->apint_val[ap->apint_length - 1];
	int count = -1;
	while (toFind != 0)
	{
		count++;
		toFind = toFind >> 1;
	}
	return count;
}

ApInt *apint_lshift(ApInt *ap)
{
	/* TODO: implement */
	int bringOver = 0;
	for (int i = 0; i < ap->apint_length; i++)
	{
		uint64_t currentVal = ap->apint_val[i];
		if (currentVal >> 63 >= 1)
		{
			bringOver = 1;
		}
		currentVal = currentVal << 1;
		if (bringOver)
		{
			currentVal++;
		}
		else
		{
			bringOver = 0;
		}
		ap->apint_val[i] = currentVal;
	}
	if (bringOver)
	{
		ap = realloc(&ap, sizeof(uint64_t) * (ap->apint_length + 1) + 16);
		ap->apint_val[ap->apint_length] = 1UL;
		ap->apint_length = ap->apint_length + 1;
	}
	//assert(0);
	return ap;
}

ApInt *apint_lshift_n(ApInt *ap, unsigned n)
{
	/* TODO: implement */
	//printf("\nstarting! %lu\n", ap->apint_val[0]);
	for (unsigned i = 0; i < n; i++)
	{
		ap = apint_lshift(ap);
	}
	//printf("\nsuccess! %lu\n", ap->apint_val[0]);
	// assert(0);
	return ap;
}

char *apint_format_as_hex(ApInt *ap)
{
	char *hex = malloc(ap->apint_length * 16 + 1); //find a better replacement
	int len = 0;
	printf("\nloop start with length %d\n", ap->apint_length);

	for (int i = 0; i < ap->apint_length; i++)
	{
		printf("\nlooped with hex as %s\n", hex);
		uint64_t val = ap->apint_val[i];
		int count = 0;
		do
		{
			uint64_t adder;
			adder = val % 16;
			printf("\nadder is %lu\n", adder);
			if (adder < 10)
			{
				hex[len] = 48 + adder;
			}
			else if (adder >= 10)
			{
				hex[len] = 55 + adder;
			}
			val = val >> 4;
			count++;
			len++;
		} while (val != 0 || (i < ap->apint_length - 1 && count < 16));
	}
	printf("\nloop passed\n");
	for (int i = 0; i < len / 2; i++)
	{
		char temp = hex[i];
		hex[i] = tolower(hex[len - i - 1]);
		hex[len - i - 1] = tolower(temp);
	}

	hex[len] = '\0';
	printf("\nhex is %s\n", hex);
	return hex;
}

ApInt *apint_add(const ApInt *a, const ApInt *b)
{
	int carry = 0;
	int max;
	if (a->apint_length > b->apint_length)
	{
		max = a->apint_length;
	}
	else
	{
		max = b->apint_length;
	}
	ApInt *out = malloc(sizeof(uint64_t) * max + 16);
	out->apint_length = max;
	for (int i = 0; i < max; i++)
	{
		if (i == a->apint_length)
		{
			uint64_t bVal = b->apint_val[i];
			uint64_t sum = bVal + carry;
			if (sum - bVal != carry)
			{
				carry = 1;
			}
			else
			{
				carry = 0;
			}
			out->apint_val[i] = sum;
		}
		else if (i == b->apint_length)
		{
			uint64_t aVal = a->apint_val[i];
			uint64_t sum = aVal + carry;
			if (sum - aVal != carry)
			{
				carry = 1;
			}
			else
			{
				carry = 0;
			}
			out->apint_val[i] = aVal + carry;
		}
		else
		{
			uint64_t aVal = a->apint_val[i];
			uint64_t bVal = b->apint_val[i];
			uint64_t sum = aVal + bVal + carry;
			uint64_t check = sum < aVal + carry;

			// if (sum - aVal - carry != bVal)
			if (check)
			{
				carry = 1;
			}
			else
			{
				carry = 0;
			}
			out->apint_val[i] = sum;
		}
	}

	if (carry)
	{
		out = realloc(out, sizeof(uint64_t) * (out->apint_length + 1) + 16);
		out->apint_val[out->apint_length] = 1UL;
		out->apint_length = out->apint_length + 1;
	}

	/* TODO: implement */
	// assert(0);
	// free a aand b
	return out;
}

ApInt *apint_sub(const ApInt *a, const ApInt *b)
{
	/* TODO: implement */
	assert(0);
	return NULL;
}

int apint_compare(const ApInt *left, const ApInt *right)
{
	/* TODO: implement */
	assert(0);
	return 0;
}
