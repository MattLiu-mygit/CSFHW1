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
#include <stdint.h>

ApInt *apint_create_from_u64(uint64_t val)
{
	ApInt *out = malloc(sizeof(val) + 12);
	out->apint_val[0] = val;
	out->apint_length = 1;
	return out;
}

ApInt *apint_create_from_hex(const char *hex)
{
	uint64_t fragNum;
	int lenMax = strlen(hex);
	fragNum = lenMax / 16;
	fragNum = fragNum % 1 != 0 ? fragNum + 2 : fragNum + 1;
	ApInt *out = malloc(sizeof(uint64_t) * fragNum + sizeof(int) + 4);
	out->apint_length = fragNum;

	int size = 0;
	for (int i = 0; i < (int)fragNum; i++)
	{
		int mod = 16 * i;
		char *hexFrag = (char *)malloc(sizeof(char) * (lenMax - mod >= 16 ? 16 : lenMax - mod));
		printf("%p\n", hexFrag);
		int strLoc = lenMax > 16 + mod ? lenMax - mod - 16 : 0;
		size = (lenMax > 16 + mod ? 16 : lenMax - mod);
		strncpy(hexFrag, hex + strLoc, size);
		out->apint_val[i] = 0;
		int start = (lenMax > 16 + mod ? 15 : (lenMax - mod - 1));
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
			out->apint_val[i] += val * (uint64_t)pow(16, power);
		}
	}
	return out;
}

void apint_destroy(ApInt *ap)
{
	free(ap);
}

uint64_t apint_get_bits(ApInt *ap, unsigned n)
{
	if ((int)n >= ap->apint_length)
	{
		return 0;
	}
	return ap->apint_val[n];
}

int apint_highest_bit_set(ApInt *ap)
{
	uint64_t toFind = ap->apint_val[ap->apint_length - 1];
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
		ap = realloc(&ap, sizeof(uint64_t) * (ap->apint_length + 1) + sizeof(ap->apint_length));
		ap->apint_val[ap->apint_length] = 1UL;
		ap->apint_length = ap->apint_length + 1;
	}
	return ap;
}

ApInt *apint_lshift_n(ApInt *ap, unsigned n)
{
	// make a seperate array and length
	for (int i = 0; i < (int)n; i++)
	{
		ap = apint_add(ap, ap); // This isn't looping apint_lshift
	}
	return ap;
}

char *apint_format_as_hex(ApInt *ap)
{
	char *hex = malloc(ap->apint_length * 16 + 1);
	int len = 0;

	for (int i = 0; i < ap->apint_length; i++)
	{
		uint64_t val = ap->apint_val[i];
		int count = 0;
		do
		{
			uint64_t adder;
			adder = val % 16;
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
		} while (val != 0 || (i < ap->apint_length - 1 && count < 16));
	}
	for (int i = 0; i < len / 2; i++)
	{
		char temp = hex[i];
		hex[i] = hex[len - i - 1];
		hex[len - i - 1] = temp;
	}

	hex[len] = '\0';
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
	ApInt *out = malloc(sizeof(uint64_t) * max + 20);
	out->apint_length = max;
	for (int i = 0; i < max; i++)
	{
		if (i == a->apint_length)
		{
			uint64_t bVal = b->apint_val[i];
			uint64_t sum = bVal + carry;
			carry = bVal > UINT64_MAX - carry;
			out->apint_val[i] = sum;
		}
		else if (i == b->apint_length)
		{
			uint64_t aVal = a->apint_val[i];
			carry = aVal > UINT64_MAX - carry;
			out->apint_val[i] = aVal + carry;
		}
		else
		{
			uint64_t aVal = a->apint_val[i];
			uint64_t bVal = b->apint_val[i];
			uint64_t sum = aVal + bVal + carry;
			carry = aVal > UINT64_MAX - bVal;
			out->apint_val[i] = sum;
		}
	}

	if (carry)
	{
		//out = realloc(out, sizeof(uint64_t) * (out->apint_length + 1) + 16);
		out->apint_val[out->apint_length] = 1UL;
		out->apint_length = out->apint_length + 1;
	}

	return out;
}

ApInt *apint_sub(const ApInt *a, const ApInt *b)
{

	ApInt *out = malloc(sizeof(uint64_t) * a->apint_length + 8);
	out->apint_length = a->apint_length;
	if (a->apint_length < b->apint_length)
	{
		free(out);
		return NULL;
	}

	int carry = 0;
	for (int i = 0; i < a->apint_length; i++)
	{
		if (i == a->apint_length)
		{
			out->apint_val[i] = b->apint_val[i] - carry;
			carry = 1;
			break;
		}
		else if (i == b->apint_length)
		{
			out->apint_val[i] = a->apint_val[i] - carry;
			carry = 0;
		}
		else
		{
			uint64_t aVal = a->apint_val[i] - carry;
			uint64_t bVal = b->apint_val[i];
			uint64_t difference = aVal - bVal;
			out->apint_val[i] = difference;
			carry = bVal > aVal;
		}
	}
	if (carry)
	{
		free(out);
		return NULL;
	}
	return out;
}

int apint_compare(const ApInt *left, const ApInt *right)
{
	if (left->apint_length > right->apint_length)
	{
		return 1;
	}
	else if (left->apint_length < right->apint_length)
	{
		return -1;
	}

	for (int i = left->apint_length - 1; i >= 0; i--)
	{
		if (left->apint_val[i] > right->apint_val[i])
		{
			return 1;
		}
		if (left->apint_val[i] < right->apint_val[i])
		{
			return -1;
		}
	}
	return 0;
}
