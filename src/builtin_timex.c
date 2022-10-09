#include "../include/builtin_exit.h"
#include "../include/shell_exe.h"
#include "../include/shell_io.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void builtin_timex(struct CMDs cmds) {
    struct ISRLinkedListNode *first = cmds.command_list->sentinal->next;
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
            (double) info->user_time / (double) sysconf(_SC_CLK_TCK),
            (double) info->sys_time / (double) sysconf(_SC_CLK_TCK)
        );
    }
    isr_linked_list_free(result, 0);
}