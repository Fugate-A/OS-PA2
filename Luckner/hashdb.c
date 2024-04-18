// hashdb.c

#include "hashdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rwlocks.h"

// Initialize the list lock
RWLock list_lock = RWLOCK_INIT;

hashRecord *hashTable[100]; // Assuming size of hashTable is 100
pthread_mutex_t lock[100]; // Assuming one lock for each index in hashTable

uint32_t hashFunction(const char *name) { // Adjust parameter to const char*
    uint32_t hash = 0;
    while (*name) {
        hash += *name;
        hash += (hash << 10);
        hash ^= (hash >> 6);
        name++;
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % 100;
}

void insert(const char *name, uint32_t salary) {
    uint32_t hash = hashFunction(name);
    pthread_mutex_lock(&lock[hash]);
    hashRecord *record = malloc(sizeof(hashRecord));
    if (record == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    record->hash = hash;
    strcpy(record->name, name);
    record->salary = salary;
    record->next = hashTable[hash];
    hashTable[hash] = record;
    pthread_mutex_unlock(&lock[hash]);
}

void delete(const char *name) {
    uint32_t hash = hashFunction(name);
    pthread_mutex_lock(&lock[hash]);
    hashRecord *record = hashTable[hash];
    hashRecord *prev = NULL;
    while (record != NULL) {
        if (strcmp(record->name, name) == 0) {
            if (prev == NULL) {
                hashTable[hash] = record->next;
            } else {
                prev->next = record->next;
            }
            free(record);
            break;
        }
        prev = record;
        record = record->next;
    }
    pthread_mutex_unlock(&lock[hash]);
}

uint32_t search(const char *name) {
    uint32_t hash = hashFunction(name);
    pthread_mutex_lock(&lock[hash]);
    hashRecord *record = hashTable[hash];
    while (record != NULL) {
        if (strcmp(record->name, name) == 0) {
            uint32_t salary = record->salary;
            pthread_mutex_unlock(&lock[hash]);
            return salary;
        }
        record = record->next;
    }
    pthread_mutex_unlock(&lock[hash]);
    return 0; // Return 0 if not found
}

void print_list() {
    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(&lock[i]);
        hashRecord *record = hashTable[i];
        printf("Index %d:\n", i);
        while (record != NULL) {
            printf("Name: %s, Salary: %d\n", record->name, record->salary);
            record = record->next;
        }
        pthread_mutex_unlock(&lock[i]);
    }
}
