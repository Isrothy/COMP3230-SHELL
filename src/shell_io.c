#include "../include/shell_io.h"
#include <memory.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

char const *getPrompt() {
    return "3230shell ## ";
}

char *readline() {
    size_t capacity = 64;
    size_t size = 0;
    char *line = (char *) malloc(capacity * sizeof(char));
    if (line == NULL) {
        return NULL;
    }
    while (1) {
        char c = (char) getchar();
        if (c == '\n') {
            break;
        }
        line[size] = c;
        ++size;
        if (size + 1 == capacity) {
            line = (char *) realloc(line, capacity * 2 * sizeof(char));
            if (line == NULL) {
                return NULL;
            }
            capacity *= 2;
        }
    }
    line[size] = '\0';
    return line;
}