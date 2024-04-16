#ifndef __hashdb_h__
#define __hashdb_h__

#include <stdint.h>

typedef struct hash_struct {
	uint32_t hash;
	char name[50];
	uint32_t salary;
	struct hash_struct *next;
} hashRecord;

#endif
