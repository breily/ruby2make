#ifndef REDIR_H
#define REDIR_H

#include "parser.h"

/*
 *  Used just as the return value for the _get_filename function - 
 *  holds a PARSED object because _get_filename removes filename 
 *  and redirection symbols from the tokens.
 */
typedef struct {
    PARSED tokens;
    char *file_name;
} REDIRECTION;

/*
 *  If necessary opens a fd to replace stdin, returns it.
 */
int get_infile(PARSED *tokens);

/*
 *  If necessary opens a fd to replace stdout, returns it.
 */
int get_outfile(PARSED *tokens);

/*
 *  Parses out the redirect file name from a PARSED struct.
 */
REDIRECTION _get_filename(PARSED tokens, char *search);

#endif
