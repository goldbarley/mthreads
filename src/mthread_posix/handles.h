#ifndef MTHREAD_POSIX_HANDLES_H_
#define MTHREAD_POSIX_HANDLES_H_

#include "mthread/types.h"

#include <pthread.h>

struct mthread
{
	pthread_t handle;
	mthread_routine_t routine;
	mthread_status_t status;
};

struct mthread_attr
{
	int32_t detachstate;
	size_t stacksize;
	void *stackaddr;
	size_t guardsize;
};

struct mthread_mutex
{
	pthread_mutex_t handle;
	mthread_flags_t flags;
	mthread_bool_t init;
};

struct mthread_mutexattr
{
	int32_t type;
	int32_t pshared;
	int32_t robustness;
};

#endif /* MTHREAD_POSIX_HANDLES_H_ */
