#ifndef GLOB_H
#define GLOB_H

#include <glob.h>
#include "parser.h"

/*  Checks the array to see if
 *  any globbing is used.  
 *  Returns 1 if yes, 0 if no.
 */
int check_glob(PARSED tokens);

/*  Sets up the glob_t object with
 *  the proper command, glob, and 
 *  arguments.
 *  Returns this object.
 */
glob_t setup_glob(PARSED tokens);

#endif
