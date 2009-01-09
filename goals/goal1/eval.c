#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <glob.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "eval.h"
#include "parse.h"
#include "builtins.h"
#include "globbing.h"
#include "environ.h"
#include "redirect.h"
#include "aliases.h"
#include "options.h"

void eval(char *command) {
    //strip_comments(command);
    strip_leading_ws(command);
    if (!strlen(command)) {
        return; // Blank command does nothing
    }
    
    // Substitute in aliases and expand variables
    if (get_setting("aliases?")) {
        expand_aliases(command);
    }

    // Splits command into array of tokens
    int spaces = count_spaces(command);
    char **tokens = calloc((spaces + 1), sizeof *tokens);
    int num_tokens = split_line(command, tokens);

    // Builtins
    int is_builtin = parse_builtins(tokens, &num_tokens); // Parse other builtins
    if (is_builtin) {
        while (num_tokens > 0) {
            free(tokens[num_tokens - 1]);
            num_tokens--;
        }
        return;
    }
    printf("nt: %d\n", num_tokens);
    num_tokens = check_background(tokens, num_tokens);
    printf("nt: %d\n", num_tokens);

    // Fork and run foreground processes
    pid_t pid;
    int ret;
    // Child process here
    if ((pid = fork()) == 0) {
        // fork again for a background process
        if (get_setting("background?")) {
            pid_t pid2;
            if ((pid2 = fork()) < 0) {
                error("fork", "second fork failed");
            } else if (pid2 > 0) {
                exit(0);
            }
            fprintf(stdout, "[%d] %s\n", getpid(), command);
        }
        launch(tokens, num_tokens);
    }
    // Parent process here
    else if (pid > 0) {
        if (waitpid(pid, &ret, 0) != pid) {
            error("wait", "invalid pid returned");
        }
        int exit_status = WEXITSTATUS(ret);
        fprintf(stderr, "process exited with status = %d, return = %d\n", 
                exit_status, ret);
    } else {
        error("fork", "first fork failed");
        exit(1);
    }

    while (num_tokens > 0) {
        free(tokens[num_tokens - 1]);
        num_tokens--;
    }
}

void launch(char **tokens, int num_tokens) {
    // Parse any stdin/stdout redirects
    char *in = "<"; char *out = ">"; char *app = ">>";
    int in_fd = redirect(parse_redirect(tokens, &num_tokens, in), in);
    int app_fd = redirect(parse_redirect(tokens, &num_tokens, app), app);
    int out_fd = redirect(parse_redirect(tokens, &num_tokens, out), out);
    swap_fd(in_fd, app_fd, out_fd);
    // close all file descriptors for background process
    if (get_setting("background?")) {
        if (in_fd == -1) close(STDIN_FILENO);
        if (app_fd == -1 && out_fd == -1) close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }
    // if globbing is enabled and necessary
    if (get_setting("glob?") && check_glob(tokens, num_tokens)) {
        glob_t globbuf = setup_glob(tokens, num_tokens);
        execvp(tokens[0], &globbuf.gl_pathv[0]);
    // Otherwise just run the command
    } else {
        tokens[num_tokens] = (char *) 0;
        execvp(tokens[0], tokens);
    }
    // exec exits itself, so if we got here then we failed
    _exit(EXIT_FAILURE);
}

int check_background(char **tokens, int num_tokens) {
    if (!strcmp(tokens[num_tokens - 1], "&")) {
        // take away the '&' token
        free(tokens[num_tokens - 1]);
        set_setting("background?", 1);
        return (num_tokens - 1);
    } else set_setting("background?", 0);
    return num_tokens;
}
