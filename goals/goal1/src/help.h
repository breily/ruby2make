#ifndef HELP_H
#define HELP_H

#include "parser.h"

/*  
 *  Prints the usage information for slash.
 */
void usage(char *arg); 

/*
 *  Prints help when no topic is given.
 */
void general_help();

/*  
 *  Handles the printing of help.
 */
void print_help(PARSED tokens);

#endif
