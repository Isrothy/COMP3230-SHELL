#include "../include/builtin_exit.h"
#include "../include/proc_mag.h"
#include "../include/shell_io.h"
#include <signal.h>
#include <stdlib.h>

void builtin_exit(char **arg_list) {
    if (arg_list[1] != NULL) {
        shell_output("\"exit\" with other arguments!!!\n");
    } else {
        signal(SIGCHLD, SIG_DFL);
        proc_mag_release();
        shell_output("3230shell: Terminated\n");
        exit(0);
    }
}