#ifndef BUILTINS_H
#define BUILTINS_H

/*  Parses out and executes the following commands:
 *      exit / x
 *      cd [directory]
 *      get [variable]
 *      set [variable] [value]
 *      alias [name] [command]
 *      help [topic]
 *      source [file]
 *      tokens [command]
 *
 *  Returns 1 if a builtin was found, 0 otherwise.
 */
int parse_builtins(char **tokens, int *num_tokens);

#endif
