#ifndef ERROR_H
#define ERROR_H

/*  Indicate an error in function/command *fail,
 *  giving *msg as explanation.
 */
void error(char *fail, char *msg);

/*  Indicate an error in function/command *fail,
 *  that occurred with *file.
 *
 *  Calls error().
 */
void f_error(char *fail, char *file);

#endif
