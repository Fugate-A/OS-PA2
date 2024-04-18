// chash.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashdb.h"

#define MAX_COMMAND_LEN 50

void process_commands(const char *filename);
void execute_command(const char *command);

int main() {
    process_commands("commands.txt");
    return 0;
}

void process_commands(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char command[MAX_COMMAND_LEN];
    while (fgets(command, MAX_COMMAND_LEN, file) != NULL) {
        // Remove newline character
        command[strcspn(command, "\n")] = 0;
        execute_command(command);
    }

    fclose(file);
}

void execute_command(const char *command) {
    char *token;
    char *command_copy = strdup(command);
    token = strtok(command_copy, ",");
    if (strcmp(token, "threads") == 0) {
        // Parse number of threads (not used in this implementation)
        strtok(NULL, ","); // Skip the next token
        // No need to store num_threads, as it's not used
    } else if (strcmp(token, "insert") == 0) {
        // Parse name and salary
        token = strtok(NULL, ",");
        char *name = token;
        token = strtok(NULL, ",");
        uint32_t salary = atoi(token);
        // Insert or update record
        insert(name, salary);
    } else if (strcmp(token, "delete") == 0) {
        // Parse name
        token = strtok(NULL, ",");
        char *name = token;
        // Delete record
        delete(name);
    } else if (strcmp(token, "search") == 0) {
        // Parse name
        token = strtok(NULL, ",");
        char *name = token;
        // Search for record
        search(name);
    } else if (strcmp(token, "print") == 0) {
        // Print entire contents of the list
        print_list();
    } else {
        fprintf(stderr, "Unknown command: %s\n", token);
    }
    free(command_copy);
}
