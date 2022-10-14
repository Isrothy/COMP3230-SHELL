#ifndef C3230_PROC_MANG
#define C3230_PROC_MANG

#include <sys/types.h>

/**
 * @brief to manage chile processes
 * pid: process id
 * cmd: command
 * bg: none-zero if background
 */
struct ProcInfo {
    pid_t pid;
    char *cmd;
    int bg;
};

/**
 * @brief initialize the process manager
 * should be called at the beginning of the program
 *
 */
void proc_mag_init();

/**
 * @brief add a running process
 *
 * @param pid
 * @param cmd
 * @param bg
 */
void proc_add(pid_t pid, char *cmd, int bg);

/**
 * @brief find a process according to the pid
 *
 * @param pid
 * @return struct ProcInfo* NULL if not found
 */
struct ProcInfo *proc_query(pid_t pid);

/**
 * @brief remove a process from the manager
 *
 * @param pid
 */
void proc_del(pid_t pid);

/**
 * @brief release all the resource of process manager
 *
 */
void proc_mag_release();

#endif