#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

void read_line(char *ret) {
    strcpy(ret, "");
    int len = 0;
    int cur;
    while ((cur = getchar()) != EOF) {
        if (cur == '\n') break;
        ret[len] = cur;
        len++;
    }
    if (cur == EOF && len == 0) return;
    ret[len] = '\0';
}

int count_spaces(char *line) {
    int spaces = 0;
    int i = 0;
    while (line[i] != '\0') {
        if (line[i] == ' ') spaces++;
        i++;
    }
    return spaces;
}

int store_token(char *token, char **array, int count) {
    array[count] = calloc(1, strlen(token) + 1);
    strcpy(array[count], token);
    strcpy(token, "");
    return ++count;
}

int split_line(char *line, char **ret) {
    int single_quote_flag = 0;
    int double_quote_flag = 0;

    char token[strlen(line)];
    strcpy(token, "");
    
    int counter = 0;

    int j = 0;
    while (j < strlen(line)) {
        if (line[j] == '\"') {
            if (!double_quote_flag) {
                double_quote_flag = 1;
                if (single_quote_flag) {
                    strncat(token, &line[j], 1);
                }
            } else {
                double_quote_flag = 0;
                if (!single_quote_flag) {
                    counter = store_token(token, ret, counter);
                } else {
                    strncat(token, &line[j], 1);
                }
            }   
        } else if (line[j] == '\'') {
            if (!single_quote_flag) {
                single_quote_flag = 1;
                if (double_quote_flag) {
                    strncat(token, &line[j], 1);
                }
            } else {
                single_quote_flag = 0;
                if (!double_quote_flag) {
                    counter = store_token(token, ret, counter);
                } else {
                    strncat(token, &line[j], 1);
                }
            }   
        } else if (line[j] == ' ' || line[j] == '\t') {
            if (single_quote_flag || double_quote_flag) {
                strncat(token, &line[j], 1);
            } else {
                if (strlen(token) != 0) {
                    counter = store_token(token, ret, counter);
                }
            }
        } else {
            strncat(token, &line[j], 1);
        }
        j++;
    }
    if (strlen(token) != 0) {
        counter = store_token(token, ret, counter);
    }
    return counter;
}

void strip_leading_ws(char *line) {
    char *tmp = calloc(1, strlen(line) + 1);
    int i = 0;
    while (line[i] == '\t' || line[i] == ' ') { i++; }
    if (i > 0) {
        strcpy(tmp, &line[i]);
        strcpy(line, tmp);
    }
    free(tmp);
}

void strip_comments(char *line) {
    char *comment = strchr(line, '#');
    if (comment != NULL) {
        printf("checking '%s'...\n", comment);
        int i = 0;
        while (strcmp(comment, &line[i])) { i++; }
        printf("matched at %d...\n", i);
        if (i == 0) {
            strcpy(line, "\0");
        } else {    
            char *tmp = calloc(1, strlen(line) + 1);
            strncpy(tmp, line, i);
            strcpy(line, tmp);
            free(tmp);
        }
    }
}
