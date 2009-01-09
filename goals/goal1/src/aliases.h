#ifndef ALIASES_H
#define ALIASES_H

/*  Structure to hold alias data.
 */
typedef struct {
    char *name;
    char *command;
} ALIAS;

/*  Stores the aliases, with
 *  num_aliases being the count.
 */
int num_aliases;
ALIAS aliases[100];

/*  Sets an alias with name *name that substitutes
 *  to *command.
 */
int set_alias(char *name, char *command);

/*  Checks the first token of the command to see if it
 *  is an alias, replaces it if necessary.
 */
void expand_aliases(char *line);

#endif
