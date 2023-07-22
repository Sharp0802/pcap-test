#include "log.h"
#include <stdarg.h>
#include <stdio.h>

#ifdef __whopr__
__attribute__((externally_visible))
#endif
void prt_err(const char* fmt, ...)
{
	fputs("error: ", stdout);
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

#ifdef __whopr__
__attribute__((externally_visible))
#endif
void prt_info(const char* fmt, ...)
{
	fputs("info: ", stdout);
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

