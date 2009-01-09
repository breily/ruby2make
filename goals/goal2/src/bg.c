#include <string.h>
#include <stdlib.h>

#include "bg.h"
#include "parser.h"
#include "environ.h"

// There's probably no need to have this lone function in
// its own file, but I like to separate/organize things.
//
// Basically checks for an '&' as the last token - this would
// indicate a background task.
int check_bg(PARSED tokens) {
    if (!strcmp(tokens.storage[tokens.count - 1], "&")) {
        // Frees the array index holding '&'
        free(tokens.storage[tokens.count - 1]);
        set_setting("@background?", 1);
        // We have one less token now
        return (tokens.count - 1);
    } else set_setting("@background?", 0);
    return tokens.count;
}
