#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/history.h>

#include "prompt.h"
#include "environ.h"

void eval_macro(char *macro, char *prompt) {
    if (!strcmp(macro, "pwd")) { //  {pwd} macro
        char *cwd;
        char *ptr;
        long size = pathconf(".", _PC_PATH_MAX);
        if ((cwd = (char *) malloc((size_t) size)) != NULL) {
            ptr = getcwd(cwd, (size_t) size);
            free(cwd);
        } else {
            ptr = "/somewhere?";
        }
        if (!strncmp(ptr, getenv("HOME"), strlen(getenv("HOME")))) {
            strcat(prompt, "~");
            strcat(prompt, &ptr[strlen(getenv("HOME"))]);
            return;
        }
        strcat(prompt, ptr);
    } else if (!strncmp("date", macro, 4)) { //  {date} macro (see 'man date')
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
    } else if (!strcmp(macro, "user")) { // {user} macro
        char *data = getlogin();
        strcat(prompt, data);
    } else if (!strcmp(macro, "histlen")) { // {histlen} macro
        char data[4];
        sprintf(data, "%d", history_length);
        strcat(prompt, data);
    } else {
        strcat(prompt, "{");
        strcat(prompt, macro);
        strcat(prompt, "}");
    }
}

char *eval_prompt(char *prompt) {
    sub_env(prompt);
    // Flag to indicate inside {...} block
    int in_macro = 0;
    // Counters to track lengths
    int prompt_len = strlen(prompt);
    // Buffers
    char macro[prompt_len];
    *macro -= *macro;
    char *new_prompt = calloc(1, 128);
    *new_prompt -= *new_prompt;
    int i = 0;
    while (i < prompt_len) {
        if (prompt[i] == '\\' && prompt[i - 1] != '\\') {
            i++;
            continue;
        }   
        if (prompt[i] == '{' && prompt[i - 1] != '\\') {
            in_macro = 1;
        } else if (prompt[i] == '}' && prompt[i - 1] != '\\') {
            if (in_macro) {
                in_macro = 0;
                eval_macro(macro, new_prompt);
                strcpy(macro, "");
            } else {
                strncat(new_prompt, &prompt[i], 1);
            }
        } else {
            if (in_macro) {
                strncat(macro, &prompt[i], 1);
            } else {
                strncat(new_prompt, &prompt[i], 1);
            }
        }
        i++;
    }
    if (in_macro && strlen(macro)) {
        eval_macro(macro, new_prompt);
    }
    char *copy = calloc(1, strlen(new_prompt) + 1);
    strcpy(copy, new_prompt);
    free(new_prompt);
    return copy;
}
