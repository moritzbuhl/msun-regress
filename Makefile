# $OpenBSD: Makefile,v 1.3 2020/12/17 00:51:11 bluhm Exp $

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
