#include <string.h>
#include <stdlib.h>

#include "aliases.h"

ALIAS aliases[] = {
};

int num_aliases = 0;

int set_alias(char *name, char *command) {
    int i = 0;
    while (i < num_aliases) {
        if (!strcmp(aliases[i].name, name)) {
            free(aliases[i].command);
            aliases[i].command = calloc(1, strlen(command) + 1);
            strcpy(aliases[i].command, command);
            return 1;
        }
        i++;
    }
    if (num_aliases == 100) return 0;
    char *a_name = calloc(1, strlen(name) + 1);
    strcpy(a_name, name);
    char *a_command = calloc(1, strlen(command) + 1);
    strcpy(a_command, command);
    ALIAS a = { a_name, a_command };
    aliases[num_aliases] = a;
    num_aliases++;
    return 2;
}

void expand_aliases(char *line) {
    int a = 0;
    while (a < num_aliases) {
        int alias_len = strlen(aliases[a].name);
        int is_match = 1;
        char *cmd;
        int len = 0;
        while (line[len] != ' ' && len < strlen(line)) { len++; }
        if (len != alias_len) {
            a++;
            continue;
        }    
        int i = 0;
        while (i < len) {
            if (line[i] != aliases[a].name[i]) {
                is_match = 0;
                break;
            }    
            i++;
        }
        if (is_match) {
            char *tmp = calloc(1, strlen(line) + strlen(aliases[a].command) + 1);
            strcpy(tmp, aliases[a].command);
            strcat(tmp, &line[len]);
            strcpy(line, tmp);
            free(tmp);
        }
        a++;
    }
}
