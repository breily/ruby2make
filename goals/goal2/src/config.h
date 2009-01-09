/*
 *  This file contains all the settings that you can change before
 *  compilation.
 */

#ifndef CONFIG_H
#define CONFIG_H

// this file is loaded each time at startup
#define CONFIG_FILE       "~/.slash_config"

#define START_MESSAGE     "welcome to slash"
#define EXIT_MESSAGE      "thanks for using slash"

// your default prompt
#define PROMPT            "$ "
// #define PROMPT           "{pwd}$ "
// #define PROMPT           "{date%H:%M:%S}$ "
// #define PROMPT           "{histlen}$ "
// #define PROMPT           "hello, {user}$ "

#define VERSION           "0.3"
#define AUTHOR            "brian reily"

// display start and end messages
#define VERBOSE           "1"
// display error messages
#define ERROR_MESSAGES    "1"
// enable wildcards
#define GLOB              "1"
// load a config file
#define CONFIG            "1"
// use aliases
#define ALIASES           "1"

// run 'ls' after each 'cd'
#define LS_AFTER_CD       "1"
// which 'ls' to run - can be any command
#define LS_COMMAND        "ls"

// enable history
#define HISTORY           "1"
// using the same command multiple times in a row will
// only add one entry to history
#define NO_HISTORY_REPEAT "1"
// store history in a file
#define USE_HISTORY_FILE  "1"
// where to store it
#define HISTORY_FILE      "~/.slash_history"

#endif
