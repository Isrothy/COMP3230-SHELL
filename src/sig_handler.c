#include "../include/sig_handler.h"
#include "../include/proc_mag.h"
#include "../include/shell_exe.h"
#include "../include/shell_io.h"
#include <bits/types/sigset_t.h>
#include <signal.h>
#include <sys/wait.h>

void handle_sig_int(int signum) {
    shell_output("\n%s", getPrompt());
}

void handle_sig_tstp(int signum) {
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