#ifndef C3230SHELL_SHELL_IO
#define C3230SHELL_SHELL_IO

/**
 * @brief Get the command line prompt
 *
 * @return char const*
 */
const char *getPrompt();

/**
 * @brief Get the Error Prefix. By default "3230shell: "
 *
 * @return const char*
 */
const char *getErrorPrefix();

/**
 * @brief read chars until '\n'
 *
 * @return char*
 */
char *readline();

/**
 * @brief encapsuliate the output.
 *
 * @param format string format just like printf
 * @param ... arguments
 */
void shell_output(const char *format, ...);

/**
 * @brief encapsuliate the error output
 *
 * @param format string format just like printf
 * @param ... arguments
 */
void shell_error(const char *format, ...);

#endif