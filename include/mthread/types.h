#ifndef MTHREAD_TYPES_H_
#define MTHREAD_TYPES_H_ 1

#include "common.h"

MTHREAD_BEGIN_DECLS

#include <stddef.h>
#include <stdint.h>

#define MTHREAD_SIZEOF_MTHREAD (24U)
#define MTHREAD_SIZEOF_MTHREAD_ATTR (32U)
#define MTHREAD_SIZEOF_MTHREAD_MUTEX (48U)
#define MTHREAD_SIZEOF_MTHREAD_MUTEXATTR (12U)
#define MTHREAD_SIZEOF_MTHREAD_COND (56U)
#define MTHREAD_SIZEOF_MTHREAD_CONDATTR (8U)


typedef struct mthread mthread_t;
typedef struct mthread_attr mthread_attr_t;
typedef struct mthread_mutex mthread_mutex_t;
typedef struct mthread_mutexattr mthread_mutexattr_t;
typedef struct mthread_cond mthread_cond_t;
typedef struct mthread_condattr mthread_condattr_t;

typedef uintptr_t mthread_handle_t;
typedef int32_t mthread_result_t;

enum
{
	MTHREAD_SUCCESS = 0,
	MTHREAD_FAILURE = -1,
	MTHREAD_ERROR_NOT_INITIALIZED = -2,
	MTHREAD_ERROR_INVALID_ARGUMENT = -3
};

/* Detach state. */
enum
{
	MTHREAD_CREATE_JOINABLE = 0,
	MTHREAD_CREATE_DETACHED = 1
};

/* Mutex types. */
enum
{
	MTHREAD_MUTEX_NORMAL = 0,
	MTHREAD_MUTEX_RECURSIVE = 1,
	MTHREAD_MUTEX_ERRORCHECK = 2
};

/* Mutex process shared. */
enum
{
	MTHREAD_PROCESS_PRIVATE = 0,
	MTHREAD_PROCESS_SHARED = 1
};

/* Mutex robustness. */
enum
{
	MTHREAD_MUTEX_STALLED = 0,
	MTHREAD_MUTEX_ROBUST = 1
};

/* Mutex protocols. */
enum
{
	MTHREAD_PRIO_NONE,
	MTHREAD_PRIO_INHERIT,
	MTHREAD_PRIO_PROTECT
};

typedef enum mthread_status
{
	MTHREAD_STATUS_INACTIVE = 0,
	MTHREAD_STATUS_ACTIVE = 1,
	MTHREAD_STATUS_JOINED = 2,
	MTHREAD_STATUS_DETACHED = 3,
	MTHREAD_STATUS_MAX_VALUE = 0x7FFFFFFF
} mthread_status_t;

typedef uint32_t mthread_flags_t;

typedef void *(*mthread_routine_t)(void *);

#define MTHREAD_FALSE (0)
#define MTHREAD_TRUE (1)

typedef unsigned char mthread_bool_t;

MTHREAD_END_DECLS

#endif /* MTHREAD_TYPES_H_ */
