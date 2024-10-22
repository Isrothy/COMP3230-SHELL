#include "../include/proc_mag.h"
#include "../include/isr_hash_table.h"
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int proc_id(void *k1, void *k2) {
    return *((pid_t *) k1) == *((pid_t *) k2);
}

size_t proc_hash(void *k) {
    size_t K = *((pid_t *) k);
    return (K << 56) ^ (K << 48) ^ (K << 40) ^ (K << 32) ^ (K << 24) ^ (K << 16) ^ (K << 8) ^ K;
}

struct ISRHashTable *ptb;

void proc_mag_init() {
    ptb = isr_hash_table_new(proc_hash, proc_id);
}

void proc_add(const pid_t pid, char *cmd, const int bg) {
    pid_t *key = (pid_t *) malloc(sizeof(pid_t));
    *key = pid;
    char *new_cmd = (char *) malloc((strlen(cmd) + 1) * sizeof(char));
    strcpy(new_cmd, cmd);
    struct ProcInfo *info = (struct ProcInfo *) malloc(sizeof(struct ProcInfo));
    *info = (struct ProcInfo){pid, new_cmd, bg};
    isr_hash_table_insert(ptb, key, info);
}

struct ProcInfo *proc_query(pid_t pid) {
    return (struct ProcInfo *) isr_hash_table_find(ptb, &pid);
}

void proc_del(pid_t pid) {
    struct ISRHashTableEntity *e = isr_hash_table_remove(ptb, &pid);
    struct ProcInfo *info = (struct ProcInfo *) e->value;
    free(info->cmd);
    free(e);
}

void proc_mag_release() {
    ISRHashTableForEach(p, ptb) {
        struct ISRHashTableEntity *e = (struct ISRHashTableEntity *) p->value;
        struct ProcInfo *info = (struct ProcInfo *) e->value;
        kill(info->pid, SIGKILL);
    }
    isr_hash_table_free(ptb);
}
