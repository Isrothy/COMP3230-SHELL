#include "../include/proc_mag.h"
#include "../include/isr_hash_table.h"
#include "../include/shell_io.h"
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

void proc_add(pid_t pid, char *cmd, int bg) {

    shell_error("***%d*** inseted\n", pid);


    pid_t *key = malloc(sizeof(pid_t));
    *key = pid;
    shell_error("%d\n", *((pid_t *) (key)));
    char *new_cmd = malloc(strlen(cmd) * sizeof(char));
    strcpy(new_cmd, cmd);
    struct ProcInfo *info = malloc(sizeof(struct ProcInfo));
    *info = (struct ProcInfo){pid, new_cmd, bg};
    isr_hash_table_insert(ptb, key, info);
}

struct ProcInfo *proc_query(pid_t pid) {
    shell_error("***%d*** queried\n", pid);
    return isr_hash_table_find(ptb, &pid);
}

void proc_del(pid_t pid) {
    shell_error("***%d*** removed\n", pid);
    isr_hash_table_remove(ptb, &pid);
}

void proc_mag_free() {
    isr_hash_table_free(ptb);
}