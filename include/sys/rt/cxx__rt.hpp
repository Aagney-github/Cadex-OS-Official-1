/**
 * Copyright (C) 2019-2020 The CadexOS Project
 * This software is distributed under the GNU General Public License
 * See the file LICENSE for details
 */

/**
 * Standard C++ runtime functions
 * http://wiki.osdev.org/C%2B%2B
 */
#ifndef CXX__RT
#define CXX__RT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_ATEXIT_ENTRIES 128

// In userspace this will throw error
extern "C" void __cxa_pure_virtual() {
    // do nothing
}

struct cxa_ref_s {
    void (*f)(void *);
    void *arg;
    void *dso;
};

cxa_ref_s refs[MAX_ATEXIT_ENTRIES];
int refs_len = 0;

extern "C" int __cxa_atexit(void (*func)(void *), void *arg, void *dso_handle) {
    if (refs_len >= MAX_ATEXIT_ENTRIES)
        return -1;
    refs[refs_len].f = func;
    refs[refs_len].arg = arg;
    refs[refs_len++].dso = dso_handle;

    return 0;
}

// extern "C" void __cxa_finalize(void *d)
// {
//     if (!d)
//     {
//         for (int i = refs_len - 1; i >= 0; --i)
//         {
//             if (refs[i].f)
//                 refs[i].f(refs[i].arg);
//         }
//         return;
//     }

//     // else
//     for (int i = refs_len - 1; i >= 0; --i)
//     {
//         if (refs[i].f == d)
//         {
//             refs[i].f(refs[i].arg);
//             refs[i].f = NULL;
//         }
//     }
// }

// not thread safe
// should add a mutex-like guard with a test-and-set primitive.
namespace __cxxabiv1 {
/* guard variables */

/* The ABI requires a 64-bit type.  */
__extension__ typedef int __guard __attribute__((mode(__DI__)));

extern "C" int __cxa_guard_acquire(__guard *g) { return !*(char *)(g); }

extern "C" void __cxa_guard_release(__guard *g) { *(char *)g = 1; }

extern "C" void __cxa_guard_abort(__guard *) {}
} // namespace __cxxabiv1

void operator delete(void *ptr, size_t len) { free(ptr); }

void *operator new(unsigned int len) { return malloc(len); }

void operator delete[](void *ptr) { ::operator delete(ptr, sizeof(ptr)); }

void *operator new[](unsigned int len) { return ::operator new(len); }
#endif
