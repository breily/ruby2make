#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "environ.h"
#include "aliases.h"

ENV_VAR variables[] = {};
int num_variables = 0;

char* get_env(char *n) {
    int i = 0;
    if (!strcmp(n, "-all")) {
        while (i < num_variables) {
            fprintf(stdout, "%s: %s", variables[i].name, variables[i].value);
            i++;
            if (i != num_variables) printf("\n");
        }
    } else if (!strcmp(n, "-aliases")) {
        while (i < num_aliases) {
            fprintf(stdout, "%s: %s", aliases[i].name, aliases[i].command);
            i++;
            if (i != num_aliases) printf("\n");
        }
    } else {
        while (i < num_variables) {
            if (!strcmp(variables[i].name, n)) {
                char *ret = calloc(1, strlen(variables[i].value) + 1);
                strcpy(ret, variables[i].value);
                return ret;
            }
            i++;
        }
    }
    char *ret = calloc(1, 1);
    *ret -= *ret;
    return ret;
}

int set_env(char *name, char *value) {
    int i = 0;
    while (i < num_variables) {
        if (!strcmp(variables[i].name, name)) {
            free(variables[i].value);
            variables[i].value = calloc(1, strlen(value) + 1);
            strcpy(variables[i].value, value);
            return 1;
        }
        i++;
    }
    if (num_variables == 100) return 0;
    char *env_name = calloc(1, strlen(name) + 1);
    strcpy(env_name, name);
    char *env_value = calloc(1, strlen(value) + 1);
    strcpy(env_value, value);
    ENV_VAR var = { env_name, env_value };
    variables[num_variables] = var;
    num_variables++;
    return 2;
}

int get_setting(char *name) {
    return atoi(get_env(name));
}

void set_setting(char *name, int value) {
    if (value == 0) set_env(name, "0");
    else set_env(name, "1");
}

void sub_env(char *line) {
    if (strchr(line, '{') != NULL && strchr(line, '}') != NULL &&
            strchr(line, '@') != NULL) {
        char *var = calloc(1, strlen(line) - 2);
        int in_var = 0;
        int start = 0;
        int end = 0;
        int i = 0;
        while (i < strlen(line)) {
            if (line[i] == '{' && line[i - 1] != '\\') {
                in_var = 1;
                start = i;
            } else if (line[i] == '}' && line[i - 1] != '\\') {
                in_var = 0;
                end = i + 1;
            } else {
                if (in_var) {
                    strncat(var, &line[i], 1);
                }
            }
            i++;
        }
        char *value = get_env(var);
        free(var);
        if (!strlen(value)) return;

        char *new_line = calloc(1, strlen(value) + start + (strlen(line) - end) + 1);
        strncpy(new_line, &line[0], start);
        strcat(new_line, value);
        strcat(new_line, &line[end]);

        line = realloc(line, strlen(new_line) + 1);
        strcpy(line, new_line);
        free(new_line);
    }
}
