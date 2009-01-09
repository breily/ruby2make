#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <unistd.h>
#include <readline/history.h>

#include "input.h"
#include "builtin.h"
#include "environ.h"
#include "help.h"
#include "globbing.h"
#include "eval.h"
#include "error.h"
#include "parser.h"
#include "aliases.h"

int check_builtin(PARSED tokens) {
    char *cmds[] = { "cd", "get", "set", "history", "help",
        "alias", "source", "tokens", "x", "exit" };
    int i = 0;
    while (i < 10) {
        if (!strcmp(cmds[i], tokens.storage[0])) return 1;
        i++;
    }
    return 0;
}

void exec_builtin(PARSED tokens, int in_file, int out_file) {
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);

    if (in_file != -1) {
        dup2(in_file, STDIN_FILENO);
    }
    if (out_file != -1) {
        dup2(out_file, STDOUT_FILENO);
    }

    if (get_setting("@background?")) {
        if (in_file == -1) close(STDIN_FILENO);
        else close(in_file);
        if (out_file == -1) close(STDOUT_FILENO);
        else close(out_file);
        close(STDERR_FILENO);
    }

    char *cmd = tokens.storage[0];
    if (!strcmp(cmd, "x") || !strcmp(cmd, "exit")) {
        builtin_exit(tokens);
    } else if (!strcmp(cmd, "cd")) {
        builtin_cd(tokens);
    } else if (!strcmp(cmd, "get")) {
        builtin_get(tokens);
    } else if (!strcmp(cmd, "set")) {
        builtin_set(tokens);
    } else if (!strcmp(cmd, "alias")) {
        builtin_alias(tokens);
    } else if (!strcmp(cmd, "help")) {
        builtin_help(tokens);
    } else if (!strcmp(cmd, "source")) {
        builtin_source(tokens);
    } else if (!strcmp(cmd, "tokens")) {
        builtin_tokens(tokens);
    } else if (!strcmp(cmd, "history")) {
        builtin_history(tokens);
    }

    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
}

void builtin_exit(PARSED tokens) {
    if (get_setting("@verbose?")) {
        fprintf(stdout, "%s\n", get_env("@exit"));
    }
    if (get_setting("@history?")) {
        if (get_setting("@use_hist_file?")) {
            if (write_history(get_env("@hist_file")) != 0) {
                error("history", "couldn't write history file");
            }
        }
    }
    int i = num_aliases - 1;
    while (i > -1) {
        free(aliases[i].name);
        free(aliases[i].command);
        i--;
    }
    i = num_variables - 1;
    while (i > -1) {
        free(variables[i].name);
        free(variables[i].value);
        i--;
    }
    i = tokens.count - 1;
    while (i > -1) {
        free(tokens.storage[i]);
        i--;
    }
    exit(0);
}

void builtin_cd(PARSED tokens) {
    // error checking
    if (tokens.count > 2) {
        error("cd", "too many arguments");
        return;
    }
    // figure out the directory
    char *dir;
    if (tokens.count == 1) {
        dir = getenv("HOME");
    } else if (get_setting("@glob?") && check_glob(tokens)) {
        glob_t gl = setup_glob(tokens);
        if (gl.gl_pathc < 1) {
            error("cd", "no matching directories found");
            return;
        } else if (gl.gl_pathc > 1) {
            error("cd", "too many matches");
            while (gl.gl_pathc > 0) {
                fprintf(stderr, "      - match found: %s\n", gl.gl_pathv[gl.gl_pathc]);
                gl.gl_pathc--;
            }
            return;
        } else {
            dir = gl.gl_pathv[1];
        }
    } else {
        if (!strcmp(tokens.storage[1], "-")) {
            dir = get_env("@oldpwd");
        } else {
            dir = tokens.storage[1];
        }
    }
    // swap in user's home directory
    if (!strcmp(dir, "~")) {
        dir = getenv("HOME");
    }
    // change directory and error check
    if (chdir(dir) == -1) {
        error("cd", "directory not found");
    } else {
        if (get_setting("@ls_after_cd?")) {
            eval(get_env("@ls_command"));
        }
        set_env("@oldpwd", get_env("@pwd"));
        set_env("@pwd", dir);
    }
}

void builtin_get(PARSED tokens) {
    if (tokens.count == 1) {
        printf("%s\n", get_env("-all"));
    } else {
        printf("%s\n", get_env(tokens.storage[1]));
    }
}

void builtin_set(PARSED tokens) {
    if (tokens.count != 3) {
        error("set", "wrong number of arguments");
    } else {
        set_env(tokens.storage[1], tokens.storage[2]);
    }
}

void builtin_alias(PARSED tokens) {
    if (tokens.count == 1) {
        printf("%s\n", get_env("-aliases"));
    } else if (tokens.count < 3) {
        error("alias", "wrong number of arguments");
    } else {
        set_alias(tokens.storage[1], tokens.storage[2]);
    }
}

void builtin_help(PARSED tokens) {
    print_help(tokens);
}

void builtin_source(PARSED tokens) {
    if (tokens.count == 1) {
        error("source", "no arguments given");
    } else {
        int i = 1;
        while (i < tokens.count) {
            int status = read_file(tokens.storage[i]);
            if (!status) {
                f_error("source", tokens.storage[i]);
            }
            i++;
        }
    }
}

void builtin_tokens(PARSED tokens) {
    int i = 0;
    while (i < tokens.count) {
        printf("[token %d] %s\n", i, tokens.storage[i]);
        i++;
    }
}

void builtin_history(PARSED tokens) {
    if (tokens.count == 1) {
        HIST_ENTRY *entry;
        int i = history_base;
        while (i < history_length) {
            entry = history_get(i);
            printf("[%d] %s\n", i, entry->line);
            i++;
        }
    } else if (!strcmp(tokens.storage[1], "-clear")) {
        clear_history();
    }
}
