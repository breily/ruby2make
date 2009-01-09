#ifndef REDIRECT_H
#define REDIRECT_H

/*  For each fd that is not -1, calls dup2() to
 *  exchange it with the appropriate stdin/stdout.
 */
void swap_fd(int in_fd, int app_fd, int out_fd);

/*  Redirects either stdin from file_name or
 *  stdout to file_name, depending on type.
 *
 *  Returns the new file descriptor if successful,
 *  otherwise -1.
 */
int redirect(char *file_name, char *type);

/*  Checks if a redirect is used and redirects
 *  stdin/stdout as necessary.
 */
char* parse_redirect(char **tokens, int *num_tokens, char *search);

#endif
