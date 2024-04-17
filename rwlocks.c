#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "hashdb.h"
#include "hashdb.c"
#include "rwlocks.h"


void init_rwlock(rwlock_t *lock) {
    lock->readers = 0;
    lock->writers = 0;
    lock->waiting_writers = 0;
    pthread_mutex_init(&lock->mutex, NULL);
    pthread_cond_init(&lock->read_cond, NULL);
    pthread_cond_init(&lock->write_cond, NULL);
}

void read_lock(rwlock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    while(lock->writers > 0 || lock->waiting_writers > 0) {
        pthread_cond_wait(&lock->read_cond, &lock->mutex);
    }
    lock->readers++;
    pthread_mutex_unlock(&lock->mutex);
}

void read_unlock(rwlock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->readers--;
    if(lock->readers == 0 && lock->waiting_writers > 0) {
        pthread_cond_signal(&lock->write_cond);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void write_lock(rwlock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->waiting_writers++;
    while(lock->readers > 0 || lock->writers > 0) {
        pthread_cond_wait(&lock->write_cond, &lock->mutex);
    }
    lock->waiting_writers--;
    lock->writers++;
    pthread_mutex_unlock(&lock->mutex);
}

void write_unlock(rwlock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->writers--;
    if(lock->waiting_writers > 0) {
        pthread_cond_signal(&lock->write_cond);
    } else {
        pthread_cond_broadcast(&lock->read_cond);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void destroy_rwlock(rwlock_t *lock) {
    pthread_mutex_destroy(&lock->mutex);
    pthread_cond_destroy(&lock->read_cond);
    pthread_cond_destroy(&lock->write_cond);
}
