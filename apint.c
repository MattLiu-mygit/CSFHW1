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
	printf("\ndoing %ul\n", val);
	//assert(0);
	return out;
}

ApInt *apint_create_from_hex(const char *hex)
{
	/* TODO: implement */
	char *lowerHex = hex;
	for (int i = 0; i <= strlen(hex); i++)
	{
		if (hex[i] >= 65 && hex[i] <= 90)
			lowerHex[i] = hex[i] + 32;
	}
	printf("lowercase is %s\n", lowerHex);
	// assert(0);
	return NULL;
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
	printf("\nstarting! %lu\n", ap->apint_val[0]);
	for (unsigned i = 0; i < n; i++)
	{
		ap = apint_lshift(ap);
	}
	printf("\nsuccess! %lu\n", ap->apint_val[0]);
	// assert(0);
	return ap;
}

char *apint_format_as_hex(ApInt *ap)
{
	/* TODO: implement */
	assert(0);
	return NULL;
}

ApInt *apint_add(const ApInt *a, const ApInt *b)
{
	/* TODO: implement */
	assert(0);
	return NULL;
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
