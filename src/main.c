#include "../include/builtin_exit.h"
#include "../include/builtin_timex.h"
#include "../include/cmd_parser.h"
#include "../include/isr_dynamic_array.h"
#include "../include/isr_linked_list.h"
#include "../include/shell_exe.h"
#include "../include/shell_io.h"
#include <assert.h>
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
        struct CMDs cmds;
        int pe = parse(input, &cmds);
        if (pe < 0) {
            fprintf(stderr, "%s\n", translate_parse_error(pe));
            fflush(stderr);
        } else if (!isr_linked_list_is_empty(cmds.command_list)) {
            char ***args = (char ***) &cmds.command_list->sentinal->next->value;
            assert(args != NULL && !isr_dynamic_array_is_empty((void **) *args));
            if (strcmp((*args)[0], "exit") == 0) {
                builtin_exit(*args);
            } else if (strcmp((*args)[0], "timeX") == 0) {
                ++(*args);
                builtin_timex(&cmds);
                --(*args);
            } else {
                exe_cmds(&cmds);
            }
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
        isr_linked_list_free(cmds.command_list, 0);
        free(input);
    }
}
