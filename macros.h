/*      $OpenBSD$       */
/*
 * Copyright (c) 2021 Moritz Buhl <mbuhl@openbsd.org>
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

#define __FBSDID(a)
#define rounddown(x,y)	(((x)/(y))*(y))

#define feprintf(i)	do {	\
	if (i == FE_INVALID)	\
		printf("FE_INVALID");	\
	if (i == FE_DENORMAL)	\
		printf("FE_DENORMAL");	\
	if (i == FE_DIVBYZERO)	\
		printf("FE_DIVBYZERO");	\
	if (i == FE_OVERFLOW)	\
		printf("FE_OVERFLOW");	\
	if (i == FE_UNDERFLOW)	\
		printf("FE_UNDERFLOW");	\
	if (i == FE_INEXACT)	\
		printf("FE_INEXACT");	\
} while(0)

#define fesprintf(mask)	do {	\
	int i = mask;	\
	for (int s = FE_INEXACT; s > 0; s >>= 1) {	\
		if (s & i) {	\
			feprintf(s);	\
			i -= s;	\
			if (i == 0)	\
				break;	\
			else	\
				printf(" | ");	\
		}	\
	}	\
	if (i != mask)	\
		printf("\n");	\
} while(0)
