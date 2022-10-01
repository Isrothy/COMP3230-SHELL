#include "../include/builtin_exit.h"
#include "../include/builtin_timex.h"
#include "../include/shell_exe.h"
#include "../include/shell_io.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_sig_int(int signum) {
    printf("\n%s", getPrompt());
    fflush(stdout);
}

int main() {
    signal(SIGINT, handle_sig_int);
    while (1) {
        printf("%s", getPrompt());
        fflush(stdout);
        char *input = readline();
        size_t capacity = 8;
        size_t size = 0;
        char **arg_list = (char **) malloc(capacity * sizeof(char *));
        const char *delim = " ";
        char *token = strtok(input, delim);
        while (token != NULL) {
            arg_list[size] = token;
            ++size;
            if (size + 1 == capacity) {
                arg_list = (char **) realloc(arg_list, capacity * 2 * sizeof(char *));
                capacity *= 2;
            }
            token = strtok(NULL, delim);
        }
        arg_list[size] = NULL;
        if (arg_list[0] != NULL) {
            if (strcmp(arg_list[0], "exit") == 0) {
                builtin_exit(arg_list);
            } else if (strcmp(arg_list[0], "timeX") == 0) {
                builtin_timex(arg_list + 1);
            } else {
                shell_exe(arg_list);
            }
        }
        free(arg_list);
        free(input);
    }
}
