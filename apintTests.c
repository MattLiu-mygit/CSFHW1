/*
 * CSF Assignment 1
 * Unit tests for arbitrary-precision integer data type
 *
 * These tests are by no means comprehensive.  You will need to
 * add more tests of your own!  In particular, make sure that
 * you have tests for more challenging situations, such as
 *
 * - large values
 * - adding/subtracting/comparing values with different lengths
 * - special cases (carries when adding, borrows when subtracting, etc.)
 * - etc.
 * 
 * Matthew Liu
 * mliu78@jh.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apint.h"
#include "tctest.h"

typedef struct
{
	ApInt *ap0;
	ApInt *ap1;
	ApInt *ap110660361;
	ApInt *max1;
	ApInt *max1Minus1;
	ApInt *apHex6e34;
	ApInt *apHex0;
	ApInt *apHexMaxPlusOne;
	ApInt *apHexDoubleMax;
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testCreateFromU64(TestObjs *objs);
void testCreateFromHex(TestObjs *objs);
void testHighestBitSet(TestObjs *objs);
void testLshiftN(TestObjs *objs);
void testCompare(TestObjs *objs);
void testFormatAsHex(TestObjs *objs);
void testAdd(TestObjs *objs);
void testSub(TestObjs *objs);
void testLshift(TestObjs *objs);

int main(int argc, char **argv)
{
	TEST_INIT();

	if (argc > 1)
	{
		/*
		 * name of specific test case to execute was provided
		 * as a command line argument
		 */
		tctest_testname_to_execute = argv[1];
	}

	TEST(testCreateFromU64);
	TEST(testCreateFromHex);
	TEST(testHighestBitSet);
	TEST(testLshift);
	TEST(testLshiftN);
	TEST(testCompare);
	TEST(testFormatAsHex);
	TEST(testAdd);
	TEST(testSub);

	TEST_FINI();
}

TestObjs *setup(void)
{
	TestObjs *objs = malloc(sizeof(TestObjs));
	objs->ap0 = apint_create_from_u64(0UL);
	objs->ap1 = apint_create_from_u64(1UL);
	objs->ap110660361 = apint_create_from_u64(110660361UL);
	objs->max1 = apint_create_from_u64(0xFFFFFFFFFFFFFFFFUL);
	objs->max1Minus1 = apint_create_from_u64(0xFFFFFFFFFFFFFFFEUL);
	objs->apHex6e34 = apint_create_from_hex("06e34");
	objs->apHex0 = apint_create_from_hex("0");
	objs->apHexMaxPlusOne = apint_create_from_hex("10000000000000000");
	objs->apHexDoubleMax = apint_create_from_hex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

	return objs;
}

void cleanup(TestObjs *objs)
{
	apint_destroy(objs->ap0);
	apint_destroy(objs->ap1);
	apint_destroy(objs->ap110660361);
	apint_destroy(objs->max1);
	apint_destroy(objs->max1Minus1);
	apint_destroy(objs->apHex6e34);
	apint_destroy(objs->apHex0);
	apint_destroy(objs->apHexMaxPlusOne);
	apint_destroy(objs->apHexDoubleMax);

	free(objs);
}

void testCreateFromU64(TestObjs *objs)
{
	ASSERT(0UL == apint_get_bits(objs->ap0, 0));
	ASSERT(1UL == apint_get_bits(objs->ap1, 0));
	ASSERT(110660361UL == apint_get_bits(objs->ap110660361, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
	ASSERT(0xFFFFFFFFFFFFFFFEUL == apint_get_bits(objs->max1Minus1, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->apHexDoubleMax, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->apHexDoubleMax, 1));
}

void testCreateFromHex(TestObjs *objs)
{
	ASSERT(0x6e34UL == apint_get_bits(objs->apHex6e34, 0));
	ASSERT(0UL == apint_get_bits(objs->apHex0, 0));
	ASSERT(0UL == apint_get_bits(objs->apHexMaxPlusOne, 0));
	ASSERT(1UL == apint_get_bits(objs->apHexMaxPlusOne, 1));
}

void testHighestBitSet(TestObjs *objs)
{
	ASSERT(-1 == apint_highest_bit_set(objs->ap0));
	ASSERT(0 == apint_highest_bit_set(objs->ap1));
	ASSERT(26 == apint_highest_bit_set(objs->ap110660361));
	ASSERT(63 == apint_highest_bit_set(objs->max1));
	ASSERT(-1 == apint_highest_bit_set(objs->apHex0));
	ASSERT(64 == apint_highest_bit_set(objs->apHexMaxPlusOne));
}

void testLshift(TestObjs *objs)
{
	ApInt *result;

	result = apint_lshift(objs->ap0);
	ASSERT(0UL == apint_get_bits(result, 0));
	apint_destroy(result);

	result = apint_lshift(objs->ap1);
	ASSERT(2UL == apint_get_bits(result, 0));
	apint_destroy(result);

	result = apint_lshift(objs->ap110660361);
	ASSERT(0xD311612UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->max1);
	ASSERT(0xFFFFFFFFFFFFFFFEUL == apint_get_bits(result, 0));
	ASSERT(1UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->apHex6e34);
	ASSERT(0x6e34UL * 2 == apint_get_bits(result, 0));
	apint_destroy(result);
}

void testLshiftN(TestObjs *objs)
{
	ApInt *result;
	result = apint_lshift_n(objs->ap0, 17);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->ap1, 17);
	ASSERT(0x20000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->ap110660361, 17);
	ASSERT(0xD3116120000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->max1, 1);
	ASSERT(0xFFFFFFFFFFFFFFFEUL == apint_get_bits(result, 0));
	ASSERT(1UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->max1, 2);
	ASSERT(0xFFFFFFFFFFFFFFFcUL == apint_get_bits(result, 0));
	ASSERT(3UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->max1, 64);
	ASSERT(0x0UL == apint_get_bits(result, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->apHex6e34, 1);
	ASSERT(0x6e34UL * 2 == apint_get_bits(result, 0));
	apint_destroy(result);

	result = apint_lshift_n(objs->apHex6e34, 2);
	ASSERT(0x6e34UL * 4 == apint_get_bits(result, 0));
	apint_destroy(result);

	result = apint_lshift_n(objs->max1, 65);
	ASSERT(0x0UL == apint_get_bits(result, 0));
	ASSERT(0xFFFFFFFFFFFFFFFEUL == apint_get_bits(result, 1));
	ASSERT(1UL == apint_get_bits(result, 2));
	apint_destroy(result);
}

void testCompare(TestObjs *objs)
{
	/* 1 > 0 */
	ASSERT(apint_compare(objs->ap1, objs->ap0) > 0);
	/* 0 < 1 */
	ASSERT(apint_compare(objs->ap0, objs->ap1) < 0);
	/* 110660361 > 0 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap0) > 0);
	/* 110660361 > 1 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap1) > 0);
	/* 0 < 110660361 */
	ASSERT(apint_compare(objs->ap0, objs->ap110660361) < 0);
	/* 1 < 110660361 */
	ASSERT(apint_compare(objs->ap1, objs->ap110660361) < 0);

	ASSERT(apint_compare(objs->ap0, objs->ap0) == 0);

	ApInt *left = apint_create_from_hex("54a47c364bb5e238d8436eff99b62a9fbccf7954702407084be07ccdc28cd4e3069bcaf0e7c81ee4b22dd73501f5a");
	ApInt *right = apint_create_from_hex("54126e51ae104d2cf51e3b05f5fc3e8e23acd08a22764a515dfc33bbc7a434c12d021189efe2f2b");
	ASSERT(apint_compare(left, right) > 0);
	apint_destroy(left);
	apint_destroy(right);

	ASSERT(apint_compare(objs->max1, objs->max1) == 0);

	ASSERT(apint_compare(objs->apHex6e34, objs->apHex0) > 0);

	ASSERT(apint_compare(objs->apHex6e34, objs->max1) < 0);
}

void testFormatAsHex(TestObjs *objs)
{
	char *s;

	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(objs->ap0))));
	free(s);

	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(objs->ap1))));
	free(s);

	ASSERT(0 == strcmp("6988b09", (s = apint_format_as_hex(objs->ap110660361))));
	free(s);

	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->max1))));
	free(s);

	ASSERT(0 == strcmp("6e34", (s = apint_format_as_hex(objs->apHex6e34))));
	free(s);

	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(objs->apHex0))));
	free(s);
}

void testAdd(TestObjs *objs)
{
	ApInt *sum;
	char *s;

	/* 0 + 0 = 0 */
	sum = apint_add(objs->ap0, objs->ap0);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 0 = 1 */
	sum = apint_add(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 0 + 1 = 1 */
	sum = apint_add(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 1 = 2 */
	sum = apint_add(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 110660361 + 1 = 110660362 */
	sum = apint_add(objs->ap110660361, objs->ap1);
	ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* FFFFFFFFFFFFFFFF + 0 = FFFFFFFFFFFFFFFF */
	sum = apint_add(objs->max1, objs->ap0);
	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
	sum = apint_add(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 10000000000000000 + 10000000000000000 = 20000000000000000 */
	sum = apint_add(objs->max1, objs->ap1);
	ApInt *secondSum = apint_add(sum, sum);
	ASSERT(0 == strcmp("20000000000000000", (s = apint_format_as_hex(secondSum))));
	apint_destroy(sum);
	apint_destroy(secondSum);
	free(s);

	sum = apint_add(objs->apHex6e34, objs->max1);
	ASSERT(0 == strcmp("10000000000006e33", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* FFFFFFFFFFFFFFFF + 0 = FFFFFFFFFFFFFFFF */
	sum = apint_add(objs->ap1, objs->apHexDoubleMax);
	ASSERT(0 == strcmp("100000000000000000000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	ApInt *a = apint_create_from_hex("d4fa6f0b63ad80a34b93b74d");
	ApInt *b = apint_create_from_hex("3935dcebf95bdf");
	sum = apint_add(a, b);
	ASSERT(0 == strcmp("d4fa6f0b63e6b680378d132c", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	apint_destroy(b);
	apint_destroy(a);
	free(s);
}

void testSub(TestObjs *objs)
{
	//	printf("\ntest format started\n");
	ApInt *a, *b, *diff;
	char *s;

	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting 0 from 1 is 1 */
	diff = apint_sub(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap1));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 1 is 0 */
	diff = apint_sub(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap0));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 0 can't be represented because it is negative */
	ASSERT(NULL == apint_sub(objs->ap0, objs->ap1));

	/* test involving larger values */
	a = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	b = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	diff = apint_sub(a, b);
	ASSERT(0 == strcmp("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
										 (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	apint_destroy(b);
	apint_destroy(a);
	free(s);
}
