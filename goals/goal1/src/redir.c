#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "error.h"
#include "redir.h"
#include "parser.h"

// opens a filename and returns its file descriptor
int get_infile(PARSED *tokens) {
    REDIRECTION file = _get_filename(*tokens, "<");
    if (file.file_name != NULL) {
        int fd = open(file.file_name, O_RDONLY);
        if (fd == -1) {
            f_error("<", file.file_name);
            exit(EXIT_FAILURE);
        }
        free(file.file_name);
        *tokens = file.tokens;
        return fd;
    }
    return -1;
}

// opens a filename either in truncate ('>') or append ('>>') mode
// and returns its file descriptor
int get_outfile(PARSED *tokens) {
    REDIRECTION file = _get_filename(*tokens, ">>");
    if (file.file_name != NULL) {
        int fd = open(file.file_name, O_WRONLY | O_APPEND | O_CREAT, 0770);
        if (fd == -1) {
            f_error(">>", file.file_name);
            exit(EXIT_FAILURE);
        }
        free(file.file_name);
        *tokens = file.tokens;
        return fd;
    } else {
        file = _get_filename(*tokens, ">");
        if (file.file_name != NULL) {
            int fd = open(file.file_name, O_WRONLY | O_CREAT | O_TRUNC, 0770);
            if (fd == -1) {
                f_error(">>", file.file_name);
                exit(EXIT_FAILURE);
            }
            free(file.file_name);
            *tokens = file.tokens;
            return fd;
        }
    }
    return -1;
}

// looks for a filename after a certain symbol - either '<', '>', or '>>'
// returns the file name given - a space is not necessary
REDIRECTION _get_filename(PARSED tokens, char *search) {
    // array to hold tokens not involved in redirection
    char **new_tokens = calloc(tokens.count, sizeof *new_tokens);
    int count = 0; // holds count of new_tokens
    int changed = 0; // flag to indicate tokens was altered

    char *ret = NULL; // file name to return
    int n_tokens = 0; // loop counter
    while (n_tokens < tokens.count) {
        // this makes sure > doesn't affect >>
        if (strlen(tokens.storage[n_tokens]) < strlen(search)) {
            // allocate space and add token to new_tokens
            new_tokens[count] = calloc(1, strlen(tokens.storage[n_tokens]) + 1);
            strcpy(new_tokens[count], tokens.storage[n_tokens]);
            count++;
            n_tokens++;
            continue;
        }
        // copy the first 1 or 2 chars to compare to search
        char *ptr = calloc(1, strlen(search) + 1);
        strncpy(ptr, tokens.storage[n_tokens], strlen(search));
        // if a redirect symbol is found
        if (!strcmp(ptr, search)) {
            changed = 1; // flag that array was changed
            // if just the symbol, i.e. '> a.txt'
            if (strlen(tokens.storage[n_tokens]) == strlen(search)) {
                // if the redirect symbol is last, indicate error
                if ((n_tokens + 1) == tokens.count) {
                    error(search, "no argument given");
                    changed = 0;
                } else { // copy filename to ret
                    ret = calloc(1, strlen(tokens.storage[n_tokens + 1]) + 1);
                    strcpy(ret, tokens.storage[n_tokens + 1]);
                    n_tokens++;
                }
            // if no space, i.e. '>a.txt'    
            } else {
                if (!strcmp(search, ">>")) tokens.storage[n_tokens]++;
                tokens.storage[n_tokens]++;
                ret = calloc(1, strlen(tokens.storage[n_tokens]) + 1);
                strcpy(ret, tokens.storage[n_tokens]);
            }
        } else { // otherwise add the token to new_tokens
            new_tokens[count] = calloc(1, strlen(tokens.storage[n_tokens]) + 1);
            strcpy(new_tokens[count], tokens.storage[n_tokens]);
            count++;
        }
        n_tokens++;
        free(ptr);
    }
    PARSED p = { new_tokens, count };
    REDIRECTION file = { p, ret };
    return file;
}
