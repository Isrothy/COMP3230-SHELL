#include "../include/shell_exe.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
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

int exe_an_excmd(char **arg_list, int in_file, int out_file, struct ProcInfo *info) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);

    pid_t child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "ERROR: FORK FAILED");
        fflush(stderr);
        return -1;
    }

    if (child_pid == 0) {
        signal(SIGINT, SIG_DFL);

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
            fprintf(stderr, "Invalid or unsupported signal\n");
            fflush(stderr);
            exit(0);
        } else if (sig != SIGUSR1) {
            fprintf(stderr, "Caught signal %d\n", sig);
            fflush(stderr);
            exit(0);
        }
        int exe_ret = execvp(arg_list[0], arg_list);
        if (exe_ret < 0) {
            fprintf(stderr, "'%s': %s\n", arg_list[0], translate_exec_error_message());
            fflush(stderr);
        }

        exit(0);
    } else {
        signal(SIGINT, SIG_IGN);

        if (in_file != 0) {
            close(in_file);
        }
        if (out_file != 1) {
            close(out_file);
        }

        struct tms st_cpu;
        times(&st_cpu);

        kill(child_pid, SIGUSR1);

        int stat;
        pid_t ret = waitpid(child_pid, &stat, 0);

        struct tms ed_cpu;
        times(&ed_cpu);

        fflush(stderr);
        fflush(stdout);

        *info = (struct ProcInfo){
            child_pid,
            arg_list[0],
            ed_cpu.tms_cutime - st_cpu.tms_cutime,
            ed_cpu.tms_cstime - st_cpu.tms_cstime,
        };


        if (ret == -1) {
            fprintf(stderr, "ERROR: WAITING FOR THE CHILD PROCESS. ERRNO: %d\n", errno);
            fflush(stderr);
            return 0;
        }
        if (!WIFEXITED(stat)) {
            if (WIFSIGNALED(stat)) {
                printf("'%s': killed by signal %d\n", arg_list[0], WTERMSIG(stat));
                fflush(stdout);
            } else if (WIFSTOPPED(stat)) {
                printf("'%s': stopped by signal %d\n", arg_list[0], WSTOPSIG(stat));
                fflush(stdout);
            }
        }
        return 0;
    }
}


struct ISRLinkedList *exe_excmds(struct CMDs cmds) {
    struct ISRLinkedList *results = new_isr_linked_list();
    int in = 0, out = 1;
    int pipes[2];
    for (struct ISRLinkedListNode *p = cmds.command_list->sentinal->next; p != NULL; p = p->next) {
        if (p->next != NULL) {
            int r = pipe(pipes);
            if (r < 0) {
                fprintf(stderr, "Pipe err\n");
                fflush(stderr);
                isr_linked_list_free(results, 1);
                return NULL;
            }
            out = pipes[1];
        } else {
            out = 1;
        }
        struct ProcInfo *info = (struct ProcInfo *) malloc(sizeof(struct ProcInfo));
        int re = exe_an_excmd((char **) p->value, in, out, info);
        if (re < 0) {
            isr_linked_list_free(results, 1);
            return NULL;
        }
        isr_linked_list_insert_tail(results, info);
        if (p->next != NULL) {
            in = pipes[0];
        }
    }
    return results;
}