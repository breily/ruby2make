#ifndef PARSE_H
#define PARSE_H

/*  Reads one line from stdin,
 *  stores in ret.
 */
void read_line(char *ret);

/*  Counts the spaces and returns the
 *  number to determine the maximum
 *  number of tokens.
 */
int count_spaces(char *line);

/*  Stores the token in the array at
 *  the spot indicated by counter.
 *  Increments counter and returns it.
 */
int store_token(char *token, char **array, int count); 

/*  Splits line into array of tokens,
 *  returns this in ret.
 *
 *  Tokens are split on whitespace, or
 *  within quotes.
 */
int split_line(char *line, char **ret);

/*  Removes leading whitespace.
 */
void strip_leading_ws(char *line);

/*  Removes comments ('#').
 */
void strip_comments(char *line);

#endif
