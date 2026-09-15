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
	if (!mutex || mutex->init)
		return MTHREAD_FAILURE;

	pthread_mutexattr_t pattr;
	int32_t error = pthread_mutexattr_init(&pattr);
	if (error)
		return error;

	if (attr)
	{
		int32_t type = 0;
		switch(attr->type)
		{
			case MTHREAD_MUTEX_NORMAL:
				type = PTHREAD_MUTEX_NORMAL;
				break;
			case MTHREAD_MUTEX_RECURSIVE:
				type = PTHREAD_MUTEX_RECURSIVE;
				break;
			case MTHREAD_MUTEX_ERRORCHECK:
				type = PTHREAD_MUTEX_ERRORCHECK;
			default:
				return MTHREAD_FAILURE;
		}

		pthread_mutexattr_settype(&pattr, type);
	}

	pthread_mutex_init(&mutex->handle, NULL);

	mutex->init = MTHREAD_TRUE;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutex_lock(mthread_mutex_t *mutex)
{
	if (!mutex || !mutex->init)
		return MTHREAD_FAILURE;

	int32_t result = pthread_mutex_lock(&mutex->handle);
	if (result != 0)
		return result;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutex_trylock(mthread_mutex_t *mutex)
{
	if (!mutex || !mutex->init)
		return MTHREAD_FAILURE;

	int32_t result = pthread_mutex_lock(&mutex->handle);
	if (result != 0)
		return result;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutex_unlock(mthread_mutex_t *mutex)
{
	if (!mutex || !mutex->init)
		return MTHREAD_FAILURE;

	int32_t result = pthread_mutex_unlock(&mutex->handle);
	if (result != 0)
		return result;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutex_destroy(mthread_mutex_t *mutex)
{
	if (!mutex || !mutex->init)
		return MTHREAD_FAILURE;

	int32_t result = pthread_mutex_unlock(&mutex->handle);
	if (result != 0)
		return result;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_init(mthread_mutexattr_t *attr)
{
	if (!attr)
		return MTHREAD_FAILURE;

	memset(attr, 0, sizeof(struct mthread_mutexattr));

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_settype(mthread_mutexattr_t *attr,
					   int32_t type)
{
	if (!attr)
		return MTHREAD_FAILURE;

	switch (type)
	{
		case MTHREAD_MUTEX_NORMAL:
		case MTHREAD_MUTEX_RECURSIVE:
		case MTHREAD_MUTEX_ERRORCHECK:
			attr->type = type;
			break;
		default:
			return MTHREAD_FAILURE;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_gettype(mthread_mutexattr_t *MTHREAD_RESTRICT attr,
					   int32_t *MTHREAD_RESTRICT type)
{
	if (!attr)
		return MTHREAD_FAILURE;

	*type = attr->type;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_setpshared(mthread_mutexattr_t *attr,
					      int32_t pshared)
{
	if (!attr)
		return MTHREAD_FAILURE;

	switch (pshared)
	{
		case MTHREAD_PROCESS_PRIVATE:
		case MTHREAD_PROCESS_SHARED:
			attr->pshared = pshared;
			break;
		default:
			return MTHREAD_FAILURE;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_getpshared(mthread_mutexattr_t *MTHREAD_RESTRICT attr,
					      int32_t *MTHREAD_RESTRICT pshared)
{
	if (!attr || !pshared)
		return MTHREAD_FAILURE;

	*pshared = attr->pshared;

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_setrobust(mthread_mutexattr_t *attr,
					     int32_t robustness)
{
	if (!attr)
		return MTHREAD_FAILURE;

	switch (robustness)
	{
		case MTHREAD_MUTEX_STALLED:
		case MTHREAD_MUTEX_ROBUST:
			attr->robustness = robustness;
			break;
		default:
			return MTHREAD_FAILURE;
	}

	return MTHREAD_SUCCESS;
}

MTHREAD_API
mthread_result_t mthread_mutexattr_getrobust(mthread_mutexattr_t *MTHREAD_RESTRICT attr,
					     int32_t *MTHREAD_RESTRICT robustness)
{
	if (!attr || !robustness)
		return MTHREAD_FAILURE;

	*robustness = attr->robustness;

	return MTHREAD_SUCCESS;
}
