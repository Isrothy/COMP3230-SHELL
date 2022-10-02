#ifndef C3230_CMD_PARSER
#define C3230_CMD_PARSER

#include "isr_linked_list.h"

#define PE_BG_IN_THE_MID (-2)
#define PE_MULTI_BG (-3)
#define PE_PIPE_EMPTY_CMD (-4)

struct CMDs {
    int background;
    struct ISRLinkedList *command_list;
};

const char *translate_parse_error(int pe);

int parse(char *str, struct CMDs *cmds);

#endif