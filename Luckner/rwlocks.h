#ifndef RWLOCKS_H
#define RWLOCKS_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int readers;
    int writers;
    int waiting_writers;
} RWLock;

#define RWLOCK_INIT { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0, 0, 0 }

void RWLock_init(RWLock *rwlock);
void RWLock_acquire_readlock(RWLock *rwlock);
void RWLock_release_readlock(RWLock *rwlock);
void RWLock_acquire_writelock(RWLock *rwlock);
void RWLock_release_writelock(RWLock *rwlock);

#endif
