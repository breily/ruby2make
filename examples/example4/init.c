#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "init.h"
#include "options.h"
#include "eval.h"
#include "environ.h"
#include "file.h"
#include "aliases.h"

void init_settings(int argc, char **argv) {
    set_env("@config",      "profile.slash");
    set_env("@start",       "starting slash...");
    set_env("@exit",        "exiting slash...");
    set_env("@prompt",      "@pwd@$ ");
    set_env("@version",     "0.2");
    set_env("@author",      "brian reily");
    set_env("@author_id",   "bjr5e");

    set_env("verbose?",     "1");
    set_env("glob?",        "1");
    set_env("execute?",     "0");
    set_env("exit?",        "0");
    set_env("config?",      "1");
    set_env("script?",      "0");
    set_env("aliases?",     "1");
    set_env("ls_after_cd?", "1");
    set_env("@ls_command",  "ls");

    set_env("background?",  "0");

    parse_opts(argc, argv);
}

void init_execute() {
    if (get_setting("execute?")) {
        eval(get_env("@command"));
        if (get_setting("exit?")) {
            exit(0);
        }
    }
}    

void init_script() {
    if (get_setting("script?")) {
        int ret = read_file(get_env("@script"));
        if (ret == 0) {
            f_error("script", get_env("@script"));
        }
        if (get_setting("exit?")) {
            exit(0);
        }
    }
}

void init_msg() {
    if (get_setting("verbose?")) {
        printf("%s\n", get_env("@start"));
    }
}    

void init_config() {
    if (get_setting("config?")) {
        int ret = read_file(get_env("@config"));
        if (ret == 0) {
            f_error("config", get_env("@config"));
        }
    }
}    
