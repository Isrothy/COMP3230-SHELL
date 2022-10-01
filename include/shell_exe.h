#ifndef C3230_SHELL_EXE
#define C3230_SHELL_EXE

#include "cmd_parser.h"
#include "isr_linked_list.h"
#include <sys/types.h>

struct ProcInfo {
    pid_t pid;
    char *cmd;
    clock_t user_time;
    clock_t sys_time;
};

struct ProcInfo *exe_external_cmd(char **arg_list, int pipe_in, int pipe_out);

struct ISRLinkedList *exe_cmds(struct CMDs *cmds);

#endif