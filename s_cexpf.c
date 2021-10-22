/*	$OpenBSD: s_cexpf.c,v 1.2 2010/07/18 18:42:26 guenther Exp $	*/
/*
 * Copyright (c) 2008 Stephen L. Moshier <steve@moshier.net>
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

/*							cexpf()
 *
 *	Complex exponential function
 *
 *
 *
 * SYNOPSIS:
 *
 * void cexpf();
 * cmplxf z, w;
 *
 * cexpf( &z, &w );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the exponential of the complex argument z
 * into the complex result w.
 *
 * If
 *     z = x + iy,
 *     r = exp(x),
 *
 * then
 *
 *     w = r cos y + i r sin y.
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      -10,+10     30000       1.4e-7      4.5e-8
 *
 */

#include <complex.h>
#include <math.h>

// XXX:
#include <stdio.h>
#include <fenv.h>

float complex
cexpf(float complex z)
{
	float complex w;
	float r, x, y;

	x = crealf (z);
	y = cimagf (z);
	if (isnan(x)) {
		if (fpclassify(y) & FP_ZERO) {
			return z;
		} else {
			return NAN + NAN * I;
		}
	}

	if (isfinite(x) && isnan(y))
		return NAN + NAN * I;

	if (signbit(x) && isinf(x) && isnan(y))
		return 0 + 0 * I;

	if (isinf(x) && isnan(y))
		return z;

	printf("%s:%d except: %d\n",  __FILE__, __LINE__, fetestexcept(FE_ALL_EXCEPT));
	r = expf(x);
	printf("%s:%d except: %d\n",  __FILE__, __LINE__, fetestexcept(FE_ALL_EXCEPT));
	w = r * cosf(y) +  r * sinf(y) * I;
	printf("%s:%d except: %d\n",  __FILE__, __LINE__, fetestexcept(FE_ALL_EXCEPT));
	return (w);
}
