/*	$OpenBSD$	*/
/*-
 * Copyright (c) 2007 David Schultz <das@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "macros.h"

/*
 * Tests for csqrt{,f,l}()
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>

#include <complex.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

#include "atf-c.h"

#include "test-utils.h"

#pragma	STDC CX_LIMITED_RANGE	OFF

#define test(func, z, result, checksign)			do {	\
	volatile long double complex _d = z;				\
	debug("  testing %s(%Lg + %Lg I) ~= %Lg + %Lg I\n", #func,	\
	    creall(_d), cimagl(_d), creall(result), cimagl(result));	\
	ATF_CHECK(cfpequal_cs((func)(_d), (result), (checksign)));	\
} while (0)

#define testall(func, z, result, checksign)			do {	\
	test(func, z, result, checksign);				\
	test(func##f, z, result, checksign);				\
	test(func##l, z, result, checksign);				\
} while (0)

void test_overflow(int);
void test_precision(int, int);

ATF_TC(finite);
ATF_TC_HEAD(finite, tc)
{
	atf_tc_set_md_var(tc, "descr", "Test csqrt for some finite arguments "
	    "where the answer is exact. (We do not test if it produces "
	    "correctly rounded answers when the result is inexact, nor do we "
	    "check whether it throws spurious exceptions.)");
}
ATF_TC_BODY(finite, tc)
{
	static const double tests[] = {
	    /* csqrt(a + bI) = x + yI */
	    /* a	b	x	y */
		0,	8,	2,	2,
		0,	-8,	2,	-2,
		4,	0,	2,	0,
		-4,	0,	0,	2,
		3,	4,	2,	1,
		3,	-4,	2,	-1,
		-3,	4,	1,	2,
		-3,	-4,	1,	-2,
		5,	12,	3,	2,
		7,	24,	4,	3,
		9,	40,	5,	4,
		11,	60,	6,	5,
		13,	84,	7,	6,
		33,	56,	7,	4,
		39,	80,	8,	5,
		65,	72,	9,	4,
		987,	9916,	74,	67,
		5289,	6640,	83,	40,
		460766389075.0, 16762287900.0, 678910, 12345
	};
	/*
	 * We also test some multiples of the above arguments. This
	 * array defines which multiples we use. Note that these have
	 * to be small enough to not cause overflow for float precision
	 * with all of the constants in the above table.
	 */
	static const double mults[] = {
		1,
		2,
		3,
		13,
		16,
		0x1.p30,
		0x1.p-30,
	};

	double a, b;
	double x, y;
	unsigned i, j;

	for (i = 0; i < nitems(tests); i += 4) {
		for (j = 0; j < nitems(mults); j++) {
			a = tests[i] * mults[j] * mults[j];
			b = tests[i + 1] * mults[j] * mults[j];
			x = tests[i + 2] * mults[j];
			y = tests[i + 3] * mults[j];
			testall(csqrt, CMPLXL(a, b), CMPLXL(x, y), 1);
		}
	}

}

ATF_TC(zeros);
ATF_TC_HEAD(zeros, tc)
{
	atf_tc_set_md_var(tc, "descr", "Test the handling of +/- 0.")
}
ATF_TC_BODY(zeros, tc)
{

	testall(csqrt, CMPLXL(0.0, 0.0), CMPLXL(0.0, 0.0), 1);
	testall(csqrt, CMPLXL(-0.0, 0.0), CMPLXL(0.0, 0.0), 1);
	testall(csqrt, CMPLXL(0.0, -0.0), CMPLXL(0.0, -0.0), 1);
	testall(csqrt, CMPLXL(-0.0, -0.0), CMPLXL(0.0, -0.0), 1);
}

ATF_TC(infinities);
ATF_TC_HEAD(infinities, tc)
{
	atf_tc_set_md_var(tc, "descr", "Test the handling of infinities "
	    "when the other argument is not NaN.");
}
ATF_TC_BODY(infinities, tc)
{
	static const double vals[] = {
		0.0,
		-0.0,
		42.0,
		-42.0,
		INFINITY,
		-INFINITY,
	};

	unsigned i;

	for (i = 0; i < nitems(vals); i++) {
		if (isfinite(vals[i])) {
			testall(csqrt, CMPLXL(-INFINITY, vals[i]),
			    CMPLXL(0.0, copysignl(INFINITY, vals[i])), 1);
			testall(csqrt, CMPLXL(INFINITY, vals[i]),
			    CMPLXL(INFINITY, copysignl(0.0, vals[i])), 1);
		}
		testall(csqrt, CMPLXL(vals[i], INFINITY),
		    CMPLXL(INFINITY, INFINITY), 1);
		testall(csqrt, CMPLXL(vals[i], -INFINITY),
		    CMPLXL(INFINITY, -INFINITY), 1);
	}
}

ATF_TC(nans);
ATF_TC_HEAD(nans, tc)
{
	atf_tc_set_md_var(tc, "descr", "Test the handling of NaNs.");
}
ATF_TC_BODY(nans, tc)
{
	testall(csqrt, CMPLXL(INFINITY, NAN), CMPLXL(INFINITY, NAN), 1);

	testall(csqrt, CMPLXL(-INFINITY, NAN), CMPLXL(NAN, INFINITY), 1);

	testall(csqrt, CMPLXL(NAN, INFINITY), CMPLXL(INFINITY, INFINITY), 1);
	testall(csqrt, CMPLXL(NAN, -INFINITY), CMPLXL(INFINITY, -INFINITY), 1);

	testall(csqrt, CMPLXL(0.0, NAN), CMPLXL(NAN, NAN), 1);
	testall(csqrt, CMPLXL(-0.0, NAN), CMPLXL(NAN, NAN), 1);
	testall(csqrt, CMPLXL(42.0, NAN), CMPLXL(NAN, NAN), 1);
	testall(csqrt, CMPLXL(-42.0, NAN), CMPLXL(NAN, NAN), 1);
	testall(csqrt, CMPLXL(NAN, 0.0), CMPLXL(NAN, NAN), 1);
	testall(csqrt, CMPLXL(NAN, -0.0), CMPLXL(NAN, NAN), 1);
	testall(csqrt, CMPLXL(NAN, 42.0), CMPLXL(NAN, NAN), 1);
	testall(csqrt, CMPLXL(NAN, -42.0), CMPLXL(NAN, NAN), 1);
	testall(csqrt, CMPLXL(NAN, NAN), CMPLXL(NAN, NAN), 1);
}

ATF_TC(overflow);
ATF_TC_HEAD(overflow, tc)
{
	atf_tc_set_md_var(tc, "descr", "Test whether csqrt(a + bi) works "
	    "for inputs that are large enough to cause overflow in "
	    "hypot(a, b) + a.  Each of the tests is scaled up to "
	    "near MAX_EXP.");
}
ATF_TC_BODY(overflow, tc)
{
	test_overflow(DBL_MAX_EXP);
	test_overflow(FLT_MAX_EXP);
	test_overflow(LDBL_MAX_EXP);
}

void
test_overflow(int maxexp)
{
	long double a, b, x, y;
	long double complex result;
	int exp, i;

	ATF_CHECK(maxexp > 0 && maxexp % 2 == 0);

	for (i = 0; i < 4; i++) {
		exp = maxexp - 2 * i;

		/* csqrt(115 + 252*I) == 14 + 9*I */
		a = ldexpl(115 * 0x1p-8, exp);
		b = ldexpl(252 * 0x1p-8, exp);
		x = ldexpl(14 * 0x1p-4, exp / 2);
		y = ldexpl(9 * 0x1p-4, exp / 2);
		testall(csqrt, CMPLXL(a, b), CMPLXL(x, y), 1);

		/* csqrt(-11 + 60*I) = 5 + 6*I */
		a = ldexpl(-11 * 0x1p-6, exp);
		b = ldexpl(60 * 0x1p-6, exp);
		x = ldexpl(5 * 0x1p-3, exp / 2);
		y = ldexpl(6 * 0x1p-3, exp / 2);
		testall(csqrt, CMPLXL(a, b), CMPLXL(x, y), 1);

		/* csqrt(225 + 0*I) == 15 + 0*I */
		a = ldexpl(225 * 0x1p-8, exp);
		b = 0;
		x = ldexpl(15 * 0x1p-4, exp / 2);
		y = 0;
		testall(csqrt, CMPLXL(a, b), CMPLXL(x, y), 1);
	}
}

ATF_TC(precision);
ATF_TC_HEAD(precision, tc)
{
	atf_tc_set_md_var(tc, "descr", "Test that precision is maintained "
	    "for some large squares.  Set all or some bits in the lower "
	    "mantdig/2 bits, square the number, and try to recover the "
	    "sqrt. Note: (x + xI)**2 = 2xxI");
}
ATF_TC_BODY(precision, tc)
{
	test_precision(DBL_MAX_EXP, DBL_MANT_DIG);
	test_precision(FLT_MAX_EXP, FLT_MANT_DIG);
	test_precision(LDBL_MAX_EXP,
#ifndef __i386__
	    LDBL_MANT_DIG
#else
	    DBL_MANT_DIG
#endif
	    );
}

void
test_precision(int maxexp, int mantdig)
{
	long double b, x;
	long double complex result;
	uint64_t mantbits, sq_mantbits;
	int exp, i;

	ATF_CHECK(maxexp > 0 && maxexp % 2 == 0);
	ATF_CHECK(mantdig <= 64);
	mantdig = rounddown(mantdig, 2);

	for (exp = 0; exp <= maxexp; exp += 2) {
		mantbits = ((uint64_t)1 << (mantdig / 2 )) - 1;
		for (i = 0;
		    i < 100 && mantbits > ((uint64_t)1 << (mantdig / 2 - 1));
		    i++, mantbits--) {
			sq_mantbits = mantbits * mantbits;
			/*
			 * sq_mantibts is a mantdig-bit number.  Divide by
			 * 2**mantdig to normalize it to [0.5, 1), where,
			 * note, the binary power will be -1.  Raise it by
			 * 2**exp for the test.  exp is even.  Lower it by
			 * one to reach a final binary power which is also
			 * even.  The result should be exactly
			 * representable, given that mantdig is less than or
			 * equal to the available precision.
			 */
			b = ldexpl((long double)sq_mantbits,
			    exp - 1 - mantdig);
			x = ldexpl(mantbits, (exp - 2 - mantdig) / 2);
			ATF_CHECK(b == x * x * 2);
			testall(csqrt, CMPLXL(0, b),  CMPLXL(x, x), 1);
		}
	}
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, finite);
	ATF_TP_ADD_TC(tp, zeros);
	ATF_TP_ADD_TC(tp, infinities);
	ATF_TP_ADD_TC(tp, nans);
	ATF_TP_ADD_TC(tp, overflow);
	ATF_TP_ADD_TC(tp, precision);
}
