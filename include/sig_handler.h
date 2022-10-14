#ifndef C3230_SIG_HANDLER
#define C3230_SIG_HANDLER

/**
 * @brief handle signals to the main process
 *
 */


/**
 * @brief handle SIGINT. just ignore it
 *
 * @param signum
 */
void handle_sig_int(int signum);

/**
 * @brief handle TSTP. just ignore it
 *
 * @param signum
 */
void handle_sig_tstp(int signum);

/**
 * @brief handle SIGCHLD. If the child ran in the background, then release the resources and show the execution information
 *
 * @param signum
 */
void handle_sig_chld(int signum);

#endif