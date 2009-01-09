#ifndef INIT_H
#define INIT_H

/*  Initializes settings and parses command 
 *  line options.
 */
void init_settings(int argc, char **argv);

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
