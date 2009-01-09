#ifndef INIT_H
#define INIT_H

/*  Initializes settings.
 */
void init_settings();

/*  Calls parse_opts() to parse command line settings.
 */
void init_options(int argc, char **argv);

/*
 *  Initializes history library and reads history file.
 */
void init_history();
/*  Handles execution of a single command.
 */
void init_execute();

/*  Handles execution of a script.
 */
void init_script();

/*  Handles printing the startup message.
 */
void init_msg();

/*  Handles the loading of the config file.
 */
void init_config();

#endif
