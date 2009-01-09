#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "redirect.h"

void swap_fd(int in_fd, int app_fd, int out_fd) {
    if (in_fd != -1) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (app_fd != -1) {
        dup2(app_fd, STDOUT_FILENO);
        close(app_fd);
    }
    if (out_fd != -1) {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }    
}

int redirect(char *file_name, char *type) {
    int file = -1;
    if (file_name != NULL) {
        if (!strcmp(type, "<")) {
            file = open(file_name, O_RDONLY);
        } else if (!strcmp(type, ">")) {
            file = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0770);
        } else if (!strcmp(type, ">>")) {
            file = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0770);
        }
        if (file == -1) {
            f_error(type, file_name);
            exit(1);
        }
        free(file_name);
    }
    return file;
}

char* parse_redirect(char **tokens, int* num_tokens, char *search) {
    // array to hold tokens not involved in redirection
    char **new_tokens = calloc(*num_tokens, sizeof *new_tokens);
    int count = 0; // holds count of new_tokens
    int changed = 0; // flag to indicate tokens was altered

    char *ret = NULL; // file name to return
    int n_tokens = 0; // loop counter
    while (n_tokens < *num_tokens) {
        // this makes sure > doesn't affect >>
        if (strlen(tokens[n_tokens]) < strlen(search)) {
            // allocate space and add token to new_tokens
            new_tokens[count] = calloc(1, strlen(tokens[n_tokens]) + 1);
            strcpy(new_tokens[count], tokens[n_tokens]);
            count++;
            n_tokens++;
            continue;
        }
        // copy the first 1 or 2 chars to compare to search
        char *ptr = calloc(1, strlen(search) + 1);
        strncpy(ptr, tokens[n_tokens], strlen(search));
        // if a redirect symbol is found
        if (!strcmp(ptr, search)) {
            changed = 1; // flag that array was changed
            // if just the symbol, i.e. '> a.txt'
            if (strlen(tokens[n_tokens]) == strlen(search)) {
                // if the redirect symbol is last, indicate error
                if ((n_tokens + 1) == *num_tokens) {
                    error(search, "no argument given");
                    changed = 0;
                } else { // copy filename to ret
                    ret = calloc(1, strlen(tokens[n_tokens + 1]) + 1);
                    strcpy(ret, tokens[n_tokens + 1]);
                    n_tokens++;
                }
            // if no space, i.e. '>a.txt'    
            } else {
                if (!strcmp(search, ">>")) tokens[n_tokens]++;
                tokens[n_tokens]++;
                ret = calloc(1, strlen(tokens[n_tokens]) + 1);
                strcpy(ret, tokens[n_tokens]);
            }
        } else { // otherwise add the token to new_tokens
            new_tokens[count] = calloc(1, strlen(tokens[n_tokens]) + 1);
            strcpy(new_tokens[count], tokens[n_tokens]);
            count++;
        }
        n_tokens++;
        free(ptr);
    }

    int i = 0;
    while (i < count) {
        // free up the old array
        free(tokens[i]);
        // copy from the new array
        tokens[i] = calloc(1, strlen(new_tokens[i]) + 1);
        strcpy(tokens[i], new_tokens[i]);
        // free up the new array
        free(new_tokens[i]);
        i++;
    }
    // free up the rest of the space
    while (i < *num_tokens) {
        free(tokens[i]);
        free(new_tokens[i]);
        i++;
    }
    free(new_tokens);
    *num_tokens = count;

    // return the file name
    return ret;
}
