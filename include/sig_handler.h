#ifndef C3230_SIG_HANDLER
#define C3230_SIG_HANDLER

void handle_sig_int(int signum);

void handle_sig_tstp(int signum);

void handle_sig_chld(int signum);

#endif