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
		realloc(&ap, sizeof(uint64_t) * (ap->apint_length + 1) + 16);
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
	/* TODO: implement */
	//assert(0);
	printf("\nformatting %lu\n", ap->apint_val[0]);
	char hex[] = "";
	//int count = 0;
	for (int i = 0; i < ap->apint_length; i++)
	{
		printf("star\n");
		uint64_t val = ap->apint_val[i];
		int mod = pow(16, i + 1);
		printf("\nmod is %d and val is %lu, %d times\n", mod, val, ap->apint_length);
		val = val % mod;
		val = val >> 16 * i;
		char toAdd;
		if (val < 10)
		{
			toAdd = 48 + val;
			printf("\nadded %c\n", toAdd);
			hex[i] = toAdd;
			printf("\nhex is after %s\n", hex);
		}
		else if (val >= 10)
		{
			toAdd = 55 + val;
			strncat(hex, toAdd, 1);
		}
	}
	//printf("\nhex is before %s\n", hex);
	//hex[ap->apint_length] = '\0';
	//printf("\nhex is after %s\n", hex);
	char *out = hex;
	return out;
}

ApInt *apint_add(const ApInt *a, const ApInt *b)
{
	printf("adding %lu and %lu", a->apint_val[0], b->apint_val[0]);
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
	// ApInt *out = malloc(sizeof(uint64_t) * max + 16);
	// out->apint_length = max;
	// for (int i = 0; i < out->apint_length || carry; i++)
	// {
	// 	if (i == a->apint_length)
	// 	{
	// 		printf("happened");
	// 	}

	// 	if (i < b->apint_length)
	// 	{ //handle carryover
	// 		out->apint_val[i] = *a->apint_val[i] + *b->apint_val[i] + carry;
	// 	}
	// 	else
	// 	{
	// 		out->apint_val[i] = a->apint_val[i] + carry;
	// 	}
	// 	//carry = a->apint_val[i] >= (pow(2, 64) - 1);
	// 	if (carry)
	// 	{
	// 		//out->apint_val[i] -= pow(2, 64) - 1;
	// 	}
	// }

	/* TODO: implement */
	// assert(0);
	return a;
	//return out;
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
