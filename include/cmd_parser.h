#ifndef C3230_CMD_PARSER
#define C3230_CMD_PARSER

#include "isr_linked_list.h"

#define PE_CONS_PIPE (-1)
#define PE_ST_PIPE (-2)
#define PE_ED_PIPE (-3)
#define PE_BG_IN_THE_MID (-4)
#define PE_MULTI_BG (-5)

struct CMDs {
    int background;
    struct ISRLinkedList *command_list;
};

const char *translate_parse_error(int pe);

int parse(char *str, struct CMDs *cmds);

#endif