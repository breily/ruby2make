#include <sys/types.h>

#ifndef EVAL_H
#define EVAL_H

/*  Evaluates the given command; command
 *  is one line and eval takes care of all 
 *  parsing/etc.
 */
void eval(char *command);

/*  Checks for redirects and launches the process.
 */
void launch(char **tokens, int num_tokens);

/*  Checks to see if the task is a background task.
 *  Sets the 'background?' setting and removes the 
 *  '&' from the token array.
 */
int check_background(char **tokens, int num_tokens);

#endif
