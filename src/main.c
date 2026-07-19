#include <stdio.h>
#include "history.h"
#include "util.h"

//------------------ error codes ------------------
//  2   -   couldn't open history file
//-------------------------------------------------

int main(void) {
    char current_line[512];

    FILE *historyfile = open_history();
    if (historyfile == NULL) {
        printf("couldn't open ur history file, only bash and zsh for now :(\n");
        newline(2);
        return 2;
    }

    while (fgets(current_line, sizeof(current_line), historyfile) != NULL) {
        printf("%s", current_line);
    }

    fclose(historyfile);
    return 0;
}