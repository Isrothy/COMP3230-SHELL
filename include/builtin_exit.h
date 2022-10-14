#ifndef CS3230_SHELL_EXIT
#define CS3230_SHELL_EXIT

/**
 * @brief execute exit command
 * first check whether the input is valid.
 * Then, send SIGKILL to all background processes and release the resources.
 * Finally, exit CS3230 shell
 *
 * @param arg_list a list of argument. arg_list[0] should be "exit"
 */
void builtin_exit(char **arg_list);

#endif