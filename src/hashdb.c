#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "hashdb.h"
#include "rwlocks.h"

hashRecord* list;
rwlock_t list_lock;

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
    size_t i = 0;
    uint32_t hash = 0;
    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

void insert(uint8_t* key, uint32_t values) {
    uint32_t hashTemp = jenkins_one_at_a_time_hash(key, strlen(key));
    write_lock(&list_lock);
    hashRecord* temp = list;
    if(temp == NULL){
        hashRecord* tempNode = (hashRecord*)malloc(sizeof(hashRecord));
        tempNode->hash = hashTemp;
        strcpy(tempNode->name, key);
        tempNode->salary = values;
        tempNode->next = NULL;
        list = tempNode;
        write_unlock(&list_lock);
        return;
    }
    while(temp->next != NULL && temp->hash != hashTemp){
        temp = temp->next;
    }
    if(temp->hash == hashTemp){
        temp->salary = values;
    }else if(temp->next == NULL){
        hashRecord* newNode = (hashRecord*)malloc(sizeof(hashRecord));
        newNode->hash = hashTemp;
        strcpy(newNode->name, key);
        newNode->salary = values;
        newNode->next = NULL;
        temp->next = newNode;
    }
    write_unlock(&list_lock);
}

void delete(key) {
    uint32_t hashTemp = jenkins_one_at_a_time_hash(key, strlen(key));
    write_lock(&list_lock);
    hashRecord* temp = list;
    if(temp->hash == hashTemp){
        list = list->next;
        free(temp);
        write_unlock(&list_lock);
        return;
    }
    while(temp->next != NULL){
        if(temp->next->hash == hashTemp){
            hashRecord* bye = temp->next;
            temp->next = bye->next;
            free(bye);
            write_unlock(&list_lock);
            return;
        }else
            temp = temp->next;
    }
    write_unlock(&list_lock);
}

uint32_t* search(key){
    uint32_t hashTemp = jenkins_one_at_a_time_hash(key, strlen(key));
    read_lock(&list_lock);
    hashRecord* temp = list;
    while(temp != NULL){
        if(temp->hash == hashTemp) {
            uint32_t* result = &(temp->salary);
            read_unlock(&list_lock);
            return result;
        } else
            temp = temp->next;
    }
    read_unlock(&list_lock);
    return NULL;
}
