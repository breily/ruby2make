#ifndef EVAL_H
#define EVAL_H

#include "parser.h"

/*  Evaluates the given command; command
 *  is one line and eval takes care of all 
 *  parsing/etc.
 */
void eval(char *command);

/*
 *  Take care of globbing and exec.
 */
void exec_process(PARSED tokens, int in_file, int out_file);

#endif
