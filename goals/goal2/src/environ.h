#ifndef ENVIRON_H
#define ENVIRON_H

/*  Holds settings data.
 */
typedef struct {
    char *name;
    char *value;
} ENV_VAR;

/*  The array of environment variables.
 */
int num_variables;
ENV_VAR variables[100];

/*  Returns the value of the environment variable
 *  *name.  If not found, returns "".
 */
char* get_env(char *name);

/*  Sets the value of environment variable *name to
 *  *value.  Creates if necessary.
 *
 *  Returns 1 if an existing variable was changed.
 *  Returns 0 if maximum number of variables reached (100).
 *  Returns 2 if a new variable was added.
 */
int set_env(char *name, char *value);

/*  An alias for get_env() specifically for settings - 
 *  returns an int instead of a string.
 */
int get_setting(char *name);

/*  An alias for set_env() specifically for settings - 
 *  takes an int instead of a string.
 */
void set_setting(char *name, int value); 

/*  Replaces {@var} strings with the values of vars.
 */
void sub_env(char *line);

#endif
