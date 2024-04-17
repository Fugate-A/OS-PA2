#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "hashdb.h"
#include "hashdb.c"
#include "rwlocks.h"


rwlock_t list_lock;

void print_list() {
    read_lock(&list_lock);
    hashRecord* temp = list;
    while(temp != NULL) {
        printf("%u,%s,%u\n", temp->hash, temp->name, temp->salary);
        temp = temp->next;
    }
    read_unlock(&list_lock);
}

void process_command(char* command, char* param1, char* param2) {
    if(strcmp(command, "insert") == 0) {
        insert(param1, atoi(param2));
    } else if(strcmp(command, "delete") == 0) {
        delete(param1);
    } else if(strcmp(command, "search") == 0) {
        uint32_t* result = search(param1);
        if(result != NULL)
            printf("%s,%u\n", param1, *result);
        else
            printf("No Record Found\n");
    } else if(strcmp(command, "print") == 0) {
        print_list();
    }
}

int main() {
    // Initialize lock
    init_rwlock(&list_lock);

    // Read commands from file
    FILE* file = fopen("commands.txt", "r");
    if(file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char command[20], param1[50], param2[20];
    while(fscanf(file, "%s %s %s", command, param1, param2) == 3) {
        process_command(command, param1, param2);
    }

    // Clean up
    fclose(file);
    destroy_rwlock(&list_lock);

    return 0;
}