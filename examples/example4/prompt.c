#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "prompt.h"

void eval_macro(char *macro, char *prompt) {
    if (!strcmp(macro, "pwd")) { //  @pwd@ macro
        char *cwd;
        char *ptr;
        long size = pathconf(".", _PC_PATH_MAX);
        if ((cwd = (char *) malloc((size_t) size)) != NULL) {
            ptr = getcwd(cwd, (size_t) size);
        } else {
            ptr = "/somewhere?";
        }
        if (!strcmp(ptr, getenv("HOME"))) {
            ptr = "~";
        }
        strcat(prompt, ptr);
    } else if (!strncmp("date", macro, 4)) { //  @date@ macro
     /*
     *      %a -- short day of week, %A -- long day of week
     *      %b -- short month, %B -- long month
     *      %m -- month number, %e -- day of month
     *      %H -- hour, %M -- minutes, %S -- seconds
     *      %Z -- time zone
     *      %Y -- four digit year, %y -- two digit year
     *      probably more...
     */
        char format[strlen(macro) - 4];
        strcpy(format, "date +'");
        strcat(format, &macro[4]);
        strcat(format, "'");

        FILE *fp = popen(format, "r");
        if (fp != NULL) {
            char data[256];
            while (fgets(data, sizeof data, fp)) {}
            pclose(fp);
            strncat(prompt, data, strlen(data) - 1);
        }
    } else if (!strcmp(macro, "user")) { // @user@ macro
        char *data = getlogin();
        strcat(prompt, data);
    } else if (!strcmp(macro, "userid")) { // @userid@ macro
        char *data = calloc(1, sizeof (char *));
        sprintf(data, "%d", getuid());
        strcat(prompt, data);
        free(data);
    } else if (!strcmp(macro, "groupid")) { // @groupid@ macro
        char *data = calloc(1, sizeof (char *));
        sprintf(data, "%d", getgid());
        strcat(prompt, data);
        free(data);
    }
}

void print_prompt(char *prompt) {
    // Flag to indicate inside @...@ block
    int in_macro = 0;
    // Counters to track lengths
    int prompt_len = strlen(prompt);
    int macro_len = 0;
    int new_prompt_len = 0;
    // Buffers
    char macro[prompt_len];
    char new_prompt[prompt_len];
    strcpy(macro, "");
    strcpy(new_prompt, "");
    // Loop counter
    int i = 0;
    while (i < prompt_len) {
        // Either start or end a macro
        if (prompt[i] == '@') {
            // If start, set flag
            if (!in_macro) in_macro = 1;
            // If end, ...
            else {
                // Unset flag
                in_macro = 0;
                // Evaluate the macro (substitute it in)
                eval_macro(macro, new_prompt);
                // Reset macro length and buffer
                strcpy(macro, "");
            }
        // If not '@', either add to macro buffer...    
        } else if (in_macro) {
            strncat(macro, &prompt[i], 1);
        // or just add current character to new prompt    
        } else {
            strncat(new_prompt, &prompt[i], 1);
        }
        i++;
    }
    printf("%s", new_prompt);
}
