#define _XOPEN_SOURCE 600
#include "testlib.h"
#include "multi_mutex.h"
#include <pthread.h>
#include <errno.h>

#define MUTEX_C 10
#define UNLOCK_C 4

int main() {
	test_start("You lock all mutexes in your multi_mutex_trylock.");

	pthread_mutex_t m[MUTEX_C];
	pthread_mutex_t *mp[MUTEX_C];
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);

	for (int i = 0; i < MUTEX_C; i++) {
		pthread_mutex_init(&m[i], &attr);
		mp[i] = &m[i];
	}

	test_equals_int(multi_mutex_trylock(mp, UNLOCK_C), 0, "multi_mutex_trylock succeeds");

	for (int i = 0; i < UNLOCK_C; i++) {
		test_equals_int(pthread_mutex_unlock(&m[i]), 0, "pthread_mutex_unlock succeeds");
	}

	for (int i = UNLOCK_C; i < MUTEX_C; i++) {
		test_equals_int(pthread_mutex_unlock(&m[i]), EPERM, "pthread_mutex_unlock fails with EPERM");
	}

	pthread_mutex_t m2[MUTEX_C]; //Create new mutex set
	pthread_mutex_t *mp2[MUTEX_C];
	pthread_mutexattr_t attr2;
	pthread_mutexattr_init(&attr2);
	pthread_mutexattr_settype(&attr2, PTHREAD_MUTEX_NORMAL);
	pthread_mutexattr_setrobust(&attr2, PTHREAD_MUTEX_ROBUST); //Make mutex_unlock return an error if already unlocked
	for (int i = 0; i < MUTEX_C; i++) {
		pthread_mutex_init(&m2[i], &attr2);
		mp2[i] = &m2[i];
	}

    pthread_mutex_lock(&m2[MUTEX_C - 2]); //Lock the last 2 mutexes
    pthread_mutex_lock(&m2[MUTEX_C - 1]);
    test_equals_int(multi_mutex_trylock(mp2, MUTEX_C), -1, "multi_mutex_trylock fails if one mutex is already locked");
	for (int i = 0; i < MUTEX_C-2; i++) {
		test_equals_int(pthread_mutex_unlock(&m2[i]), EPERM, "pthread_mutex_unlock fails with EPERM (multi_mutex_unlock releases locks when one couldn't be acquired)");
	}
	for (int i = MUTEX_C - 2; i < MUTEX_C; i++) {
		test_equals_int(pthread_mutex_unlock(&m2[i]), 0, "pthread_mutex_unlock succeeds (multy_mutex_unlock leaves locked mutexes locked)");
	}

	return test_end();
}

