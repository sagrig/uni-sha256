#ifndef EROR_HAND_H
#define EROR_HAND_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

void eror_hndl(const char *func, const int line, const int eror)
{
     fprintf(stderr, "%s: %s(%d): %s\n",
	     program_invocation_short_name,
	     func, line, strerror(eror));
}
#endif /* EROR_HAND_H */
