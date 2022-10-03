#ifndef C3230_PROC_MANG
#define C3230_PROC_MANG

#include <sys/types.h>
struct ProcInfo {
    pid_t pid;
    char *cmd;
    int bg;
};

void proc_mag_init();

void proc_add(pid_t pid, char *cmd, int bg);

struct ProcInfo *proc_query(pid_t pid);

void proc_del(pid_t pid);

void proc_mag_free();

#endif