#pragma once

#include <library/_cheader.h>
#include <library/stddef.h>

_Begin_C_Header

    // extern void
    // exit(int status);
extern char *getenv(const char *name);

// extern void *malloc(size_t size);
// extern void free(void *ptr);
// extern void *calloc(size_t nmemb, size_t size);
// extern void *realloc(void *ptr, size_t size);

extern void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

//extern int exec(char *, int, char **, char **, int);
//extern int system(char *, int, char **, char **);

// extern int abs(int j);

extern int putenv(char *name);
extern int setenv(const char *name, const char *value, int overwrite);
extern int unsetenv(const char *str);

extern double strtod(const char *nptr, char **endptr);

#define ATEXIT_MAX 32
extern int atexit(void (*h)(void));
extern void _handle_atexit(void);

#define RAND_MAX 0x7FFFFFFF

extern void abort(void);

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

extern void *bsearch(const void *key, const void *base, size_t nmemb, size_t size,
                     int (*compar)(const void *, const void *));

extern char *mktemp(char *);
extern int mkstemp(char *);

extern size_t mbstowcs(wchar_t *dest, const char *src, size_t n);
extern size_t wcstombs(char *dest, const wchar_t *src, size_t n);

typedef struct
{
    int quot;
    int rem;
} div_t;
typedef struct
{
    long int quot;
    long int rem;
} ldiv_t;

extern div_t div(int numerator, int denominator);
extern ldiv_t ldiv(long numerator, long denominator);

/* These are supposed to be in limits, but gcc screwed us */
#define PATH_MAX 4096
#define NAME_MAX 255
extern char *realpath(const char *path, char *resolved_path);

// PANIC function
extern void PANIC(char *sender, char *str);
int abs(int a);
_End_C_Header
