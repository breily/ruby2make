#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file.h"
#include "eval.h"

int read_file(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        return 0;
    }

    char *command = calloc(1, 1024);
    strcpy(command, "");
    int len = 0;
    int c;
    while ((c = getc(fp)) != EOF) {
        if (c == '\n') {
            if (len) eval(command);
            free(command);
            char *command = calloc(1, 1024);
            strcpy(command, "");
            len = 0;
        } else {
            command[len] = c;
            len++;
        }
    }

    fclose(fp);
    return 1;
}
