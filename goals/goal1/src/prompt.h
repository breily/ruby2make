#ifndef PROMPT_H
#define PROMPT_H

/*  Evaluates the macro, substituting it
 *  into *prompt.  Currently accepts:
 *      @pwd@
 *      @dateFORMAT@
 *      @user@
 *      more... (see prompt.c)
 */
void eval_macro(char *macro, char *prompt);

/*  Takes the prompt, parsing out and passing
 *  macro blocks to eval_macro().  At the end,
 *  prints prompt.
 */
char *eval_prompt(char *prompt);

#endif
