#ifndef C3230_SHELL_EXE
#define C3230_SHELL_EXE

#include <sys/types.h>

struct ProcInfo {
    pid_t pid;
    char *cmd;
    clock_t user_time;
    clock_t sys_time;
};

struct ProcInfo *exe_external_cmd(char **arg_list);

#endif