#define _POSIX_C_SOURCE (200112L)

#include "handles.h"
#include "mthread/mthread.h"
#include "mthread/types.h"

#include <string.h>
#include <time.h>

#include <pthread.h>
#include <sched.h>
#include <unistd.h>

MTHREAD_API
mthread_handle_t mthread_get_platform_handle(mthread_t *thread)
{
	if (!thread)
		return NULL;
	return (mthread_handle_t)((uintptr_t) thread->handle);
}

MTHREAD_API
mthread_result_t mthread_create(mthread_t *MTHREAD_RESTRICT thread,
				const mthread_attr_t *MTHREAD_RESTRICT attr,
				mthread_routine_t start_routine,
				void *MTHREAD_RESTRICT arg)
{
	if (!thread || !start_routine)
		return MTHREAD_FAILURE;

	thread->routine = start_routine;
	mthread_status_t status = MTHREAD_STATUS_ACTIVE;

	pthread_attr_t pattr;
	pthread_attr_init(&pattr);

	if (attr)
	{
		int detach = (attr->detachstate == MTHREAD_CREATE_DETACHED) ?
			PTHREAD_CREATE_DETACHED :PTHREAD_CREATE_JOINABLE;
		if (attr->detachstate == MTHREAD_CREATE_DETACHED)
		{
			status = MTHREAD_STATUS_DETACHED;
			detach = PTHREAD_CREATE_DETACHED;
		}
		else
			detach = PTHREAD_CREATE_JOINABLE;

		pthread_attr_setdetachstate(&pattr, detach);

		if (attr->stacksize)
			pthread_attr_setstacksize(&pattr, attr->stacksize);

		if (attr->stackaddr)
			pthread_attr_setstack(&pattr, attr->stackaddr,
					      attr->stacksize);

		if (attr->guardsize)
			pthread_attr_setguardsize(&pattr, attr->guardsize);
	}

	pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;
	thread->status = status;

	int32_t result = pthread_create(&thread->handle, &pattr,
					start_routine, arg);

	pthread_attr_destroy(&pattr);

	if (result != 0)
	{
		thread->status = MTHREAD_STATUS_INACTIVE;
		return result;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_join(mthread_t *MTHREAD_RESTRICT thread,
			      void **MTHREAD_RESTRICT retval)
{
	if (!thread || thread->status == MTHREAD_STATUS_JOINED ||
		thread->status == MTHREAD_STATUS_DETACHED)
		return MTHREAD_FAILURE;

	int32_t result = pthread_join(thread->handle, retval);
	if (result == 0)
	{
		thread->status = MTHREAD_STATUS_JOINED;
		return MTHREAD_SUCCESS;
	}

	pthread_mutexattr_settype(NULL, 0);

	return MTHREAD_FAILURE;
}

MTHREAD_API
mthread_result_t mthread_detach(mthread_t *thread)
{
	if (!thread || thread->status == MTHREAD_STATUS_JOINED ||
		thread->status == MTHREAD_STATUS_DETACHED)
		return MTHREAD_FAILURE;

	mthread_status_t prev = thread->status;
	thread->status = MTHREAD_STATUS_DETACHED;

	int32_t result = pthread_detach(thread->handle);
	if (result != 0)
	{
		thread->status = prev;
		return MTHREAD_FAILURE;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
void mthread_exit(void *retval)
{
	pthread_exit(retval);
}

MTHREAD_API
mthread_result_t mthread_yield(void)
{
	return (sched_yield() == 0) ? MTHREAD_SUCCESS : MTHREAD_FAILURE;
}

MTHREAD_API
uint64_t mthread_nsleep(uint64_t ns)
{
	struct timespec ts = {
		.tv_sec = (time_t)(ns / 1000000000ULL),
		.tv_nsec = (long int)(ns % 1000000000ULL)
	};

	if (nanosleep(&ts, &ts) == -1)
		return (uint64_t)(ts.tv_sec * 1000000000ULL) +
		(uint64_t) ts.tv_nsec;

	return 0;
}

MTHREAD_API
uint64_t mthread_sleep(uint64_t s)
{
	return sleep(s);
}

MTHREAD_API
mthread_handle_t mthread_self(void)
{
	return (mthread_handle_t)((uintptr_t) pthread_self());
}

MTHREAD_API
mthread_result_t mthread_attr_init(mthread_attr_t *attr)
{
	if (!attr)
		return MTHREAD_FAILURE;

	memset(attr, 0, sizeof(struct mthread_attr));

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_setdetachstate(mthread_attr_t *attr,
					     int32_t detachstate)
{
	if (!attr)
		return MTHREAD_FAILURE;

	switch (detachstate)
	{
		case MTHREAD_CREATE_JOINABLE:
		case MTHREAD_CREATE_DETACHED:
			attr->detachstate = detachstate;
			break;
		default:
			return MTHREAD_FAILURE;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_getdetachstate(mthread_attr_t *MTHREAD_RESTRICT attr,
					     int32_t *MTHREAD_RESTRICT detachstate)
{
	if (!attr || !detachstate)
		return MTHREAD_FAILURE;

	*detachstate = attr->detachstate;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_setstacksize(mthread_attr_t *attr,
					   size_t stacksize)
{
	if (!attr)
		return MTHREAD_FAILURE;

	attr->stacksize = stacksize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_getstacksize(mthread_attr_t *MTHREAD_RESTRICT attr,
					   size_t *MTHREAD_RESTRICT stacksize)
{
	if (!attr || !stacksize)
		return MTHREAD_FAILURE;

	*stacksize = attr->stacksize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_setstack(mthread_attr_t *MTHREAD_RESTRICT attr,
				       void *MTHREAD_RESTRICT stackaddr,
				       size_t stacksize)
{
	if (!attr || !stackaddr)
		return MTHREAD_FAILURE;

	attr->stackaddr = stackaddr;
	attr->stacksize = stacksize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_getstack(mthread_attr_t *MTHREAD_RESTRICT attr,
				       void **MTHREAD_RESTRICT stackaddr,
				       size_t *MTHREAD_RESTRICT stacksize)
{
	if (!attr || !stackaddr || !stacksize)
		return MTHREAD_FAILURE;

	*stackaddr = attr->stackaddr;
	*stacksize = attr->stacksize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_setguardsize(mthread_attr_t *attr,
					   size_t guardsize)
{
	if (!attr)
		return MTHREAD_FAILURE;

	attr->guardsize = guardsize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_getguradsize(mthread_attr_t *MTHREAD_RESTRICT attr,
					   size_t *MTHREAD_RESTRICT guardsize)
{
	if (!attr || !guardsize)
		return MTHREAD_FAILURE;

	*guardsize = attr->guardsize;

	return MTHREAD_SUCCESS;
}
