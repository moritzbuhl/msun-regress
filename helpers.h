/*	$OpenBSD$	*/
/*
 * Copyright (c) 2019 Moritz Buhl <mbuhl@moritzbuhl.de>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef	_HELPERS_H_
#define	_HELPERS_H_

#include <fenv.h>
#include <stdio.h>

#include "test-utils.h"

int verbose = 0;

void check_feexcept(int, int);
void check_fpequal(long double, long double);
void printFE(int);

void
check_fpequal(long double a, long double b)
{
	if (!fpequal(a, b))
		printf("%.32Lf != %.32Lf.\nFAIL\n",a,b);
}

void
check_feexcept(int mask, int excepts)
{
	int res;

	res = fetestexcept(mask);

	if (res != excepts) {
		printf("fetestexcept expected \"");
		printFE(excepts);
		printf("\" has \"");
		printFE(res);
		printf("\".\nFAIL\n");
	}
}

void
printFE(int excepts)
{
	int i;
	char *FEstr[] = { "FE_DIVBYZERO", "FE_INEXACT", "FE_INVALID",
		"FE_OVERFLOW", "FE_UNDERFLOW", "FE_DENORMAL" };
	int FEs[] = { FE_DIVBYZERO, FE_INEXACT, FE_INVALID,
		FE_OVERFLOW, FE_UNDERFLOW, FE_DENORMAL };

	for (i = 0; i < sizeof(FEs)/sizeof(*FEs); i++) {
		if (excepts & FEs[i]) {
			printf("%s", FEstr[i]);
			excepts ^= FEs[i];
			if (excepts)
				printf("|");
		}
	}
}
#endif /* _HELPERS_H_ */
