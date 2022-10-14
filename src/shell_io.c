#include "../include/shell_io.h"
#include "../include/proc_mag.h"
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

const char *getPrompt() {
    return "3230shell ## ";
}

const char *getErrorPrefix() {
    return "3230shell: ";
}

char *readline() {
    size_t capacity = 64;
    size_t size = 0;
    char *line = (char *) malloc(capacity * sizeof(char));
    if (line == NULL) {
        return NULL;
    }
    while (1) {
        int c = getchar();
        if (c == EOF) {
            signal(SIGCHLD, SIG_DFL);
            proc_mag_release();
            shell_output("\n");
            exit(0);
        }
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

void shell_output(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    fflush(stdout);
}

void shell_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s", getErrorPrefix());
    vfprintf(stderr, format, args);
    va_end(args);
    fflush(stderr);
}