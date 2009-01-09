#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

#include "builtins.h"
#include "environ.h"
#include "help.h"
#include "globbing.h"
#include "eval.h"
#include "error.h"
#include "exit_shell.h"
#include "redirect.h"

int builtin_redirect(char **tokens, int *num_tokens) {
    char *app = ">>"; char *out = ">";
    int app_fd = redirect(parse_redirect(tokens, num_tokens, app), app);
    int out_fd = redirect(parse_redirect(tokens, num_tokens, out), out);
    swap_fd(-1, app_fd, out_fd);
    if (app_fd != -1 || out_fd != -1) return 1;
    return 0;
}

int parse_builtins(char **tokens, int *num_tokens) {
    int out_copy = dup(1);
    // exit
    if (!strcmp(tokens[0], "x") || !strcmp(tokens[0], "exit")) {
        exit_shell();
    // cd [DIRECTORY]
    } else if (!strcmp(tokens[0], "cd")) {
        // error checking
        if (*num_tokens > 2) {
            error("cd", "too many arguments");
            return 1;
        }
        if (builtin_redirect(tokens, num_tokens)) {
            dup2(out_copy, 1);
            error("cd", "cannot be redirected");
            return 1;
        }
        // figure out the directory
        char *dir;
        if (*num_tokens == 1) {
            dir = getenv("HOME");
        } else if (get_setting("glob?") && check_glob(tokens, *num_tokens)) {
            glob_t gl = setup_glob(tokens, *num_tokens);
            if (gl.gl_pathc < 1) {
                error("cd", "no matching directories found");
                return 1;
            } else if (gl.gl_pathc > 1) {
                error("cd", "too many matches");
                while (gl.gl_pathc > 0) {
                    fprintf(stderr, "      - match found: %s\n", gl.gl_pathv[gl.gl_pathc]);
                    gl.gl_pathc--;
                }
                return 1;
            } else {
                dir = gl.gl_pathv[1];
            }
        } else {
            dir = tokens[1];
        }
        // swap in user's home directory
        if (!strcmp(dir, "~")) {
            dir = getenv("HOME");
        }
        // change directory and error check
        if (chdir(dir) == -1) {
            error("cd", "directory not found");
        } else {
            if (get_setting("ls_after_cd?")) {
                eval(get_env("@ls_command"));
            }
        }
        return 1;
    // get [VARIABLE]
    } else if (!strcmp(tokens[0], "get")) {
        if (*num_tokens == 1) {
            error("get", "no argument given");
            return 1;
        }
        builtin_redirect(tokens, num_tokens);
        char *var = get_env(tokens[1]);
        printf("%s\n", var);
        free(var);
        dup2(out_copy, 1);
        return 1;
    // set [VARIABLE] [VALUE]    
    } else if (!strcmp(tokens[0], "set")) {
        if (*num_tokens != 3) {
            error("set", "wrong number of arguments");
            return 1;
        }    
        if (builtin_redirect(tokens, num_tokens)) {
            dup2(out_copy, 1);
            error("set", "cannot be redirected");
            return 1;
        }
        set_env(tokens[1], tokens[2]);
        return 1;
    // alias [NAME] [COMMAND]    
    } else if (!strcmp(tokens[0], "alias")) {
        if (*num_tokens < 3) {
            error("alias", "wrong number of arguments");
        } else if (builtin_redirect(tokens, num_tokens)) {
            dup2(out_copy, 1);
            error("set", "cannot be redirected");
            return 1;
        } else {
            set_alias(tokens[1], tokens[2]);
        }
        return 1;
    // help [TOPIC]    
    } else if (!strcmp(tokens[0], "help")) {
        builtin_redirect(tokens, num_tokens);
        print_help(tokens, *num_tokens);
        dup2(out_copy, 1);
        return 1;
    // source [FILE] [FILE] ... [FILE]    
    } else if (!strcmp(tokens[0], "source")) {
        if (*num_tokens == 1) {
            error("source", "no argument given");
        } else {
            builtin_redirect(tokens, num_tokens);
            int i = 1;
            while (i < *num_tokens) {
                int ret = read_file(tokens[i]);
                if (ret == 0) {
                   f_error("source", tokens[i]);
                }
                i++;
            }
            dup2(out_copy, 1);
            return 1;
        }
    // tokens [COMMAND]    
    } else if (!strcmp(tokens[0], "tokens")) {
        builtin_redirect(tokens, num_tokens);
        int i = 0;
        while (i < *num_tokens) {
            printf("tokens[%d]: %s\n", i, tokens[i]);
            i++;
        }
        dup2(out_copy, 1);
        return 1;
    }
    return 0;
}
