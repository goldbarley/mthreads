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
	return (mthread_handle_t)((uintptr_t) thread->size);
}

MTHREAD_API
mthread_result_t mthread_create(mthread_t *MTHREAD_RESTRICT thread,
				const mthread_attr_t *MTHREAD_RESTRICT attr,
				mthread_routine_t start_routine,
				void *MTHREAD_RESTRICT arg)
{
	if (!thread || !start_routine)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	MTHREAD_CAST_NEW(thread, struct mthread_s *, thrd);

	thrd->routine = start_routine;
	mthread_status_t status = MTHREAD_STATUS_ACTIVE;

	pthread_attr_t pattr;
	pthread_attr_init(&pattr);

	if (attr)
	{
		MTHREAD_CAST_NEW(attr, const struct mthread_attr_s *, attrib);

		int detach = (attrib->detachstate == MTHREAD_CREATE_DETACHED) ?
			PTHREAD_CREATE_DETACHED :PTHREAD_CREATE_JOINABLE;
		if (attrib->detachstate == MTHREAD_CREATE_DETACHED)
		{
			status = MTHREAD_STATUS_DETACHED;
			detach = PTHREAD_CREATE_DETACHED;
		}
		else
			detach = PTHREAD_CREATE_JOINABLE;

		pthread_attr_setdetachstate(&pattr, detach);

		if (attrib->stacksize)
			pthread_attr_setstacksize(&pattr, attrib->stacksize);

		if (attrib->stackaddr)
			pthread_attr_setstack(&pattr, attrib->stackaddr,
					      attrib->stacksize);

		if (attrib->guardsize)
			pthread_attr_setguardsize(&pattr, attrib->guardsize);
	}

	pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;
	thrd->status = status;

	int32_t error = pthread_create(&thrd->handle, &pattr,
					start_routine, arg);

	pthread_attr_destroy(&pattr);

	if (error)
	{
		thrd->status = MTHREAD_STATUS_INACTIVE;
		return error;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_join(mthread_t *MTHREAD_RESTRICT thread,
			      void **MTHREAD_RESTRICT retval)
{
	if (!thread)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	struct mthread_s *thrd = (struct mthread_s *) thread;

	if (thrd->status == MTHREAD_STATUS_JOINED ||
		thrd->status == MTHREAD_STATUS_DETACHED)
		return MTHREAD_FAILURE;

	int32_t error = pthread_join(thrd->handle, retval);
	if (!error)
	{
		thrd->status = MTHREAD_STATUS_JOINED;
		return MTHREAD_SUCCESS;
	}

	return error;
}

MTHREAD_API
mthread_result_t mthread_detach(mthread_t *thread)
{
	if (!thread)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	MTHREAD_CAST_NEW(thread, struct mthread_s *, thrd);

	if (thrd->status == MTHREAD_STATUS_JOINED ||
		thrd->status == MTHREAD_STATUS_DETACHED)
		return MTHREAD_FAILURE;

	mthread_status_t prev = thrd->status;
	thrd->status = MTHREAD_STATUS_DETACHED;

	int32_t error = pthread_detach(thrd->handle);
	if (error)
	{
		thrd->status = prev;
		return error;
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

	MTHREAD_CAST_NEW(attr, struct mthread_attr_s *, attrib);
	memset(attrib, 0, MTHREAD_SIZEOF_MTHREAD_ATTR);
	attrib->detachstate = MTHREAD_CREATE_JOINABLE;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_setdetachstate(mthread_attr_t *attr,
					     int32_t detachstate)
{
	if (!attr)
		return MTHREAD_FAILURE;

	MTHREAD_CAST_NEW(attr, struct mthread_attr_s *, attrib);

	switch (detachstate)
	{
		case MTHREAD_CREATE_JOINABLE:
		case MTHREAD_CREATE_DETACHED:
			attrib->detachstate = detachstate;
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

	*detachstate = ((struct mthread_attr_s *) attr)->detachstate;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_setstacksize(mthread_attr_t *attr,
					   size_t stacksize)
{
	if (!attr)
		return MTHREAD_FAILURE;

	((struct mthread_attr_s *) attr)->stacksize = stacksize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_getstacksize(mthread_attr_t *MTHREAD_RESTRICT attr,
					   size_t *MTHREAD_RESTRICT stacksize)
{
	if (!attr || !stacksize)
		return MTHREAD_FAILURE;

	*stacksize = ((struct mthread_attr_s *) attr)->stacksize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_setstack(mthread_attr_t *MTHREAD_RESTRICT attr,
				       void *MTHREAD_RESTRICT stackaddr,
				       size_t stacksize)
{
	if (!attr || !stackaddr)
		return MTHREAD_FAILURE;

	((struct mthread_attr_s *) attr)->stackaddr = stackaddr;
	((struct mthread_attr_s *) attr)->stacksize = stacksize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_getstack(mthread_attr_t *MTHREAD_RESTRICT attr,
				       void **MTHREAD_RESTRICT stackaddr,
				       size_t *MTHREAD_RESTRICT stacksize)
{
	if (!attr || !stackaddr || !stacksize)
		return MTHREAD_FAILURE;

	*stackaddr = ((struct mthread_attr_s *) attr)->stackaddr;
	*stacksize = ((struct mthread_attr_s *) attr)->stacksize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_setguardsize(mthread_attr_t *attr,
					   size_t guardsize)
{
	if (!attr)
		return MTHREAD_FAILURE;

	((struct mthread_attr_s *) attr)->guardsize = guardsize;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_attr_getguardsize(mthread_attr_t *MTHREAD_RESTRICT attr,
					   size_t *MTHREAD_RESTRICT guardsize)
{
	if (!attr || !guardsize)
		return MTHREAD_FAILURE;

	*guardsize = ((struct mthread_attr_s *) attr)->guardsize;

	return MTHREAD_SUCCESS;
}
