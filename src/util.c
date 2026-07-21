#include "util.h"
#include <stdio.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

void newline(int times) {
    for (int i = 0; i < times; i++) {
        printf("\n");
    }
}

int stdout_is_tty(void) {
#ifdef _WIN32
    return _isatty(_fileno(stdout));
#else
    return isatty(fileno(stdout));
#endif
}
