#include "../include/cmd_parser.h"
#include "../include/shell_exe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    struct CMDs cmds;
    char *input = (char *) malloc(100 * sizeof(char));
    strcpy(input, "ls -la | wc | tr -s \" \" | cut -f2");
    int pe = parse(input, &cmds);
    if (pe < 0) {
        puts(translate_parse_error(pe));
    } else {
        // int pipes[2];
        // int re = pipe(pipes);
        // if (re < 0) {
        //     puts("pipe error");
        // } else {
        //     struct ISRLinkedListNode *p = isr_linked_list_head(cmds.command_list);
        //     struct ISRLinkedListNode *q = p->next;
        // }
        struct ISRLinkedList *res = exe_excmds(cmds);
        for (struct ISRLinkedListNode *p = res->sentinal->next; p != NULL; p = p->next) {
            struct ProcInfo info = *((struct ProcInfo *) (p->value));
            printf("%s %d %ld %ld\n", info.cmd, info.pid, info.sys_time, info.user_time);
        }
    }

    return 0;
}