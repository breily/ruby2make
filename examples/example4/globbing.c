#include <glob.h>
#include <string.h>
#include <stdlib.h>

#include "globbing.h"

int check_glob(char **tokens, int len) {
    while (len > 0) {
        if (strchr(tokens[len - 1], '*') != NULL) return 1;
        len--;
    }
    return 0;
}

glob_t setup_glob(char **tokens, int len) {
    glob_t ret;
    ret.gl_offs = 1;
    int i = 1;
    while (i < len) {
        if (strchr(tokens[i], '*') == NULL) ret.gl_offs++;
        i++;
    }
    int append_glob = 0;
    i = 1;
    while (i < len) {
        if (strchr(tokens[i], '*') != NULL) {
            if (append_glob) {
                glob(tokens[i], GLOB_APPEND | GLOB_MARK, NULL, &ret);
            } else {
                glob(tokens[i], GLOB_DOOFFS | GLOB_MARK, NULL, &ret);
                append_glob = 1;
            }
        }
        i++;
    }
    i = 0;
    while (i < ret.gl_offs) {
        ret.gl_pathv[i] = tokens[i];
        i++;
    }
    return ret;
}
