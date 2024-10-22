#include "../include/shell_exe.h"
#include "../include/shell_io.h"
#include <assert.h>
#include <stdint.h>
#include <unistd.h>

void builtin_timex(const struct CMDs cmds) {
    struct ISRLinkedListNode *first = cmds.command_list->sentinel->next;
    if (first == NULL || first->value == NULL || ((char **) (first->value))[0] == NULL) {
        shell_error("3230shell: \" timeX \" cannot be a standalone command\n");
        return;
    }
    if (cmds.background) {
        shell_error("3230shell: \"timeX\" cannot be run in background mode\n");
        return;
    }
    struct ISRLinkedList *result = exe_excmds(cmds);
    if (result == NULL) {
        return;
    }
    ISRLinkedListForEach(p, result) {
        struct ExeRet *info = (struct ExeRet *) p->value;
        assert(info != NULL);
        shell_output(
            "(PID)%jd  (CMD)%s    (usr)%.3f s  (sys)%.3f s\n",
            (intmax_t) info->pid,
            info->cmd,
            (double) info->usage.ru_utime.tv_sec + (double) info->usage.ru_utime.tv_usec * 1e-6,
            (double) info->usage.ru_stime.tv_sec + (double) info->usage.ru_stime.tv_usec * 1e-6
        );
    }
    isr_linked_list_free(result, 1);
}