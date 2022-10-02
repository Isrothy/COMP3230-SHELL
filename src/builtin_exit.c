#include "../include/builtin_exit.h"
#include "../include/shell_io.h"
#include <stdio.h>
#include <stdlib.h>

void builtin_exit(char **arg_list) {
    if (arg_list[1] != NULL) {
        shell_output("\"exit\" with other arguments!!!");
    } else {
        shell_output("3230shell: Terminated");
        exit(0);
    }
}