// hashdb.h

#ifndef HASHDB_H
#define HASHDB_H

#include <stdint.h>

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

void insert(const char *name, uint32_t salary);
void delete(const char *name);
uint32_t search(const char *name);
void print_list();

#endif
