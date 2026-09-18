#define _POSIX_C_SOURCE (200112L)

#include "mthread/common.h"
#include "handles.h"
#include "mthread/attr.h"
#include "mthread/types.h"
#include "mthread/mthread.h"

#include <pthread.h>
#include <string.h>

MTHREAD_API
mthread_result_t mthread_cond_init(mthread_cond_t *MTHREAD_RESTRICT cond,
				   mthread_condattr_t *cond_attr)
{
	if (!cond)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	MTHREAD_CAST_NEW(cond, struct mthread_cond_s *, cnd);

	if (cnd->init)
		return MTHREAD_FAILURE;

	pthread_condattr_t pattr;

	int32_t error = MTHREAD_SUCCESS;

	if (cond_attr)
	{
		MTHREAD_CAST_NEW(cond_attr, struct mthread_condattr_s *, cnd_attr);

		error = pthread_condattr_init(&pattr);
		if (error)
			return error;

		int32_t attr_field = 0;
		switch (cnd_attr->pshared)
		{
			case MTHREAD_PROCESS_PRIVATE:
				attr_field = PTHREAD_PROCESS_PRIVATE;
				break;
			case MTHREAD_PROCESS_SHARED:
				attr_field = PTHREAD_PROCESS_SHARED;
				break;
			default:
				goto cleanup;
		}

		error = pthread_condattr_setpshared(&pattr, attr_field);
		if (error)
			goto cleanup;

		error = pthread_condattr_setclock(&pattr, cnd_attr->clock_id);
		if (error)
			goto cleanup;
	}

	error = pthread_cond_init(&cnd->handle, &pattr);
	if (error)
		goto cleanup;

	cnd->init = MTHREAD_TRUE;

	cleanup:
	pthread_condattr_destroy(&pattr);

	MTHREAD_COND_RET(error);
}

MTHREAD_API
mthread_result_t mthread_cond_signal(mthread_cond_t *cond)
{
	if (!cond)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	return pthread_cond_signal(&((struct mthread_cond_s *) cond)->handle);
}

MTHREAD_API
mthread_result_t mthread_cond_broadcast(mthread_cond_t *cond)
{
	if (!cond)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	return pthread_cond_broadcast(&((struct mthread_cond_s *) cond)->handle);
}

MTHREAD_API
mthread_result_t mthread_cond_wait(mthread_cond_t *MTHREAD_RESTRICT cond,
				   mthread_mutex_t * MTHREAD_RESTRICT mutex)
{
	if (!cond || !mutex)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	return pthread_cond_wait(&((struct mthread_cond_s *) cond)->handle,
				 &((struct mthread_mutex_s *) mutex)->handle);
}

MTHREAD_API
mthread_result_t mthread_cond_timedwait(mthread_cond_t *MTHREAD_RESTRICT cond,
					mthread_mutex_t *MTHREAD_RESTRICT mutex,
					const struct timespec *MTHREAD_RESTRICT abstime)
{
	if (!cond || !mutex || !abstime)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	return pthread_cond_timedwait(&((struct mthread_cond_s *) cond)->handle,
				      &((struct mthread_mutex_s *) mutex)->handle,
				      abstime);
}

MTHREAD_API
mthread_result_t mthread_cond_destroy(mthread_cond_t *cond)
{
	if (!cond)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	return pthread_cond_destroy(&((struct mthread_cond_s *) cond)->handle);
}

MTHREAD_API
mthread_result_t mthread_condattr_init(mthread_condattr_t *attr)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	memset(attr, 0, MTHREAD_SIZEOF_MTHREAD_CONDATTR);

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_condattr_destroy(mthread_condattr_t *attr)
{
	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_condattr_setpshared(mthread_condattr_t *attr,
					     int32_t pshared)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	switch (pshared)
	{
		case MTHREAD_PROCESS_PRIVATE:
		case MTHREAD_PROCESS_SHARED:
			((struct mthread_condattr_s *) attr)->pshared = pshared;
			break;
		default:
			return MTHREAD_ERROR_INVALID_ARGUMENT;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_condattr_getpshared(const mthread_condattr_t *MTHREAD_RESTRICT attr,
					     int32_t *MTHREAD_RESTRICT pshared)
{
	if (!attr || !pshared)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	*pshared = ((struct mthread_condattr_s *) attr)->pshared;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_condattr_setclock(mthread_condattr_t *attr,
					   int32_t clock_id)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	((struct mthread_condattr_s *) attr)->clock_id = clock_id;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_condattr_getclock(const mthread_condattr_t *MTHREAD_RESTRICT attr,
					   int32_t *MTHREAD_RESTRICT clock_id)
{
	if (!attr || !clock_id)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	*clock_id = ((struct mthread_condattr_s *) attr)->clock_id;

	return MTHREAD_SUCCESS;
}
