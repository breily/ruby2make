#include <glob.h>
#include <string.h>
#include <stdlib.h>

#include "globbing.h"
#include "parser.h"

int check_glob(PARSED tokens) {
    int i = 1;
    while (i < tokens.count) {
        if (strchr(tokens.storage[i], '*') != NULL) return 1;
        i++;
    }
    return 0;
}

glob_t setup_glob(PARSED tokens) {
    glob_t ret;
    ret.gl_offs = 1;
    int i = 1;
    while (i < tokens.count) {
        if (strchr(tokens.storage[i], '*') == NULL) ret.gl_offs++;
        i++;
    }
    int append_glob = 0;
    i = 1;
    while (i < tokens.count) {
        if (strchr(tokens.storage[i], '*') != NULL) {
            if (append_glob) {
                glob(tokens.storage[i], GLOB_APPEND | GLOB_MARK, NULL, &ret);
            } else {
                glob(tokens.storage[i], GLOB_DOOFFS | GLOB_MARK, NULL, &ret);
                append_glob = 1;
            }
        }
        i++;
    }
    i = 0;
    while (i < ret.gl_offs) {
        ret.gl_pathv[i] = tokens.storage[i];
        i++;
    }
    return ret;
}
