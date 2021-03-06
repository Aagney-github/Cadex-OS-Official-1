#ifndef A3CF21CF_99C5_4632_B55F_0EEE21FE3AF9
#define A3CF21CF_99C5_4632_B55F_0EEE21FE3AF9

#include <library/types.h>
#include <kernel/types.h>
#include <stddef.h>
#include <bits/types/locale_t.h>

#include <bits/cake_src.h>

#define _REDIR_TIME64 1
#define _Addr         int
#define _Int64        long long
#define _Reg          int

#define __BYTE_ORDER 1234
#define __LONG_MAX   0x7fffffffL

#ifndef __cplusplus
#ifdef __WCHAR_TYPE__
typedef __WCHAR_TYPE__ wchar_t;
#else
typedef long wchar_t;
#endif
#endif

#if defined(__FLT_EVAL_METHOD__) && __FLT_EVAL_METHOD__ == 0
typedef float float_t;
typedef double double_t;
#else
typedef long double float_t;
typedef long double double_t;
#endif

#if !defined(__cplusplus)
typedef struct {
    _Alignas(8) long long __ll;
    long double __ld;
} max_align_t;
#elif defined(__GNUC__)
typedef struct {
    __attribute__((__aligned__(8))) long long __ll;
    long double __ld;
} max_align_t;
#else
typedef struct {
    alignas(8) long long __ll;
    long double __ld;
} max_align_t;
#endif

#endif /* A3CF21CF_99C5_4632_B55F_0EEE21FE3AF9 */
