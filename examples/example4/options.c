#include <stdio.h>
#include <stdlib.h>

#include "environ.h"
#include "options.h"

int match_opts(char *arg, char *short_opt, char *long_opt) {
    return (!strcmp(arg, short_opt) || !strcmp(arg, long_opt));
}

void parse_opts(int argc, char **argv) {
    int i = 1;
    while (i < argc) {
        if (match_opts(argv[i], "-q", "--quiet")) {
            set_setting("verbose?", 0);
        } else if (match_opts(argv[i], "-v", "--verbose")) {
            set_setting("verbose?", 1);
            fprintf(stdout, "enabling verbosity...\n");
        } else if (match_opts(argv[i], "-g", "--globbing")) {
            set_setting("glob?", 1);
            if (get_setting("verbose?"))
                fprintf(stdout, "enabling globbing...\n");
        } else if (match_opts(argv[i], "-ng", "--noglobbing")) {
            set_setting("glob?", 0);
            if (get_setting("verbose?"))
                fprintf(stdout, "disabling globbing...\n");
        } else if (match_opts(argv[i], "-c", "--config")) {
            if ((i + 1) == argc) {
                fprintf(stderr, "error - '%s' failed: no config file specified;\n", argv[i]);
            } else {
                set_setting("config?", 1);
                set_env("@config", argv[i + 1]);
                i++;
            }
        } else if (match_opts(argv[i], "-e", "--execute")) {
            if ((i + 1) == argc) {
                fprintf(stderr, "error - '%s' failed: no command specified;\n", argv[i]);
                exit(1);
            }
            set_setting("execute?", 1);
            set_setting("exit?", 1);
            set_env("@command", argv[i + 1]);
            i++;
        } else if (match_opts(argv[i], "-s", "--script")) {
            if ((i + 1) == argc) {
                fprintf(stderr, "error - '%s' failed: no script specified;\n", argv[i]);
                exit(1);
            }
            set_setting("script?", 1);
            set_setting("exit?", 1);
            set_env("@script", argv[i + 1]);
            i++;
        } else if (match_opts(argv[i], "-ne", "--noexit")) {
            set_setting("exit?", 0);
            if (get_setting("verbose?"))
                fprintf(stdout, "setting noexit...\n");
        } else if (match_opts(argv[i], "-nc", "--noconfig")) {
            set_setting("config?", 0);
            if (get_setting("verbose?"))
                fprintf(stdout, "disabling config file...\n");
        } else if (match_opts(argv[i], "-h", "--help")) {
            usage(argv[0]);
        } else if (match_opts(argv[i], "-na", "--noalias")) {
            set_setting("aliases?", 0);
            if (get_setting("verbose?"))
                fprintf(stdout, "disabling aliases...\n");
        } else if (match_opts(argv[i], "-a", "--alias")) {
            set_setting("aliases?", 1);
            if (get_setting("verbose?"))
                fprintf(stdout, "enabling aliases...\n");
        } else if (match_opts(argv[i], "-nl", "--nolsaftercd")) {
            set_setting("ls_after_cd?", 0);
            if (get_setting("verbose?"))
                fprintf(stdout, "disabling ls after cd...\n");
        } else {
            if (get_setting("verbose?")) {
                fprintf(stderr, "error - arg '%s' failed: not recognized;\n", argv[i]);
            }
        }
        i++;
    }
}
