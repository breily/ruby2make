#ifndef SETTINGS_H
#define SETTINGS_H

/*  Checks to see if *arg is either the short option
 *  or the long option.
 */   
int match_opts(char *arg, char *short_opt, char *long_opt);

/*  Parses command line settings.
 */
void parse_opts(int argc, char **argv);

#endif
