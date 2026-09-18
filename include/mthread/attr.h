#ifndef MTHREAD_ATTR_H_
#define MTHREAD_ATTR_H_ 1

#include "common.h"

MTHREAD_BEGIN_DECLS

#if defined(_MSC_VER)
#ifdef MTHREAD_LIB_STATIC
#define MTHREAD_API
#elif defined(MTHREAD_LIB_SHARED)
#define MTHREAD_API __declspec(dllexport)
#else
#define MTHREAD_API __declspec(dllimport)
#endif
#define MTHREAD_INLINE static __forceinline
#else
#ifdef MTHREAD_LIB_SHARED
#define MTHREAD_API __attribute__((__visibility__("default")))
#else
#define MTHREAD_API
#endif /* MTHREAD_LIB_SHARED */
#define MTHREAD_INLINE static inline __attribute__((__always_inline__))
#endif /* Compilers */

#if defined(__GNUC__) || defined(__clang__)
#define MTHREAD_HOT __attribute__((__hot__))
#else
#define MTHREAD_HOT
#endif /* Compilers */

#ifdef __cplusplus
#define MTHREAD_RESTRICT __restrict
#else
#define MTHREAD_RESTRICT restrict
#endif /* __cplusplus */

MTHREAD_END_DECLS

#endif /* MTHREAD_ATTR_H_ */
