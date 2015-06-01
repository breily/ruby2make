#ifndef BUILTIN_H
#define BUILTIN_H

#include "parser.h"
/*
 *  Checks to see if tokens include a builtin command.
 */
int check_builtin(PARSED tokens);

/*
 *  Relay the tokens to the appropriate function.
 */
void exec_builtin(PARSED tokens, int in_file, int out_file);

/*
 *  Actually execute the command.
 */
void builtin_exit(PARSED tokens);
void builtin_cd(PARSED tokens);
void builtin_get(PARSED tokens);
void builtin_set(PARSED tokens);
void builtin_alias(PARSED tokens);
void builtin_help(PARSED tokens);
void builtin_source(PARSED tokens);
void builtin_tokens(PARSED tokens);
void builtin_history(PARSED tokens);

#endif
