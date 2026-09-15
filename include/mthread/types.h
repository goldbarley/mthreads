#ifndef MTHREAD_TYPES_H_
#define MTHREAD_TYPES_H_ 1

#include <stddef.h>
#include <stdint.h>

typedef struct mthread mthread_t;
typedef struct mthread_attr mthread_attr_t;
typedef struct mthread_mutex mthread_mutex_t;
typedef struct mthread_mutexattr mthread_mutexattr_t;

typedef void * mthread_handle_t;
typedef int32_t mthread_result_t;

enum
{
	MTHREAD_SUCCESS = 0,
	MTHREAD_FAILURE = -1
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

typedef uint64_t mthread_flags_t;

typedef void *(*mthread_routine_t)(void *);

#define MTHREAD_FALSE (0)
#define MTHREAD_TRUE (1)

typedef uint8_t mthread_bool_t;

#endif /* MTHREAD_TYPES_H_ */
