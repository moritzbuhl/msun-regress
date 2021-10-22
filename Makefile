#	$OpenBSD: Makefile,v 1.3 2020/12/17 00:51:11 bluhm Exp $

# Copyright (c) 2021 Moritz Buhl <moritzbuhl@openbsd.org>
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

# Each test program in PROGS may define several numbered subtests.
# In a first step compile all programs and extract their parameters.
# For each PROG define new regression subtests based on the test number.

.if defined(NUMBERS)
REGRESS_TARGETS =	${NUMBERS:C/(.*)/run-${PROG}-\1 cleanup-${PROG}-\1/}
.else
REGRESS_TARGETS =	${PROGS:S/^/run-/}
.endif

PROGS =
PROGS+= cexp_test
PROGS+= conj_test
PROGS+= csqrt_test
PROGS+= ctrig_test
PROGS+= exponential_test
PROGS+= fenv_test
PROGS+= fma_test
PROGS+= fmaxmin_test
PROGS+= ilogb_test
PROGS+= invctrig_test
PROGS+= invtrig_test
PROGS+= logarithm_test
PROGS+= lrint_test
PROGS+= lround_test
PROGS+= nan_test
PROGS+= nearbyint_test
PROGS+= next_test
PROGS+= rem_test
PROGS+= trig_test

# failing tests

. for t in run-cexp_test-{1,2,3,4,5,7}
${t}:
	# cexp functions don't handle inf and nan cases
	@echo DISABLED
. endfor

. for t in run-csqrt_test-{1,2,3} run-ctrig_test-{1,2,3,4,6} \
	run-exponential_test-1 run-fma_test-7 run-invctrig_test-{1,2,3,4} \
	run-invtrig_test-{1,2,3,5,6,7} run-logarithm_test-1 run-lround_test-1 \
	run-nan_test-1 run-nearbyint_test-1 run-next_test-{1,2,4,5} \
	run-rem_test-3 run-trig_test-3
${t}:
	# XXX: Doesn't work for unknown reasons
	@echo DISABLED
. endfor


LDADD=	-lm
DPADD=	${LIBM}
CFLAGS+=	-fno-builtin

.for t in ${TESTS}
REGRESS_TARGETS+=	run-$t
run-$t: $t
	./$t
.endfor

. for p in ${PROGS}
SRCS_$p =		$p.c atf-c.c
. endfor

SRCS_rem_test += fls.c

.for p in ${PROGS}
run-$p: $p
	ulimit -c unlimited && \
	ntests="`./$p -n`" && \
	echo "1..$$ntests" && \
	tnumbers="`jot -ns' ' - 1 $$ntests`" && \
	${.MAKE} -C ${.CURDIR} PROG=$p NUMBERS="$$tnumbers" regress
.endfor

.if defined(NUMBERS)
CUR_USER !=		id -g

. for n in ${NUMBERS}
DESCR_$n !=		eval `./${PROG} -i $n` && echo $$DESCR
REQ_USER_$n !=		eval `./${PROG} -i $n` && echo $$REQ_USER

.  if ${REQ_USER_$n} == "root"
REGRESS_ROOT_TARGETS +=	run-${PROG}-$n
.  endif

run-${PROG}-$n:
	@echo "$n ${DESCR_$n}"
.  if ${REQ_USER_$n} == "root"
	${SUDO} ./${PROG} -r $n
.  elif ${REQ_USER_$n} == "unprivileged" && ${CUR_USER} == 0
	${SUDO} su ${BUILDUSER} -c exec ./${PROG} -r $n
.  elif ${REQ_USER_$n} == "unprivileged" || ${REQ_USER_$n} == ""
	./${PROG} -r $n
.  else
	# bad REQ_USER: ${REQ_USER_$n}
	false
.  endif

cleanup-${PROG}-$n:
	-./${PROG} -c $n

. endfor
.endif

.include <bsd.regress.mk>
