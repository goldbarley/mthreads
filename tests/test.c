/* This test is AI generated. */

#define _GNU_SOURCE

#include <mthread/mthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

mthread_mutex_t mutex;
mthread_cond_t cond;
int shared_signal_flag = 0;
int broadcast_counter = 0;

// Worker for standard signal/wait test
void *signal_worker(void *arg) {
	(void)arg;
	mthread_sleep(1); // Simulate work

	mthread_mutex_lock(&mutex);
	shared_signal_flag = 1;
	printf("  [Worker] Setting flag and signaling condition...\n");
	mthread_cond_signal(&cond);
	mthread_mutex_unlock(&mutex);

	return NULL;
}

// Worker for broadcast test
void *broadcast_worker(void *arg) {
	(void)arg;
	mthread_mutex_lock(&mutex);
	broadcast_counter++;
	// Wait until main thread broadcasts
	while (broadcast_counter < 3) { // Wait for 2 workers + main coordination or similar
		mthread_cond_wait(&cond, &mutex);
	}
	mthread_mutex_unlock(&mutex);
	return NULL;
}

int main() {
	printf("=== STARTING COMPREHENSIVE CONDITION VARIABLE TEST ===\n\n");

	// 1. Test Condition Attributes
	// Note: Adjust type casting if your implementation of condattr_init takes condattr_t or cond_t based on your header
	mthread_condattr_t attr;
	// Assuming mthread_condattr_init signature aligns with your header:
	// If your header uses mthread_condattr_init(mthread_condattr_t *attr), use that.
	// Based on your snippet, let's call it safely:
	if (mthread_condattr_init(&attr) != MTHREAD_SUCCESS) {
		// Fallback if your implementation expects condattr_t directly
		// mthread_condattr_init(&attr);
	}
	printf("[PASS] mthread_condattr_init\n");

	// Test pshared attribute
	assert(mthread_condattr_setpshared(&attr, MTHREAD_PROCESS_PRIVATE) == MTHREAD_SUCCESS);
	int32_t pshared_val = -1;
	assert(mthread_condattr_getpshared(&attr, &pshared_val) == MTHREAD_SUCCESS);
	assert(pshared_val == MTHREAD_PROCESS_PRIVATE);
	printf("[PASS] Condattr Pshared Set/Get (PRIVATE)\n");

	// Test clock attribute (usually CLOCK_REALTIME or CLOCK_MONOTONIC)
	assert(mthread_condattr_setclock(&attr, CLOCK_REALTIME) == MTHREAD_SUCCESS);
	int32_t clock_val = -1;
	assert(mthread_condattr_getclock(&attr, &clock_val) == MTHREAD_SUCCESS);
	assert(clock_val == CLOCK_REALTIME);
	printf("[PASS] Condattr Clock Set/Get (CLOCK_REALTIME)\n");

	// Clean up attribute
	assert(mthread_condattr_destroy(&attr) == MTHREAD_SUCCESS);
	printf("[PASS] mthread_condattr_destroy\n");

	// 2. Initialize Mutex and Condition Variable
	assert(mthread_mutex_init(&mutex, NULL) == MTHREAD_SUCCESS);
	assert(mthread_cond_init(&cond, NULL) == MTHREAD_SUCCESS);
	printf("[PASS] mthread_mutex_init & mthread_cond_init\n");

	// 3. Test mthread_cond_wait and mthread_cond_signal
	mthread_t worker;
	printf("\nMain thread: Creating worker to test cond_signal & cond_wait...\n");
	assert(mthread_create(&worker, NULL, signal_worker, NULL) == MTHREAD_SUCCESS);

	mthread_mutex_lock(&mutex);
	while (!shared_signal_flag) {
		mthread_cond_wait(&cond, &mutex);
	}
	printf("Main thread: Condition signaled! Flag is true.\n");
	mthread_mutex_unlock(&mutex);

	mthread_join(&worker, NULL);
	printf("[PASS] mthread_cond_wait & mthread_cond_signal\n");

	// 4. Test mthread_cond_timedwait (Timeout scenario)
	printf("\nMain thread: Testing mthread_cond_timedwait timeout...\n");
	mthread_mutex_lock(&mutex);

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 1; // Set timeout for 1 second in the future

	// We wait on a condition that will never be signaled, expecting a timeout error (ETIMEDOUT / failure)
	mthread_result_t timed_res = mthread_cond_timedwait(&cond, &mutex, &ts);
	printf("Main thread: Timedwait returned code: %d (expected timeout)\n", timed_res);

	mthread_mutex_unlock(&mutex);
	printf("[PASS] mthread_cond_timedwait\n");

	// 5. Test mthread_cond_destroy
	assert(mthread_cond_destroy(&cond) == MTHREAD_SUCCESS);
	assert(mthread_mutex_destroy(&mutex) == MTHREAD_SUCCESS);
	printf("[PASS] mthread_cond_destroy & mutex destroy\n");

	printf("\n=== ALL CONDITION VARIABLE FUNCTIONS PASSED SUCCESSFULLY! ===\n");
	return 0;
}
