#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>

#include "init.h"
#include "options.h"
#include "eval.h"
#include "environ.h"
#include "aliases.h"
#include "input.h"
#include "config.h"
#include "error.h"

// Initializes all the settings for slash.
void init_settings() {
    /*
     *  These settings can be defined before compilation in
     *  config.h - explanations are there.
     */
    set_env("@config",          CONFIG_FILE);
    set_env("@start",           START_MESSAGE);
    set_env("@exit",            EXIT_MESSAGE);
    set_env("@prompt",          PROMPT);
    set_env("@version",         VERSION);
    set_env("@author",          AUTHOR);
    set_env("@verbose?",        VERBOSE);
    set_env("@glob?",           GLOB);
    set_env("@config?",         CONFIG);
    set_env("@aliases?",        ALIASES);
    set_env("@ls_after_cd?",    LS_AFTER_CD);
    set_env("@ls_command",      LS_COMMAND);
    set_env("@no_hist_repeat?", NO_HISTORY_REPEAT);
    set_env("@history?",        HISTORY);
    set_env("@use_hist_file?",  USE_HISTORY_FILE);
    set_env("@hist_file",       HISTORY_FILE);
    set_env("@error_msgs?",     ERROR_MESSAGES);
    /*
     *  These don't really affect anything but would be pretty
     *  pointless to change - they allow use of 'cd -'
     */
    set_env("@pwd",             "~");
    set_env("@oldpwd",          "~");
    /*
     *  Internal settings for slash that would produce strange
     *  results if changed.
     */
    set_env("@execute?",        "0");
    set_env("@exit?",           "0");
    set_env("@script?",         "0");
    set_env("@background?",     "0");
}

// Parses command line options/switches.
void init_options(int argc, char **argv) {
    parse_opts(argc, argv);
}

// Used if '-e' or '--execute' is present; Evals the command
// and exits unless '-ne' or '--noexit' is present.
void init_execute() {
    if (get_setting("@execute?")) {
        eval(get_env("@command"));
        if (get_setting("@exit?")) {
            exit(0);
        }
    }
}    

// Used if '-s' or '--script' is present; Executes the script
// and exits unless '-ne' or '--noexit' is present.
void init_script() {
    if (get_setting("@script?")) {
        // Read and evaluate the script.
        int ret = read_file(get_env("@script"));
        // Error reading the file
        if (ret == 0) {
            f_error("script", get_env("@script"));
        }
        if (get_setting("@exit?")) {
            exit(0);
        }
    }
}

// Initializes the history library.
void init_history() {
    using_history();
    if (get_setting("@history?") && get_setting("@use_hist_file?")) {
        /*
        // For some reason this is broken.
        if (read_history(get_env("@hist_file")) != 0) {
            error("history", "couldn't read history file");
        }
        */
    }
}

// Prints a start message (Perhaps allow commands here? uptime?)
void init_msg() {
    if (get_setting("@verbose?")) {
        printf("%s\n", get_env("@start"));
    }
}    

// Reads in configuration file, i.e. ~/.slash_config
void init_config() {
    if (get_setting("@config?")) {
        int ret = read_file(get_env("@config"));
        if (ret == 0) {
            f_error("config", get_env("@config"));
        }
    }
}
