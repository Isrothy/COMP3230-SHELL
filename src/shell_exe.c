#include "../include/shell_exe.h"
#include "../include/proc_mag.h"
#include "../include/shell_io.h"
#include "../include/sig_handler.h"
#include <bits/types/sigset_t.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>


const char *translate_exec_error_message() {
    switch (errno) {
        case EACCES: {
            return "Permission denied";
        }
        case EFAULT: {
            return "Filename points outside your accessible address space";
        }
        case EINVAL: {
            return "Tried to name more than one interpreter";
        }
        case EIO: {
            return "I/O error";
        }
        case EISDIR: {
            return "An ELF interpreter was a directory";
        }
        case ELIBBAD: {
            return "An ELF interpreter was not in a recognized format";
        }
        case ELOOP: {
            return "Too many symbolic links were encountered in resolving filename"
                   " or the name of a script or ELF interpreter";
        }
        case EMFILE: {
            return "The process has the maximum number of files open";
        }
        case ENAMETOOLONG: {
            return "Filename is too long";
        }
        case ENFILE: {
            return "The system limit on the total number of open files has been reached";
        }
        case ENOENT: {
            return "No such file or directory";
        }
        case ENOEXEC: {
            return "An executable is not in a recognized format, "
                   "is for the wrong architecture, "
                   "or has some other format error that means it cannot be executed";
        }
        case ENOMEM: {
            return "Insufficient kernel memory";
        }
        case ENOTDIR: {
            return "A component of the path prefix of filename or a script "
                   "or elf interpreter is not a directory";
        }
        case EPERM: {
            return "The file system is mounted nosuid or the process is being traced, "
                   "the user is not the superuser, "
                   "and the file has the set-user-id or set-group-id bit set";
        }
        case ETXTBSY: {
            return "Executable was open for writing by one or more processes.";
        }
        default: {
            return "Unknown error";
        }
    }
}

void sig_pause(int signum) {
    pause();
}

void exe_child(
    char **arg_list,
    const int in_file,
    const int out_file,
    const int background,
    pid_t *pgid,
    const sigset_t set
) {
    if (background) {
        signal(SIGTTIN, sig_pause);
    } else {
        signal(SIGTSTP, SIG_DFL);
    }
    signal(SIGCHLD, SIG_DFL);
    signal(SIGINT, SIG_DFL);

    if (in_file != 0) {
        dup2(in_file, 0);
        close(in_file);
    }
    if (out_file != 1) {
        dup2(out_file, 1);
        close(out_file);
    }

    if (background) {
        if (*pgid == 0) {
            setpgid(0, 0);
            *pgid = getpgid(0);
        } else {
            setpgid(0, *pgid);
        }
    }

    int sig;
    int sig_ret = sigwait(&set, &sig);
    if (sig_ret == -1) {
        shell_error("Invalid or unsupported signal\n");
        exit(0);
    } else if (sig != SIGUSR1) {
        shell_error("Caught signal %d\n", sig);
        exit(0);
    }
    int res = execvp(arg_list[0], arg_list);
    if (res < 0) {
        shell_error("'%s': %s\n", arg_list[0], translate_exec_error_message());
    }
    exit(0);
}

int exe_parent(
    char **arg_list,
    const int in_file,
    const int out_file,
    const int background,
    struct ExeRet *exe_ret,
    const sigset_t oset,
    const pid_t child_pid
) {
    signal(SIGINT, SIG_IGN);
    if (!sigismember(&oset, SIGUSR1)) {
        sigset_t s;
        sigaddset(&s, SIGUSR1);
        sigprocmask(SIG_UNBLOCK, &s, NULL);
    }

    if (in_file != 0) {
        close(in_file);
    }
    if (out_file != 1) {
        close(out_file);
    }

    proc_add(child_pid, arg_list[0], background);

    kill(child_pid, SIGUSR1);

    if (background) {
        sigprocmask(SIG_SETMASK, &oset, NULL);
        signal(SIGINT, handle_sig_int);
        return 0;
    }

    while (1) {
        int stat;
        struct rusage rusage;
        pid_t pid = wait4(-1, &stat, 0, &rusage);
        struct ProcInfo *info = proc_query(pid);
        if (info == NULL) {
            shell_error(
                "What the f**k? I cannot find this pid in running processes. PID: %d\n", pid
            );
        }
        if (pid == child_pid) {
            proc_del(pid);

            *exe_ret = (struct ExeRet){
                child_pid,
                arg_list[0],
                rusage,
            };


            if (pid == -1) {
                shell_error("Error: waiting for the child process. errno: %d\n", errno);
                break;
            }
            if (!WIFEXITED(stat)) {
                if (WIFSIGNALED(stat)) {
                    switch (WTERMSIG(stat)) {
                        case SIGINT: {
                            shell_output(
                                "'%s': Interrupt by signal %d\n", arg_list[0], WTERMSIG(stat)
                            );
                            break;
                        }
                        case SIGKILL: {
                            shell_output(
                                "'%s': Killed by signal %d\n", arg_list[0], WTERMSIG(stat)
                            );
                            break;
                        }
                        default: {
                            shell_output(
                                "'%s': Terminated by signal %d\n", arg_list[0], WTERMSIG(stat)
                            );
                            break;
                        }
                    }
                } else if (WIFSTOPPED(stat)) {
                    shell_output("'%s': Stopped by signal %d\n", arg_list[0], WSTOPSIG(stat));
                }
            }
            break;
        } else {
            bgchild_notify(stat, info);
            proc_del(pid);
        }
    }
    sigprocmask(SIG_SETMASK, &oset, NULL);
    signal(SIGINT, handle_sig_int);
    return 0;
}

int exe_an_excmd(
    char **arg_list,
    const int in_file,
    const int out_file,
    const int background,
    pid_t *pgid,
    struct ExeRet *exe_ret
) {
    sigset_t set, oset;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, &oset);

    pid_t child_pid = fork();

    if (child_pid < 0) {
        shell_error("Error: fork failed");
        return -1;
    }

    if (child_pid == 0) {
        exe_child(arg_list, in_file, out_file, background, pgid, set);
    } else {
        return exe_parent(arg_list, in_file, out_file, background, exe_ret, oset, child_pid);
    }
    shell_error("What the f**k? You are not supposed to reach here");
    return -1;
}


struct ISRLinkedList *exe_excmds(const struct CMDs cmds) {
    struct ISRLinkedList *results = isr_linked_list_new();
    int in = 0, out = 1;
    int pipes[2];
    pid_t pgid = 0;
    ISRLinkedListForEach(p, cmds.command_list) {
        if (!isr_linked_list_is_tail(cmds.command_list, p)) {
            int r = pipe(pipes);
            if (r < 0) {
                shell_error("Pipe error\n");
                isr_linked_list_free(results, 1);
                return NULL;
            }
            out = pipes[1];
        } else {
            out = 1;
        }
        struct ExeRet *exe_ret = (struct ExeRet *) malloc(sizeof(struct ExeRet));
        int re = exe_an_excmd((char **) p->value, in, out, cmds.background, &pgid, exe_ret);
        if (re < 0) {
            isr_linked_list_free(results, 1);
            return NULL;
        }
        isr_linked_list_insert_tail(results, exe_ret);
        if (!isr_linked_list_is_tail(cmds.command_list, p)) {
            in = pipes[0];
        }
    }
    return results;
}

void bgchild_notify(const int stat, struct ProcInfo *info) {
    if (info == NULL) {
        shell_error("What the f**k? info is NULL\n");
    }
    if (WIFEXITED(stat)) {
        shell_output("[%d] %s Done\n", info->pid, info->cmd);
    } else if (WIFSIGNALED(stat)) {
        shell_output("[%d] %s Terminated\n", info->pid, info->cmd);
    } else if (WIFSTOPPED(stat)) {
        shell_output("[%d] %s Stopped\n", info->pid, info->cmd);
    }
}