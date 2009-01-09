#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environ.h"
#include "error.h"

// default error message
void error(char *fail, char *msg) {
    if (get_setting("@error_msgs?")) {
        fprintf(stderr, "error - '%s' failed: %s;\n", fail, msg);
    }
}

// error given when a file cannot be opened - just calls error()
void f_error(char *fail, char *file) {
    char *msg = calloc(1, strlen(file) + 18);
    sprintf(msg, "could not open '%s'", file);
    error(fail, msg);
    free(msg);
}
