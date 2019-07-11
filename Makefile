#	$OpenBSD$

PROGS += cexp_test conj_test csqrt_test exponential_test
PROGS += fenv_test fma_test fmaxmin_test ilogb_test invctrig_test
PROGS += invtrig_test logarithm_test lrint_test lround_test nan_test
PROGS += nearbyint_test next_test rem_test
# ATF
PROGS += trig_test ctrig_test

LDADD += 	-lm

SRCS_rem_test = rem_test.c fls.c

. for t in ${PROGS}
REGRESS_TARGETS+= run-$t
run-$t: $t
	@echo "\n======== $@ ========"
	${.CURDIR}/$t
. endfor

.include <bsd.regress.mk>
