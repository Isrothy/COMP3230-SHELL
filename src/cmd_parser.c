#include "../include/cmd_parser.h"
#include "../include/isr_dynamic_array.h"
#include "../include/isr_linked_list.h"
#include <ctype.h>
#include <string.h>

const char *translate_parse_error(int pe) {
    if (pe >= 0) {
        return "";
    }
    switch (pe) {
        case PE_BG_IN_THE_MID: {
            return "'&' should not appear in the middle of the command line";
        }
        case PE_MULTI_BG: {
            return "'&' should only appear once";
        }
        case PE_ST_PIPE: {
            return "Should not start with '|'";
        }
        case PE_ED_PIPE: {
            return "Should not end with '|'";
        }
        case PE_CONS_PIPE: {
            return "Should not have two consecutive | without in-between command";
        }
        default: {
            return "Unkown error";
        }
    }
}

int parse(char *str, struct CMDs *cmds) {
    cmds->background = 0;
    cmds->command_list = isr_linked_list_new();
    while (*str != '\0' && isspace(*str)) {
        ++str;
    }
    if (*str == '\0') {
        return 0;
    }

    size_t capacity = 8;
    size_t size = 0;
    char **args;
    isr_dynamic_array_init((void ***) (&args), &capacity, &size);
    int has_bg = 0;
    int in_word = 0;
    int in_pipe = 0;
    int first = 1;
    while (*str != '\0') {
        if (isspace(*str)) {
            *str = '\0';
            in_word = 0;
        } else if (*str == '|') {
            if (has_bg) {
                return PE_BG_IN_THE_MID;
            }
            if (in_pipe) {
                return PE_CONS_PIPE;
            }
            if (first) {
                return PE_ST_PIPE;
            }
            *str = '\0';
            isr_linked_list_insert_tail(cmds->command_list, args);
            isr_dynamic_array_init((void ***) (&args), &capacity, &size);
            first = 0;
            in_word = 0;
            in_pipe = 1;
        } else if (*str == '&') {
            if (has_bg) {
                return PE_MULTI_BG;
            }
            if (in_pipe) {
                return PE_ED_PIPE;
            }
            cmds->background = 1;
            if (!isr_dynamic_array_is_empty((void **) args)) {
                isr_linked_list_insert_tail(cmds->command_list, args);
                isr_dynamic_array_init((void ***) (&args), &capacity, &size);
            }
            has_bg = 1;
            first = 0;
            in_word = 0;
            in_pipe = 0;
            *str = '\0';
        } else {
            if (has_bg) {
                return PE_BG_IN_THE_MID;
            }
            if (!in_word) {
                isr_dynamic_array_push_back((void ***) &args, (void *) str, &capacity, &size);
            }
            first = 0;
            in_word = 1;
            in_pipe = 0;
        }
        ++str;
    }
    if (in_pipe) {
        return PE_ED_PIPE;
    }
    if (!isr_dynamic_array_is_empty((void **) args)) {
        isr_linked_list_insert_tail(cmds->command_list, args);
    }
    return 0;
}