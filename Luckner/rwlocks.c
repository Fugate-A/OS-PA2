#include "rwlocks.h"
#include "common_threads.h"

void RWLock_init(RWLock *rwlock) {
    Mutex_init(&rwlock->lock);
    Cond_init(&rwlock->cond);
    rwlock->readers = 0;
    rwlock->writers = 0;
    rwlock->waiting_writers = 0;
}

void RWLock_acquire_readlock(RWLock *rwlock) {
    Mutex_lock(&rwlock->lock);
    while (rwlock->writers > 0 || rwlock->waiting_writers > 0) {
        Cond_wait(&rwlock->cond, &rwlock->lock);
    }
    rwlock->readers++;
    Mutex_unlock(&rwlock->lock);
}

void RWLock_release_readlock(RWLock *rwlock) {
    Mutex_lock(&rwlock->lock);
    rwlock->readers--;
    if (rwlock->readers == 0) {
        Cond_signal(&rwlock->cond);
    }
    Mutex_unlock(&rwlock->lock);
}

void RWLock_acquire_writelock(RWLock *rwlock) {
    Mutex_lock(&rwlock->lock);
    rwlock->waiting_writers++;
    while (rwlock->readers > 0 || rwlock->writers > 0) {
        Cond_wait(&rwlock->cond, &rwlock->lock);
    }
    rwlock->waiting_writers--;
    rwlock->writers++;
    Mutex_unlock(&rwlock->lock);
}

void RWLock_release_writelock(RWLock *rwlock) {
    Mutex_lock(&rwlock->lock);
    rwlock->writers--;
    Cond_signal(&rwlock->cond);
    Mutex_unlock(&rwlock->lock);
}
