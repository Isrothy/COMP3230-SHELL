#include "../include/builtin_exit.h"
#include "../include/shell_exe.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void builtin_timex(char **arg_list) {
    if (arg_list[0] == NULL) {
        fprintf(stderr, "3230shell: \" timeX \" cannot be a standalone command");
    }
    struct ProcInfo *result = exe_external_cmd(arg_list);
    if (result != NULL) {
        printf(
            "(PID)%jd  (CMD)%s    (usr)%.3f s  (sys)%.3f s\n",
            (intmax_t) result->pid,
            result->cmd,
            (double) result->user_time / (double) sysconf(_SC_CLK_TCK),
            (double) result->sys_time / (double) sysconf(_SC_CLK_TCK)
        );
        free(result);
    }
}