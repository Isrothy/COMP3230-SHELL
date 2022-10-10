#ifndef C3230_SHELL_BUILTIN_TIMEX
#define C3230_SHELL_BUILTIN_TIMEX

#include "cmd_parser.h"

/**
 * @brief execute builtin timeX command
 * First check whether the input is valid.
 * Then, run the commands and show the running times of all commands
 *
 * @param cmds
 */
void builtin_timex(struct CMDs cmds);

#endif