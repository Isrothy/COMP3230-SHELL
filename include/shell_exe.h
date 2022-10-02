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

int exe_an_excmd(char **arg_list, int in_file, int out_file, int background, struct ProcInfo *info);

struct ISRLinkedList *exe_excmds(struct CMDs cmds);

#endif