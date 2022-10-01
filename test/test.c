#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

const char *translate_exec_error_message() {
    switch (errno) {
        case EACCES: {
            return "ERROR: PERMISSION DENIED";
        }
        case EFAULT: {
            return "ERROR: FILENAME POINTS OUTSIDE YOUR ACCESSIBLE ADDRESS SPACE";
        }
        case EINVAL: {
            return "ERROR TRIED TO NAME MORE THAN ONE INTERPRETER";
        }
        case EIO: {
            return "ERROR: I/O ERROR";
        }
        case EISDIR: {
            return "ERROR: AN ELF INTERPRETER WAS A DIRECTORY";
        }
        case ELIBBAD: {
            return "ERROR: AN ELF INTERPRETER WAS NOT IN A RECOGNIZED FORMAT";
        }
        case ELOOP: {
            return "ERROR: TOO MANY SYMBOLIC LINKS WERE ENCOUNTERED IN RESOLVING FILENAME"
                   " OR THE NAME OF A SCRIPT OR ELF INTERPRETER";
        }
        case EMFILE: {
            return "ERROR: THE PROCESS HAS THE MAXIMUM NUMBER OF FILES OPEN";
        }
        case ENAMETOOLONG: {
            return "ERROR: FILENAME IS TOO LONG";
        }
        case ENFILE: {
            return "ERROR: THE SYSTEM LIMIT ON THE TOTAL NUMBER of OPEN FILES HAS BEEN REACHED";
        }
        case ENOENT: {
            return "ERROR: THE FILE FILENAME OR A SCRIPT OR ELF INTERPRETER DOES NOT EXIST,"
                   " OR A SHARED LIBRARY NEEDED FOR FILE OR INTERPRETER CANNOT BE FOUND";
        }
        case ENOEXEC: {
            return "ERROR: AN EXECUTABLE IS NOT IN A RECOGNIZED FORMAT, "
                   "IS FOR THE WRONG ARCHITECTURE, "
                   "OR HAS SOME OTHER FORMAT ERROR THAT MEANS IT CANNOT BE EXECUTED";
        }
        case ENOMEM: {
            return "ERROR: INSUFFICIENT KERNEL MEMORY";
        }
        case ENOTDIR: {
            return "ERROR: A COMPONENT OF THE PATH PREFIX OF FILENAME OR A SCRIPT "
                   "OR ELF INTERPRETER IS NOT A DIRECTORY";
        }
        case EPERM: {
            return "ERROR: The FILE SYSTEM IS MOUNTED NOSUID OR THE PROCESS IS BEING TRACED, "
                   "THE USER IS NOT THE SUPERUSER, "
                   "AND THE FILE HAS THE SET-USER-ID OR SET-GROUP-ID BIT SET";
        }
        case ETXTBSY: {
            return "ERROR: EXECUTABLE was OPEN FOR WRITING BY ONE OR MORE PROCESSES.";
        }
        default: {
            return "UNKNOWN ERROR";
        }
    }
}


void sig_handler_parent(int signum) {
    printf("Parent : Received a signal.\n");
    exit(0);
}

void sig_handler_child(int signum) {
    printf("Child : Received a signal.\n");
    exit(0);
}

int main() {
    // char *args[] = {"no", "-l", NULL};
    // int stat = execvp(args[0], (char *const *) args);
    // fprintf(stderr, "%s\n", translate_exec_error_message());
    // printf("%d\n", stat);


    // struct tms st_cpu;
    // times(&st_cpu);
    // int cpid = fork();
    // if (cpid == 0) {
    //     long long x = 0;
    //     for (int i = 0; i < 1000000000; ++i) {
    //         x += i;
    //     }
    //     printf("%lld\n", x);
    //     exit(0);
    // } else {
    //     int stat;
    //     waitpid(cpid, &stat, 0);
    //     struct tms ed_cpu;
    //     times(&ed_cpu);
    //     printf(
    //         "%ld %ld %ld\n",
    //         ed_cpu.tms_cstime - st_cpu.tms_cstime,
    //         ed_cpu.tms_cutime - st_cpu.tms_cutime,
    //         sysconf(_SC_CLK_TCK)
    //     );
    // }

    pid_t cpid = fork();
    if (cpid == 0) {
        signal(SIGINT, sig_handler_child);
        while (1)
            ;
    } else {
        signal(SIGINT, sig_handler_parent);
        while (1)
            ;
    }

    return 0;
}