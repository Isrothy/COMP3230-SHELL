#ifndef C3230_CMD_PARSER
#define C3230_CMD_PARSER

#include "isr_linked_list.h"

/**
 * @brief two consecutive pipes
 *
 */
#define PE_CONS_PIPE (-1)

/**
 * @brief start with pipe
 *
 */
#define PE_ST_PIPE (-2)

/**
 * @brief end with pipe
 *
 */
#define PE_ED_PIPE (-3)

/**
 * @brief & in the middle of a command
 *
 */
#define PE_BG_IN_THE_MID (-4)

/**
 * @brief multiple & signs
 *
 */
#define PE_MULTI_BG (-5)

/**
 * @brief result of parsing
 * background: none-zero if running in the background, zero if foreground
 *
 */
struct CMDs {
    int background;
    struct ISRLinkedList *command_list;
};

/**
 * @brief translate error number into human readable message
 *
 * @param pe error number
 * @return const char*
 */
const char *translate_parse_error(int pe);

/**
 * @brief translate user input into commands
 *
 * @param str input. a line of commands
 * @param cmds result of translation
 * @return int 0 if success. negative number if error.
 */
int parse(char *str, struct CMDs *cmds);

#endif