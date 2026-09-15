#ifndef MTHREAD_H_
#define MTHREAD_H_ 1

#include "attr.h"
#include "types.h"

#ifdef _WIN32
#include "mthread_win32/mthread_win32.h"
#else
#include "mthread_posix/mthread_posix.h"
#endif /* _WIN32 */

MTHREAD_API
mthread_handle_t mthread_get_thread_handle(mthread_t *thread);

MTHREAD_API
mthread_result_t mthread_create(mthread_t *MTHREAD_RESTRICT thread,
				const mthread_attr_t *MTHREAD_RESTRICT attr,
				mthread_routine_t start_routine,
				void *MTHREAD_RESTRICT arg);

MTHREAD_API
mthread_result_t mthread_join(mthread_t *MTHREAD_RESTRICT thread,
			      void **MTHREAD_RESTRICT retval);

MTHREAD_API
mthread_result_t mthread_detach(mthread_t *thread);

MTHREAD_API
void mthread_exit(void *retval);

MTHREAD_API
mthread_result_t mthread_yield(void);

MTHREAD_API
uint64_t mthread_nsleep(uint64_t ns);

MTHREAD_API
uint64_t mthread_sleep(uint64_t s);

MTHREAD_API
mthread_handle_t mthread_self(void);

MTHREAD_API
mthread_result_t mthread_attr_init(mthread_attr_t *attr);

MTHREAD_API
mthread_result_t mthread_attr_destroy(mthread_attr_t *attr);

MTHREAD_API
mthread_result_t mthread_attr_setdetachstate(mthread_attr_t *attr,
					     int32_t detachstate);

MTHREAD_API
mthread_result_t mthread_attr_getdetachstate(mthread_attr_t *MTHREAD_RESTRICT attr,
					     int32_t *MTHREAD_RESTRICT detachstate);

MTHREAD_API
mthread_result_t mthread_attr_setstacksize(mthread_attr_t *attr,
					   size_t stacksize);

MTHREAD_API
mthread_result_t mthread_attr_getstacksize(mthread_attr_t *MTHREAD_RESTRICT attr,
					   size_t *MTHREAD_RESTRICT stacksize);

MTHREAD_API
mthread_result_t mthread_attr_setstack(mthread_attr_t *MTHREAD_RESTRICT attr,
				       void *MTHREAD_RESTRICT stackaddr,
				       size_t stacksize);

MTHREAD_API
mthread_result_t mthread_attr_getstack(mthread_attr_t *MTHREAD_RESTRICT attr,
				       void **MTHREAD_RESTRICT stackaddr,
				       size_t *MTHREAD_RESTRICT stacksize);

MTHREAD_API
mthread_result_t mthread_attr_setguardsize(mthread_attr_t *attr,
					   size_t guardsize);

MTHREAD_API
mthread_result_t mthread_attr_getguradsize(mthread_attr_t *MTHREAD_RESTRICT attr,
					   size_t *MTHREAD_RESTRICT guardsize);

MTHREAD_API
mthread_result_t mthread_mutex_init(mthread_mutex_t *MTHREAD_RESTRICT mutex,
				    const mthread_mutexattr_t *MTHREAD_RESTRICT attr);

MTHREAD_API
mthread_result_t mthread_mutex_lock(mthread_mutex_t *mutex);

MTHREAD_API
mthread_result_t mthread_mutex_trylock(mthread_mutex_t *mutex);

MTHREAD_API
mthread_result_t mthread_mutex_unlock(mthread_mutex_t *mutex);

MTHREAD_API
mthread_result_t mthread_mutex_destroy(mthread_mutex_t *mutex);

MTHREAD_API
mthread_result_t mthread_mutexattr_init(mthread_mutexattr_t *attr);

MTHREAD_API
mthread_result_t mthread_mutexattr_settype(mthread_mutexattr_t *attr,
					   int32_t type);

MTHREAD_API
mthread_result_t mthread_mutexattr_gettype(mthread_mutexattr_t *MTHREAD_RESTRICT attr,
					   int32_t *MTHREAD_RESTRICT type);

MTHREAD_API
mthread_result_t mthread_mutexattr_setpshared(mthread_mutexattr_t *attr,
					      int32_t pshared);

MTHREAD_API
mthread_result_t mthread_mutexattr_getpshared(mthread_mutexattr_t *MTHREAD_RESTRICT attr,
					      int32_t *MTHREAD_RESTRICT pshared);

MTHREAD_API
mthread_result_t mthread_mutexattr_setrobust(mthread_mutexattr_t *attr,
					     int32_t robustness);

MTHREAD_API
mthread_result_t mthread_mutexattr_getrobust(mthread_mutexattr_t *MTHREAD_RESTRICT attr,
					     int32_t *MTHREAD_RESTRICT robustness);

#endif /* MTHREAD_H_ */
