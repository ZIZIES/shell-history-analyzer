#include "history.h"
#include <stdlib.h>
#include <string.h>

FILE *open_history(void) {
    char history_path[512];

    char *home = getenv("HOME");
    if (home == NULL) return NULL;

    char *shell = getenv("SHELL");
    if (shell == NULL) return NULL;

    if (strstr(shell, "zsh")) {
        snprintf(history_path, sizeof(history_path), "%s/.zsh_history", home);
    } else if (strstr(shell, "bash")) {
        snprintf(history_path, sizeof(history_path), "%s/.bash_history", home);
    } else {
        return NULL;
    }

    return fopen(history_path, "r");
}