#ifndef INPUT_H
#define INPUT_H

/*
 *  Read and eval each line in a file.
 *  Used for config files and scripts.
 */
int read_file(char *file_name);

/*
 *  Reads a line from stdin and stores it in ret.
 */
char * read_stdin(char *prompt);

#endif
