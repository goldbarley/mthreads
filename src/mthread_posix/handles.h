#ifndef MTHREAD_POSIX_HANDLES_H_
#define MTHREAD_POSIX_HANDLES_H_

#include "mthread/common.h"
#include "mthread/types.h"

MTHREAD_BEGIN_DECLS

#include <pthread.h>

struct mthread_s
{
	pthread_t handle;
	mthread_routine_t routine;
	mthread_status_t status;
};
MTHREAD_ASSERT_SIZE_N_ALIGNMENT(struct mthread_s, mthread_t);

struct mthread_attr_s
{
	int32_t detachstate;
	size_t stacksize;
	void *stackaddr;
	size_t guardsize;
};
MTHREAD_ASSERT_SIZE_N_ALIGNMENT(struct mthread_attr_s, mthread_attr_t);

struct mthread_mutex_s
{
	pthread_mutex_t handle;
	mthread_flags_t flags;
	mthread_bool_t init;
};
MTHREAD_ASSERT_SIZE_N_ALIGNMENT(struct mthread_mutex_s, mthread_mutex_t);

struct mthread_mutexattr_s
{
	int32_t type;
	int32_t pshared;
	int32_t robustness;
};
MTHREAD_ASSERT_SIZE_N_ALIGNMENT(struct mthread_mutexattr_s, mthread_mutexattr_t);

MTHREAD_END_DECLS

#endif /* MTHREAD_POSIX_HANDLES_H_ */
