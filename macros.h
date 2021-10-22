/*      $OpenBSD$       */
/* Public domain - Moritz Buhl */

#define __FBSDID(a)
#define rounddown(x, y)	(((x)/(y))*(y))
#define fpequal(a, b)	fpequal_cs(a, b, 1)
#define cfpequal_cs(a, b, cs)	cfpequal(a, b) // XXX: cs is set for cfpequal
#define hexdump(...)
