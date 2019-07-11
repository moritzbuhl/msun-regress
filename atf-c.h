/*	$OpenBSD$	*/

#include <string.h>

#define ATF_TC(fn)							\
void atf_head_##fn(void);						\
void atf_body_##fn(void);

#define ATF_TC_HEAD(fn, tc)	void atf_head_##fn(void)
#define ATF_TC_BODY(fn, tc)	void atf_body_##fn(void)
#define ATF_TC_CLEANUP(fn, tc)	void atf_cleanup_##fn(void)

#define atf_tc_set_md_var(tc, attr, fmt, ...)				\
	if (strcmp(attr, "descr") == 0) {				\
		printf(fmt "\n", ##__VA_ARGS__);			\
	}

#define ATF_TP_ADD_TCS(tp)	int main(void)
#define ATF_TP_ADD_TC(_, fn)	do {					\
	atf_head_##fn();						\
	atf_body_##fn();						\
	} while (0)

#define ATF_CHECK(exp)	do {						\
	if (!(exp))							\
		printf("\nFAILED\n"__FILE__":__LINE__: " #exp);		\
	} while (0)

#define atf_tc_expect_fail(fmt, ...)					\
	printf("EXPECTED_FAIL\n"fmt, ##__VA_ARGS__)

#define atf_no_error()	0
