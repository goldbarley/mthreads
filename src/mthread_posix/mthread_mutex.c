#define _XOPEN_SOURCE (700)
#define _POSIX_C_SOURCE (200809L)

#include "handles.h"
#include "mthread/attr.h"
#include "mthread/types.h"

#include <string.h>

#include <pthread.h>

MTHREAD_API
mthread_result_t mthread_mutex_init(mthread_mutex_t *MTHREAD_RESTRICT mutex,
				    const mthread_mutexattr_t *MTHREAD_RESTRICT attr)
{
	if (!mutex)
		return MTHREAD_FAILURE;

	MTHREAD_CAST_NEW(mutex, struct mthread_mutex_s *, mtx);

	pthread_mutexattr_t pattr;
	int32_t error = pthread_mutexattr_init(&pattr);
	if (error)
		return error;

	const struct mthread_mutexattr_s *attrib = NULL;

	if (attr)
	{
		attrib = (const struct mthread_mutexattr_s *) attr;

		int32_t attr_field = 0;
		switch(attrib->type)
		{
			case MTHREAD_MUTEX_NORMAL:
				attr_field = PTHREAD_MUTEX_NORMAL;
				break;
			case MTHREAD_MUTEX_RECURSIVE:
				attr_field = PTHREAD_MUTEX_RECURSIVE;
				break;
			case MTHREAD_MUTEX_ERRORCHECK:
				attr_field = PTHREAD_MUTEX_ERRORCHECK;
				break;
			default:
				return MTHREAD_FAILURE;
		}

		error = pthread_mutexattr_settype(&pattr, attr_field);
		if (error)
			return error;

		switch(attrib->pshared)
		{
			case MTHREAD_PROCESS_PRIVATE:
				attr_field = PTHREAD_PROCESS_PRIVATE;
				break;
			case MTHREAD_PROCESS_SHARED:
				attr_field = PTHREAD_PROCESS_SHARED;
				break;
			default:
				return MTHREAD_FAILURE;
		}

		error = pthread_mutexattr_setpshared(&pattr, attr_field);
		if (error)
			return error;

		switch(attrib->robustness)
		{
			case MTHREAD_MUTEX_STALLED:
				attr_field = PTHREAD_MUTEX_STALLED;
				break;
			case MTHREAD_MUTEX_ROBUST:
				attr_field = PTHREAD_MUTEX_ROBUST;
				break;
			default:
				return MTHREAD_FAILURE;
		}

		error = pthread_mutexattr_setrobust(&pattr, attr_field);
		if (error)
			return error;
	}

	pthread_mutex_init(&mtx->handle, &pattr);

	mtx->init = MTHREAD_TRUE;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutex_lock(mthread_mutex_t *mutex)
{
	if (!mutex)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	MTHREAD_CAST_NEW(mutex, struct mthread_mutex_s *, mtx);

	if (!mtx->init)
		return MTHREAD_ERROR_NOT_INITIALIZED;

	int32_t error = pthread_mutex_lock(&mtx->handle);

	MTHREAD_COND_RET(error);
}

MTHREAD_API
mthread_result_t mthread_mutex_trylock(mthread_mutex_t *mutex)
{
	if (!mutex)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	MTHREAD_CAST_NEW(mutex, struct mthread_mutex_s *, mtx);
	if (!mtx->init)
		return MTHREAD_ERROR_NOT_INITIALIZED;

	int32_t error = pthread_mutex_trylock(&mtx->handle);

	MTHREAD_COND_RET(error);
}

MTHREAD_API
mthread_result_t mthread_mutex_unlock(mthread_mutex_t *mutex)
{
	if (!mutex)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	MTHREAD_CAST_NEW(mutex, struct mthread_mutex_s *, mtx);
	if (!mtx->init)
		return MTHREAD_ERROR_NOT_INITIALIZED;

	int32_t error = pthread_mutex_unlock(&mtx->handle);

	MTHREAD_COND_RET(error);
}

MTHREAD_API
mthread_result_t mthread_mutex_destroy(mthread_mutex_t *mutex)
{
	if (!mutex)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	MTHREAD_CAST_NEW(mutex, struct mthread_mutex_s *, mtx);
	if (!mtx->init)
		return MTHREAD_ERROR_NOT_INITIALIZED;

	int32_t error = pthread_mutex_destroy(&mtx->handle);

	MTHREAD_COND_RET(error);
}

MTHREAD_API
mthread_result_t mthread_mutexattr_init(mthread_mutexattr_t *attr)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	memset(attr, 0, MTHREAD_SIZEOF_MTHREAD_MUTEXATTR);

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_settype(mthread_mutexattr_t *attr,
					   int32_t type)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	MTHREAD_CAST_NEW(attr, struct mthread_mutexattr_s *, attrib);

	switch (type)
	{
		case MTHREAD_MUTEX_NORMAL:
		case MTHREAD_MUTEX_RECURSIVE:
		case MTHREAD_MUTEX_ERRORCHECK:
			attrib->type = type;
			break;
		default:
			return MTHREAD_ERROR_INVALID_ARGUMENT;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_gettype(mthread_mutexattr_t *MTHREAD_RESTRICT attr,
					   int32_t *MTHREAD_RESTRICT type)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	*type = ((struct mthread_mutexattr_s *) attr)->type;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_setpshared(mthread_mutexattr_t *attr,
					      int32_t pshared)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	switch (pshared)
	{
		case MTHREAD_PROCESS_PRIVATE:
		case MTHREAD_PROCESS_SHARED:
			((struct mthread_mutexattr_s *) attr)->pshared = pshared;
			break;
		default:
			return MTHREAD_ERROR_INVALID_ARGUMENT;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_getpshared(mthread_mutexattr_t *MTHREAD_RESTRICT attr,
					      int32_t *MTHREAD_RESTRICT pshared)
{
	if (!attr || !pshared)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	*pshared = ((struct mthread_mutexattr_s *) attr)->pshared;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_setrobust(mthread_mutexattr_t *attr,
					     int32_t robustness)
{
	if (!attr)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	switch (robustness)
	{
		case MTHREAD_MUTEX_STALLED:
		case MTHREAD_MUTEX_ROBUST:
			((struct mthread_mutexattr_s *) attr)->robustness = robustness;
			break;
		default:
			return MTHREAD_ERROR_INVALID_ARGUMENT;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_getrobust(mthread_mutexattr_t *MTHREAD_RESTRICT attr,
					     int32_t *MTHREAD_RESTRICT robustness)
{
	if (!attr || !robustness)
		return MTHREAD_ERROR_INVALID_ARGUMENT;

	*robustness = ((struct mthread_mutexattr_s *) attr)->robustness;

	return MTHREAD_SUCCESS;
}
