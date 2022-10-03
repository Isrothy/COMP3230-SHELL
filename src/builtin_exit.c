#include "../include/builtin_exit.h"
#include "../include/proc_mag.h"
#include "../include/shell_io.h"
#include <stdlib.h>

void builtin_exit(char **arg_list) {
    if (arg_list[1] != NULL) {
        shell_output("\"exit\" with other arguments!!!\n");
    } else {
        shell_output("3230shell: Terminated\n");
        proc_mag_free();
        exit(0);
    }
}