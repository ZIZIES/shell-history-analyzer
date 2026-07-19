#include "history.h"
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
static FILE *open_from_home(const char *home, const char *relative_path) {
    char history_path[512];
    int written = snprintf(history_path, sizeof(history_path), "%s%s", home, relative_path);

    if (written < 0 || (size_t)written >= sizeof(history_path)) return NULL;
    return fopen(history_path, "r");
}
#endif

FILE *open_history(const char *custom_path) {
    if (custom_path != NULL) return fopen(custom_path, "r");

#ifdef _WIN32
    char *appdata = getenv("APPDATA");
    if (appdata == NULL) return NULL;

    // PowerShell 7 and later.
    FILE *history = open_from_home(
        appdata,
        "\\Microsoft\\PowerShell\\PSReadLine\\ConsoleHost_history.txt"
    );
    if (history != NULL) return history;

    // Windows PowerShell 5.1.
    return open_from_home(
        appdata,
        "\\Microsoft\\Windows\\PowerShell\\PSReadLine\\ConsoleHost_history.txt"
    );
#else
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
#endif
}
