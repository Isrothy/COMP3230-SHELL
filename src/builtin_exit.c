#include "../include/builtin_exit.h"
#include <stdio.h>
#include <stdlib.h>

void builtin_exit(char **arg_list) {
    if (arg_list[1] != NULL) {
        puts("\"exit\" with other arguments!!!");
        fflush(stdout);
    } else {
        puts("3230shell: Terminated");
        fflush(stdout);
        exit(0);
    }
}