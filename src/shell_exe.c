#include "../include/shell_exe.h"
#include "../include/proc_mag.h"
#include "../include/shell_io.h"
#include <bits/types/sigset_t.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
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
            return "Command not found";
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

int exe_an_excmd(
    char **arg_list, int in_file, int out_file, int background, struct ExeRet *exe_ret
) {
    sigset_t set, oset;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, &oset);

    pid_t child_pid = fork();

    if (child_pid < 0) {
        shell_error("ERROR: FORK FAILED");
        return -1;
    }

    if (child_pid == 0) {
        if (!background) {
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
        }
        signal(SIGCHLD, SIG_DFL);

        if (in_file != 0) {
            dup2(in_file, 0);
            close(in_file);
        }
        if (out_file != 1) {
            dup2(out_file, 1);
            close(out_file);
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
    } else {
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

        struct tms st_cpu;
        times(&st_cpu);

        kill(child_pid, SIGUSR1);

        if (background) {
            sigprocmask(SIG_SETMASK, &oset, NULL);
            return 0;
        }

        while (1) {
            int stat;
            pid_t pid = waitpid(0, &stat, 0);
            struct ProcInfo *info = proc_query(pid);
            if (info == NULL) {
                shell_error(
                    "What the f**k? I can find this pid in running processes. PID: %d\n", pid
                );
            }
            if (pid == child_pid) {
                struct tms ed_cpu;
                times(&ed_cpu);

                proc_del(pid);

                *exe_ret = (struct ExeRet){
                    child_pid,
                    arg_list[0],
                    ed_cpu.tms_cutime - st_cpu.tms_cutime,
                    ed_cpu.tms_cstime - st_cpu.tms_cstime,
                };


                if (pid == -1) {
                    shell_error("Error: waiting for the child process. errno: %d\n", errno);
                    sigprocmask(SIG_SETMASK, &oset, NULL);
                    return 0;
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
                sigprocmask(SIG_SETMASK, &oset, NULL);
                return 0;
            } else {
                bgchild_notify(stat, info);
                proc_del(pid);
            }
        }
    }
}


struct ISRLinkedList *exe_excmds(struct CMDs cmds) {
    struct ISRLinkedList *results = isr_linked_list_new();
    int in = 0, out = 1;
    int pipes[2];
    ISRLinkedListForEach(p, cmds.command_list) {
        if (p->next != cmds.command_list->sentinal) {
            int r = pipe(pipes);
            if (r < 0) {
                shell_error("Pipe err\n");
                isr_linked_list_free(results, 1);
                return NULL;
            }
            out = pipes[1];
        } else {
            out = 1;
        }
        struct ExeRet *exe_ret = (struct ExeRet *) malloc(sizeof(struct ExeRet));
        int re = exe_an_excmd((char **) p->value, in, out, cmds.background, exe_ret);
        if (re < 0) {
            isr_linked_list_free(results, 1);
            return NULL;
        }
        isr_linked_list_insert_tail(results, exe_ret);
        if (p->next != cmds.command_list->sentinal) {
            in = pipes[0];
        }
    }
    return results;
}

void bgchild_notify(int stat, struct ProcInfo *info) {
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