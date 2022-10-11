#ifndef C3230_SHELL_EXE
#define C3230_SHELL_EXE

#include "cmd_parser.h"
#include "isr_linked_list.h"
#include "proc_mag.h"
#include <bits/types/struct_rusage.h>
#include <sys/types.h>

/**
 * @brief retur result of executing a command
 * pid: the process id
 * cmd: the command
 * usage: resorce usage
 */
struct ExeRet {
    pid_t pid;
    char *cmd;
    struct rusage usage;
};

/**
 * @brief execute a command
 *
 * @param arg_list a list of all arguments with arg_list[0] is the command
 * @param in_file input file number. 0 is stdin
 * @param out_file output file number. 1 is stdout
 * @param background non-zero if run in the background. zero if foreground
 * @param exe_ret the result of executing. exe_ret should not be NULL
 * @return int 0 if executed normally.
 */
int exe_an_excmd(
    char **arg_list, int in_file, int out_file, int background, struct ExeRet *exe_ret
);

/**
 * @brief execute commands linked by pipes
 *
 * @param cmds the commands linked by pipes
 * @return struct ISRLinkedList* a list of all the results. The result type should be (struct ExeRet *). return NULL if error
 */
struct ISRLinkedList *exe_excmds(struct CMDs cmds);

/**
 * @brief notify if background process ends
 *
 * @param stat status of waiting
 * @param info process information. Which is managed by proc_mag.h
 */
void bgchild_notify(int stat, struct ProcInfo *info);

#endif