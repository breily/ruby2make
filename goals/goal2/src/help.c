#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "help.h"
#include "parser.h"
#include "environ.h"
#include "error.h"

void usage(char *arg) {
    printf("usage: '%s'\t [ -q | --quiet ]", arg);
    printf(" [ -v | --verbose ] [ -g | --globbing ] \n");
    printf("           \t\t [ -c | --config FILE ]");
    printf(" [ -e | --execute COMMAND ] \n");
    printf("           \t\t [ -s | --script FILE ]");
    printf("[ -ne | --noexit ] [ -nc | --noconfig ]\n");
    printf("           \t\t [ -a | --alias ] ");
    printf("[ -na | --noalias ] [ -nl | --nolsaftercd ]\n");
    printf("\n");
    printf("for more help, run '%s' and type 'help'\n", arg);
    if (get_setting("@exit?")) {
        exit(0);
    }
}

void general_help() {
    printf("help topics (type 'help [TOPIC]' to view): \n");
    printf("\t- alias\t\t(custom commands)\n");
    printf("\t- builtins\t(list of builtin commands)\n");
    printf("\t- cd\t\t(changing the working directory)\n");
    printf("\t- config\t(configuration files)\n");
    printf("\t- exec\t\t(executing single commands)\n");
    printf("\t- exit\t\t(quitting the shell)\n");
    printf("\t- get\t\t(viewing shell variables)\n");
    printf("\t- glob\t\t(wild card expressions)\n");
    printf("\t- output\t(messages from the shell)\n");
    printf("\t- pipes\t\t(coming soon)\n");
    printf("\t- prompt\t(setting a custom prompt)\n");
    printf("\t- redirect\t(redirecting stdin/stdout)\n");
    printf("\t- script\t(running commands from a file)\n");
    printf("\t- set\t\t(setting shell variables)\n");
    printf("\ninfo: \n");
    printf("\t- author: %s\n", get_env("@author"));
    printf("\t- version: %s\n", get_env("@version"));
}

void print_help(PARSED tokens) {
    if (tokens.count == 1) {
        general_help();
    } else if (!strcmp(tokens.storage[1], "exit")) {
        printf("help for 'exit': \n");
        printf("\t- the builtin command 'exit' quits the shell\n");
        printf("\t- a shorter, temporary version is available as 'x'\n");
    } else if (!strcmp(tokens.storage[1], "cd")) {
        printf("help for 'cd': \n");
        printf("\t- syntax: cd [directory]\n");
        printf("\t- not specifying a directory will cd to ~\n");
        printf("\t- cd allows use of globs\n");
        printf("\t- cd will automatically call '@ls_command' after changing directory\n");
        printf("\t\t- to disable, run with -nl | --nolsaftercd or set ls_after_cd? to 0\n");
        printf("\t\t- to change the ls command that is called, alter @ls_command\n");
    } else if (!strcmp(tokens.storage[1], "get")) {
        printf("help for 'get': \n");
        printf("\t- syntax: get [variable]\n");
        printf("\t- environment variables usually begin with '@'\n");
        printf("\t- type 'get -all' to see all variables and settings\n");
    } else if (!strcmp(tokens.storage[1], "set")) {
        printf("help for 'set': \n");
        printf("\t- syntax: set [variable] [value]\n");
        printf("\t- if [variable] exists, its value will be changed\n");
        printf("\t- if [variable] does not exist, it is created\n");
        printf("\t- see 'get -all' for the current included variables\n");
    } else if (!strcmp(tokens.storage[1], "redirect")) {
        printf("help for 'redirect': \n");
        printf("\t- to redirect stdin from a file: \n");
        printf("\t\t- syntax: [command] < [file]\n");
        printf("\t\t- the space between < and [file] is optional\n");
        printf("\t- to redirect stdout to a file: \n");
        printf("\t\t- syntax: [command] > [file]\n");
        printf("\t\t- again, the space is optional\n");
        printf("\t- to redirect stdout to append a file: \n");
        printf("\t\t- syntax: [command] >> [file]\n");
        printf("\t\t- the space is option between >> and [file]\n");
    } else if (!strcmp(tokens.storage[1], "prompt")) {
        printf("help for 'prompt': \n");
        printf("\t- the @prompt variable allows a number of macros\n");
        printf("\t\t- @pwd@ will expand to the current working directory\n");
        printf("\t\t- @date[FORMAT]@ will expand to the date\n");
        printf("\t\t- @user@ will expand to the username\n");
        printf("\t\t- @userid@ will expand to the user's id\n");
        printf("\t\t- @groupid@ will expand to the group id\n");
    } else if (!strcmp(tokens.storage[1], "alias")) {
        printf("help for 'alias': \n");
        printf("\t- syntax: alias [ALIAS] [COMMAND]\n");
        printf("\t- aliases work as in bash, allowing you to set your own\n");
        printf("\t  names for commands.\n");
        printf("\t- aliases cannot contain spaces\n");
        printf("\t- commands must be surrounded by quotes\n");
    } else if (!strcmp(tokens.storage[1], "config")) {
        printf("help for 'config': \n");
        printf("\t- configuration files can execute any shell commands\n");
        printf("\t- they are mainly used to set settings, variables, and\n");
        printf("\t  aliases at startup\n");
        printf("\t- the config file to load on startup is stored in @config\n");
    } else if (!strcmp(tokens.storage[1], "script")) {
        printf("help for 'script': \n");
        printf("\t- syntax: source [FILE 1] [FILE 2] ... [FILE n]\n");
        printf("\t- any shell command can be included, but advanced\n");
        printf("\t  features like loops and conditionals are not supported\n");
        printf("\t- scripts can also be ran with command line options: \n");
        printf("\t\t- syntax: '-s' || '--script' [FILE]\n");
        printf("\t- the shell will exit afterwards, unless you use this option: \n");
        printf("\t\t- '-ne' or '--noexit'\n");
    } else if (!strcmp(tokens.storage[1], "output")) {
        printf("help for 'output': \n");
        printf("\t- to turn off shell messages, use '-q' or '--quiet'\n");
        printf("\t- to turn on shell messages, use '-v' or '--verbose'\n");
        printf("\t- shell messages are turned on by default, but the '--verbose'\n");
        printf("\t  option can be useful if the shell is compiled with messages off\n");
        printf("\t- this only applies to messages from the shell, not your commands\n");
    } else if (!strcmp(tokens.storage[1], "builtins")) {
        printf("help for 'builtins': \n");
        printf("\t- list of builtin commands: \n");
        printf("\t\t- exit || x\n");
        printf("\t\t- cd\n");
        printf("\t\t- get\n");
        printf("\t\t- set\n");
        printf("\t\t- alias\n");
        printf("\t\t- help\n");
        printf("\t\t- source\n");
        printf("\t\t- history\n");
    } else if (!strcmp(tokens.storage[1], "exec")) {
        printf("help for 'exec': \n");
        printf("\t- you can execute single commands with these command line options:\n");
        printf("\t\t- syntax: '-e' || '--execute' [COMMAND]\n");
        printf("\t\t- if [COMMAND] contains spaces, it must be quoted\n");
        printf("\t- the shell will exit afterwards, unless you use this option: \n");
        printf("\t\t- '-ne' or '--noexit'\n");
    } else if (!strcmp(tokens.storage[1], "glob")) {
        printf("help for 'glob': \n");
        printf("\t- globbing enables you to use wildcard expressions with commands\n");
        printf("\t- all normal wildcards are allowed (i.e., * and ?)\n");
    } else if (!strcmp(tokens.storage[1], "pipes")) {
        printf("help for 'pipes': \n");
        printf("\t- pipes are under construction for unix\n");
    } else {
        error("help", "topic not found");
        general_help();
    }    
}
