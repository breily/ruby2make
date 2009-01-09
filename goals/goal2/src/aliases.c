#include <string.h>
#include <stdlib.h>

#include "aliases.h"

// Store aliases.
ALIAS aliases[] = {};
int num_aliases = 0;

// Add an alias - if it exists already it overwrites the current
// alias.  Currently there is no way to unalias something.
int set_alias(char *name, char *command) {
    // Check for existing alias.
    int i = 0;
    while (i < num_aliases) {
        // Overwrite existing alias.
        if (!strcmp(aliases[i].name, name)) {
            free(aliases[i].command);
            aliases[i].command = calloc(1, strlen(command) + 1);
            strcpy(aliases[i].command, command);
            return 1;
        }
        i++;
    }
    // Reached alias limit - this is dumb to have a limit, I should
    // probably change it to some form of linked list.
    if (num_aliases == 100) return 0;
    // Add a new alias.
    char *a_name = calloc(1, strlen(name) + 1);
    strcpy(a_name, name);
    char *a_command = calloc(1, strlen(command) + 1);
    strcpy(a_command, command);
    ALIAS a = { a_name, a_command };
    aliases[num_aliases] = a;
    num_aliases++;
    return 2;
}

// Expands aliases at the beginning of command lines - no global aliases.
void expand_aliases(char *line) {
    // Iterate through the list of aliases.
    int a = 0;
    while (a < num_aliases) {
        int alias_len = strlen(aliases[a].name);
        int is_match = 1;
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
