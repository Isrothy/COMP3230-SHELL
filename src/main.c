#include "../include/builtin_exit.h"
#include "../include/builtin_timex.h"
#include "../include/cmd_parser.h"
#include "../include/isr_dynamic_array.h"
#include "../include/isr_linked_list.h"
#include "../include/proc_mag.h"
#include "../include/shell_exe.h"
#include "../include/shell_io.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void handle_sig_int(int signum) {
    shell_output("\n%s", getPrompt());
}

void handle_sig_chld(int signum) {
    sigset_t set, oset;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, &oset);

    int stat;
    pid_t pid = waitpid(0, &stat, WNOHANG);
    if (pid <= 0) {
        return;
    }
    struct ProcInfo *info = proc_query(pid);
    if (info == NULL) {
        return;
    }
    if (!info->bg) {
        shell_error("What the f**k? This should run in background\n");
    }
    shell_output("\n");
    bgchild_notify(stat, info);
    shell_output("%s", getPrompt());
    proc_del(pid);
    sigprocmask(SIG_BLOCK, &oset, NULL);
}

int main() {
    signal(SIGINT, handle_sig_int);
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
                exe_excmds(cmds);
            }
        }
        isr_linked_list_free(cmds.command_list, 0);
        free(input);
    }
}
