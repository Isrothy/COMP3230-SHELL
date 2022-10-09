#include "../include/builtin_exit.h"
#include "../include/builtin_timex.h"
#include "../include/cmd_parser.h"
#include "../include/proc_mag.h"
#include "../include/shell_exe.h"
#include "../include/shell_io.h"
#include "../include/sig_handler.h"
#include <signal.h>
#include <string.h>

int main() {
    signal(SIGINT, handle_sig_int);
    signal(SIGTSTP, handle_sig_tstp);
    signal(SIGCHLD, handle_sig_chld);
    proc_mag_init();
    while (1) {
        shell_output("%s", getPrompt());
        char *input = readline();
        struct CMDs cmds;
        int pe = parse(input, &cmds);
        if (pe < 0) {
            shell_error("%s\n", translate_parse_error(pe));
        } else if (!isr_linked_list_is_empty(cmds.command_list)) {
            char ***args = (char ***) &cmds.command_list->sentinal->next->value;
            if (strcmp((*args)[0], "exit") == 0) {
                builtin_exit(*args);
            } else if (strcmp((*args)[0], "timeX") == 0) {
                ++(*args);
                builtin_timex(cmds);
                --(*args);
            } else {
                struct ISRLinkedList *result = exe_excmds(cmds);
                if (result != NULL) {
                    isr_linked_list_free(result, 0);
                }
            }
        }
        isr_linked_list_free(cmds.command_list, 1);
    }
}
