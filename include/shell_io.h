#ifndef C3230SHELL_SHELL_IO
#define C3230SHELL_SHELL_IO

char const *getPrompt();

char *readline();

void shell_output(const char *format, ...);


void shell_error(const char *format, ...);

#endif