#include <stdlib.h>
#include <stdio.h>

#include "environ.h"
#include "aliases.h"

void exit_shell() {
    if (get_setting("verbose?")) {
        fprintf(stdout, "%s\n", get_env("@exit"));
    }

    int i = num_variables - 1;
    while (i > -1) {
        free(variables[i].name);
        free(variables[i].value);
        i--;
    }

    i = num_aliases - 1;
    while (i > -1) {
        free(aliases[i].name);
        free(aliases[i].command);
        i--;
    }

    exit(0);
}        
