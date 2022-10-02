#ifndef C3230SHELL_SHELL_IO
#define C3230SHELL_SHELL_IO

char const *getPrompt();

char *readline();

void shell_output(char *format, ...);


void shell_error(char *format, ...);

#endif