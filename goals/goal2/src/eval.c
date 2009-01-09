#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <glob.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>

#include "error.h"
#include "eval.h"
#include "builtin.h"
#include "globbing.h"
#include "environ.h"
#include "redir.h"
#include "aliases.h"
#include "options.h"
#include "parser.h"
#include "bg.h"

void eval(char *command) {
    /*
     *  Strip comments, replace vars, replace '~'.
     */
    //command = strip_comments(command);
    if (!strlen(command)) return;
    sub_env(command);
    replace_tilde(command);
    
    //printf("[executing] '%s'\n", command);

    /*
     *  Expand history macros, add command to history.
     */
    if (get_setting("@history?")) {
        char *expansion;
        int r = history_expand(command, &expansion);
        if (r < 0 || r == 2) {
            free(expansion);
            return;
        }
        if (r == 1) {
            printf("[hist] %s\n", expansion);
        }
        HIST_ENTRY *cur = current_history();
        if (cur != NULL) {
            if (get_setting("@no_hist_repeat?")) {
                if (strcmp(cur->line, expansion)) {
                    add_history(expansion);
                }
            } else {
                add_history(expansion);
            }
        } else {
            add_history(expansion);
        }
        command = realloc(command, strlen(expansion) + 1);
        strcpy(command, expansion);
        free(expansion);
    }

    /*
     *  Execute multiple commands if necessary.
     */
    PARSED cmds = split(command, ';');
    if (cmds.count != 0) {
        set_setting("@history?", 0);
        int i = 0;
        while (i < cmds.count) {
            eval(cmds.storage[i]);
            i++;
        }
        set_setting("@history?", 1);
        free_parsed(cmds);
        return;
    }

    /*
     *  Replaces aliases with actual commands.
     */
    if (get_setting("@aliases?")) {
        expand_aliases(command);
    }

    /*
     *  Split the command line into pipe commands.
     */
    PARSED pipes = split(command, '|');

    /*
     *  Right now, pipes don't work, so they've been disabled.  Thats
     *  at the top of the todo list though, so check back.
     */
    if (pipes.count == -1) {
        int pipes_fd[2];
        int pipe_ret = pipe(pipes_fd);
        if(pipe_ret == -1) {
            error("pipe", "couldn't connect file descriptors");
            free_parsed(pipes);
            return;
        // temporary    
        } else {
            printf("pipe between %d and %d\n", pipes_fd[0], pipes_fd[1]);
        }
        pid_t pid;
        int ret;
        int i = 0;
        while (i < pipes.count) {
            PARSED tokens = get_tokens(pipes.storage[i]);
            tokens.count = check_bg(tokens);

            if ((pid = fork()) == 0) {
                fprintf(stderr, "executing: %s\n", pipes.storage[i]);
                if (i == 0) {
                    close(STDOUT_FILENO);
                    int new_out = dup(pipes_fd[0]);
                    fprintf(stderr, "new out: %d\n", new_out);
                    //dup2(pipes_fd[1], STDOUT_FILENO);
                } else if (i == 1) {
                    close(STDIN_FILENO);
                    int new_in = dup(pipes_fd[1]);
                    fprintf(stderr, "new in: %d\n", new_in);
                    //dup2(pipes_fd[0], STDIN_FILENO);
                }
                int in_file = -1; int out_file = -1;
                exec_process(tokens, in_file, out_file);
                exit(0);
            } else if (pid > 0) {
                if (waitpid(pid, &ret, 0) != pid) {
                    error("wait", "invalid pid returned");
                }
                if (get_setting("@verbose?") && ret != 0) {
                    if (ret == 256) {
                        fprintf(stderr, "[exit %d] %s: command not found;\n", ret, command);
                    } else {
                        fprintf(stderr, "[exit %d] %s: failure?\n", ret, command);
                    }
                }
            } else {
                error(tokens.storage[0], "first fork failed");
                exit(EXIT_FAILURE);
            }
            free_parsed(tokens);
            i++;
        }
    // End pipes execution    
    } else {
        PARSED tokens = get_tokens(command);
        tokens.count = check_bg(tokens);
        /*
         *  Don't fork to exec builtins - if we fork and run a builtin like
         *  'set @prompt "$ "', then the @prompt environment variable only
         *  actually gets changed in the forked process, not in the shell
         *  that we're actually running.
         */
        if (check_builtin(tokens)) {
            int in_file = get_infile(&tokens);
            int out_file = get_outfile(&tokens);
            exec_builtin(tokens, in_file, out_file);
        /*
         *  Fork to exec programs.
         */
        } else {
            pid_t pid;
            int ret;
            if ((pid = fork()) == 0) {
                /*
                 *  Fork again to run a background task. The reason we do this
                 *  is because to detach a process from its parent you have to 
                 *  kill the parent.  At first I tried just making the parent
                 *  not wait for the child, but this just created zombie processes.
                 *  If we fork again, kill the first child, then the background
                 *  task runs as the second process.  The parent stops waiting after
                 *  the first child is killed, so everything works.
                 */
                if (get_setting("@background?")) {
                    pid_t pid2;
                    if ((pid2 = fork()) < 0) {
                        error("fork", "second fork failed");
                    } else if (pid2 > 0) {
                        // Kill child 1.
                        exit(EXIT_SUCCESS);
                    }
                    // We're now in the second child.
                    fprintf(stderr, "[%d] %s\n", getpid(), command);
                }

                int in_file = get_infile(&tokens);
                int out_file = get_outfile(&tokens);
                exec_process(tokens, in_file, out_file);

                exit(0);
            } else if (pid > 0) {
                if (waitpid(pid, &ret, 0) != pid) {
                    error("wait", "invalid pid returned");
                }
                if (get_setting("@verbose?") && ret != 0) {
                    if (ret == 256) {
                        fprintf(stderr, "[exit %d] %s: command not found;\n", ret, command);
                    } else {
                        fprintf(stderr, "[exit %d] %s: failure?\n", ret, command);
                    }
                }
            } else {
                error("fork", "first fork failed");
                exit(EXIT_FAILURE);
            }
        }
        free_parsed(tokens);
    }
    free_parsed(pipes);
}

void exec_process(PARSED tokens, int in_file, int out_file) {
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

    if (get_setting("@glob?") && check_glob(tokens)) {
        glob_t globbuf = setup_glob(tokens);
        execvp(tokens.storage[0], &globbuf.gl_pathv[0]);
    } else {
        tokens.storage[tokens.count] = (char *) 0;
        execvp(tokens.storage[0], tokens.storage);
    }
    
    _exit(EXIT_FAILURE);
}
