#include <stdlib.h>

#include "parse.h"
#include "environ.h"
#include "eval.h"
#include "init.h"

int main(int argc, char **argv) {
    init_settings(argc, argv);
    init_execute();
    init_script();
    init_msg();
    init_config();

    while (1) {
        print_prompt(get_env("@prompt"));
        char *command = calloc(1, 1024);
        read_line(command);
        eval(command);
        free(command);
    }
}
