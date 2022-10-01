#ifndef C3230_CMD_PARSER
#define C3230_CMD_PARSER

#define PE_START_WITH_PIPE (-1)
#define PE_BG_IN_THE_MID (-2)
#define PE_MULTI_BG (-3)
#define PE_NO_CMD_AFTER_PIPE (-4)

struct CMDs {
    int background;
    struct ISRLinkedList *command_list;
};

const char *translate_parse_error(int pe);

int parse(char *str, struct CMDs *cmds);

#endif