#ifndef BG_H
#define BG_H

#include "parser.h"

/*
 *  Checks for a background task.  If necessary, removes '&'.
 *  Returns new token count;
 */
int check_bg(PARSED tokens);

#endif
