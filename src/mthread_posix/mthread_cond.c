#define _POSIX_C_SOURCE (200112L)

#include "mthread/common.h"
#include "handles.h"
#include "mthread/attr.h"
#include "mthread/types.h"
#include "mthread/mthread.h"

#include <pthread.h>
#include <stdlib.h>

MTHREAD_API
mthread_result_t mthread_cond_init(mthread_cond_t **MTHREAD_RESTRICT cond,
				   mthread_condattr_t *cond_attr)
{
	if (!cond)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	mthread_cond_t *cnd = malloc(sizeof(struct mthread_cond));
	if (!cnd)
		return MTHREAD_ERROR_NOT_INITIALIZED;

	pthread_condattr_t pattr = {0};

	int32_t error = MTHREAD_SUCCESS;

	if (cond_attr)
	{
		error = pthread_condattr_init(&pattr);
		if (error)
			return error;

		int32_t attr_field = 0;
		switch (cond_attr->pshared)
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

		error = pthread_condattr_setclock(&pattr, cond_attr->clock_id);
		if (error)
			goto cleanup;
	}

	error = pthread_cond_init(&cnd->handle, &pattr);
	if (error)
		goto cleanup;

	cnd->init = MTHREAD_TRUE;

	*cond = cnd;

	cleanup:
	pthread_condattr_destroy(&pattr);

	return error ? error : MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_cond_signal(mthread_cond_t *cond)
{
	if (!cond)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	return pthread_cond_signal(&cond->handle);
}

MTHREAD_API
mthread_result_t mthread_cond_broadcast(mthread_cond_t *cond)
{
	if (!cond)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	return pthread_cond_broadcast(&cond->handle);
}

MTHREAD_API
mthread_result_t mthread_cond_wait(mthread_cond_t *MTHREAD_RESTRICT cond,
				   mthread_mutex_t * MTHREAD_RESTRICT mutex)
{
	if (!cond || !mutex)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	return pthread_cond_wait(&cond->handle, &mutex->handle);
}

MTHREAD_API
mthread_result_t mthread_cond_timedwait(mthread_cond_t *MTHREAD_RESTRICT cond,
					mthread_mutex_t *MTHREAD_RESTRICT mutex,
					const struct timespec *MTHREAD_RESTRICT abstime)
{
	if (!cond || !mutex || !abstime)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	return pthread_cond_timedwait(&cond->handle, &mutex->handle,
				      abstime);
}

MTHREAD_API
mthread_result_t mthread_cond_destroy(mthread_cond_t *cond)
{
	if (!cond)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	int32_t error = pthread_cond_destroy(&cond->handle);
	free(cond);

	return error ? error : MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_condattr_init(mthread_condattr_t **attr)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	*attr = calloc(1, sizeof(struct mthread_condattr));
	if (!attr)
		return MTHREAD_ERROR_NOT_INITIALIZED;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_condattr_destroy(mthread_condattr_t *attr)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	free(attr);

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
			attr->pshared = pshared;
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

	*pshared = attr->pshared;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_condattr_setclock(mthread_condattr_t *attr,
					   int32_t clock_id)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	attr->clock_id = clock_id;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_condattr_getclock(const mthread_condattr_t *MTHREAD_RESTRICT attr,
					   int32_t *MTHREAD_RESTRICT clock_id)
{
	if (!attr || !clock_id)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	*clock_id = attr->clock_id;

	return MTHREAD_SUCCESS;
}
