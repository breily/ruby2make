#ifndef PARSER_H
#define PARSER_H

/*
 *  Stores arrays of strings.
 */
typedef struct {
    char **storage;
    int count;
} PARSED;

/*
 *  Splits a line into separate commands, separated by char splitter.
 */
PARSED split(char *line, char splitter); 

/*
 *  Splits a command into tokens.
 */
PARSED get_tokens(char *line);

/*
 *  Counts the number of a certain character.
 */
int _count_char(char *line, char search);

/*
 *  Stores string buf in array ret at spot r.
 *  Clears buf. Returns incremented value of r.
 */
int _store(char *buf, char **array, int r);

/*
 *  Frees the allocated memory in a PARSED struct.
 */
void free_parsed(PARSED p); 

/*
 *  Remove any comments - delimited by '#'.
 */
char* strip_comments(char *line);

/*
 *  Replace '~' with value of getenv("HOME").
 */
void replace_tilde(char *command); 
#endif
