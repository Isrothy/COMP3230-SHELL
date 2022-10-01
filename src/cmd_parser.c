#include "../include/cmd_parser.h"
#include "../include/isr_dynamic_array.h"
#include "../include/isr_linked_list.h"
#include <ctype.h>

const char *translate_parse_error(int pe) {
    if (pe >= 0) {
        return "";
    }
    switch (pe) {
        case PE_START_WITH_PIPE: {
            return "Command should not start with pipe";
        }
        case PE_BG_IN_THE_MID: {
            return "'&' should not appear in the middle of the command line";
        }
        case PE_MULTI_BG: {
            return "'&' should only appear once";
        }
        case PE_NO_CMD_AFTER_PIPE: {
            return "No command after a pip";
        }
        default: {
            return "Unkown error";
        }
    }
}

int parse(char *str, struct CMDs *cmds) {
    cmds->background = 0;
    cmds->command_list = new_isr_linked_list();
    while (*str != '\0' && isspace(*str)) {
        ++str;
    }

    size_t capacity = 8;
    size_t size = 0;
    char **args;
    isr_dynamic_array_init((void ***) (&args), capacity);

    int has_bg = 0;
    int in_word = 0;
    int in_pip = 0;
    while (str != NULL) {
        if (isspace(*str)) {
            *str = '\0';
            in_word = 0;
            in_pip = 0;
        } else if (*str == '|') {
            in_word = 0;
            if (has_bg) {
                return PE_BG_IN_THE_MID;
            }
            if (isr_linked_list_is_empty(cmds->command_list)) {
                return PE_START_WITH_PIPE;
            }
            if (isr_dynamic_array_is_empty((void **) args)) {
                return PE_NO_CMD_AFTER_PIPE;
            }
            *str = '\0';
            isr_linked_list_insert_tail(cmds->command_list, args);
            in_pip = 1;
        } else if (*str == '&') {
            if (has_bg) {
                return PE_MULTI_BG;
            }
            has_bg = 1;
            in_word = 0;
            in_pip = 0;
            *str = '\0';
        } else {
            if (has_bg) {
                return PE_BG_IN_THE_MID;
            }
            if (!in_word) {
                isr_dynamic_array_push_back((void ***) &args, (void *) str, &capacity, &size);
                in_word = 1;
            }
            in_pip = 0;
        }

        ++str;
    }
    if (in_pip) {
        return PE_NO_CMD_AFTER_PIPE;
    }
    return 0;
}