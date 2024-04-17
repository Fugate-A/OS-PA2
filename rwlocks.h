#ifndef RWLOCKS_H
#define RWLOCKS_H

#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "hashdb.h"
#include "hashdb.c"
#include "rwlocks.h"


typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t read_cond;
    pthread_cond_t write_cond;
    int readers;
    int writers;
    int waiting_writers;
} rwlock_t;

void init_rwlock(rwlock_t *lock);
void read_lock(rwlock_t *lock);
void read_unlock(rwlock_t *lock);
void write_lock(rwlock_t *lock);
void write_unlock(rwlock_t *lock);
void destroy_rwlock(rwlock_t *lock);

#endif
