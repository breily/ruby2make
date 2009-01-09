#include <stdlib.h>
#include <stdio.h>

#include "environ.h"
#include "eval.h"
#include "init.h"
#include "prompt.h"
#include "input.h"

int main(int argc, char **argv) {
    /*
     *  Initialize everything we need before reading any commands.
     *  See init.c for explanations.
     */
    init_settings();
    init_options(argc, argv);
    init_execute();
    init_script();
    init_history();
    init_msg();
    init_config();

    while (1) {
        // Expand macros in the prompt.
        char *prompt = (char *) eval_prompt(get_env("@prompt"));
        // Use readline() to read input.
        char *command = calloc(1, 1024);
        command = (char *) read_stdin(prompt);

        eval(command);

        // Be responsible and call free.
        free(command);
        free(prompt);
    }
}
