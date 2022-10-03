#ifndef C3230_SHELL_EXE
#define C3230_SHELL_EXE

#include "cmd_parser.h"
#include "isr_linked_list.h"
#include "proc_mag.h"
#include <sys/types.h>

struct ExeRet {
    pid_t pid;
    char *cmd;
    clock_t user_time;
    clock_t sys_time;
};

int exe_an_excmd(char **arg_list, int in_file, int out_file, int background, struct ExeRet *info);

struct ISRLinkedList *exe_excmds(struct CMDs cmds);

void bgchild_notify(int stat, struct ProcInfo *info);

#endif