#include "../include/builtin_exit.h"
#include "../include/builtin_timex.h"
#include "../include/cmd_parser.h"
#include "../include/isr_linked_list.h"
#include "../include/shell_exe.h"
#include "../include/shell_io.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_sig_int(int signum) {
    printf("\n%s", getPrompt());
    fflush(stdout);
}

int main() {
    signal(SIGINT, handle_sig_int);
    while (1) {
        printf("%s", getPrompt());
        fflush(stdout);
        char *input = readline();
        struct CMDs cmd;
        int pr = parse(input, &cmd);
        if (pr < 0) {
            fprintf(stderr, translate_parse_error(pe));
            fflush(stderr);
        } else {
            exe_cmds(cmds);
        }

        // if (arg_list[0] != NULL) {
        //     if (strcmp(arg_list[0], "exit") == 0) {
        //         builtin_exit(arg_list);
        //     } else if (strcmp(arg_list[0], "timeX") == 0) {
        //         builtin_timex(arg_list + 1);
        //     } else {
        //         exe_external_cmd(arg_list);
        //     }
        // }
        struct ISRLinkedListNode *p = cmds.command_list;
        while (p->next != NULL) {
            p = p->next;
            free(p->value);
        }

        free(input);
    }
}
