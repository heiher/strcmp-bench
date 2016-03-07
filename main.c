/* main.c
 * Heiher <r@hev.cc>
 */

#include "uart.h"
#include "perf.h"
#include "string.h"

#define SIZE	(128 * 1024)

static char str[SIZE];

static inline void
strset (char *str, char c)
{
	unsigned int i;

	for (i=0; i<SIZE-1; i++)
	      str[i] = c;
	str[SIZE-1] = '\0';
}

static int __attribute__((optimize("O0")))
loop_bench (const char *p1, const char *p2)
{
	unsigned int i, r = 0;

	for (i=0; i<2048; i++) {
		r |= strcmp (p1, p2);
		r |= strcmp (p1, p2);
		r |= strcmp (p1, p2);
		r |= strcmp (p1, p2);
		r |= strcmp (p1, p2);
		r |= strcmp (p1, p2);
		r |= strcmp (p1, p2);
		r |= strcmp (p1, p2);
	}

	return r;
}

static void
run_bench (void)
{
	int i, j, r;
	char *p1, *p2, buf[32];
	unsigned long cycles;

	strset (str, 'a');

	p1 = (char *)(((unsigned long) str + 8) & ~7UL);
	p2 = (char *)(((unsigned long) str + 8 + 8) & ~7UL);

	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			disable_perf ();
			set_perf (0);
			enable_perf ();

			r = loop_bench (p1 + i, p2 + j);

			cycles = get_perf ();
			disable_perf ();

			puts ("s1: ");
			ultostr (buf, (unsigned long) p1 + i);
			puts (buf);
			puts (" s2: ");
			ultostr (buf, (unsigned long) p2 + j);
			puts (buf);
			puts (" r: ");
			ultostr (buf, (unsigned long) r);
			puts (buf);
			puts (" cycles: ");
			ultostr (buf, cycles);
			puts (buf);
			puts ("\r\n");
		}
	}
}

void
master_main (void)
{
	run_bench ();
	puts ("FINISH\r\n");
	while (1);
}

void
slave_main (unsigned int id)
{
}

