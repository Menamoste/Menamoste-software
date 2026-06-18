#ifndef ERR_COMPAT_H
#define ERR_COMPAT_H

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Compatibility wrapper for err() on Windows
static inline void err(int eval, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(eval);
}

// Compatibility wrapper for errx() on Windows
static inline void errx(int eval, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(eval);
}

#else
#include <err.h>
#endif

#endif
