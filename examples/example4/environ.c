#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environ.h"
#include "aliases.h"

ENV_VAR variables[] = {
};

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
    strcpy(ret, "");
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
