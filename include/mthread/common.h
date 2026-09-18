#ifndef MTHREAD_COMMON_H_
#define MTHREAD_COMMON_H_ 1

#ifdef __cplusplus
#define MTHREAD_BEGIN_DECLS extern "C" {
#define MTHREAD_END_DECLS }
#else
#define MTHREAD_BEGIN_DECLS
#define MTHREAD_END_DECLS
#endif /* __cplusplus */

MTHREAD_BEGIN_DECLS

#define MTHREAD_CAST_NEW(src, type, dst) type dst = (type)(src)
#define MTHREAD_COND_RET(error) return (error) ? (error) : 0
#define MTHREAD_STRING(x) #x

#define MTHREAD_ASSERT_SIZE(s1, s2) \
	_Static_assert(sizeof(s1) <= sizeof(s2), "Size mismatch between " MTHREAD_STRING(s1) " and "MTHREAD_STRING(s2)".")

#define MTHREAD_ASSERT_ALIGNMENT(s1, s2) \
	_Static_assert(_Alignof(s1) == _Alignof(s2), "Alignment mismatch between " MTHREAD_STRING(s1) " and "MTHREAD_STRING(s2)".")

#define MTHREAD_ASSERT_SIZE_N_ALIGNMENT(s1, s2) \
	MTHREAD_ASSERT_SIZE(s1, s2); MTHREAD_ASSERT_ALIGNMENT(s1, s2)

MTHREAD_END_DECLS

#endif /* MTHREAD_COMMON_H_ */
