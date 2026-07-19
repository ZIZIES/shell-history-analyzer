#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "history.h"
#include "util.h"
#include "entry.h"

//------------------ error codes ------------------
//  2   -   couldn't open history file
//-------------------------------------------------






int main(void) {
    char current_line[512];
    int rank = 0;

    FILE *historyfile = open_history();
    if (historyfile == NULL) {
        printf("couldn't open ur history file, only bash and zsh for now :(\n");
        newline(2);
        return 2;
    }

    struct Entry *head = NULL;

    while (fgets(current_line, sizeof(current_line), historyfile) != NULL) {
        char *cmd = strtok(current_line, " \n");
        if (cmd != NULL) head = add_cmd(head, cmd);
    }

    sort_list(head);
    struct Entry *current = head;
    while (current != NULL && rank < 20) {
        printf("%-15s ", current->cmd);
       for (int i = 0; i < current->count / 5; i++) printf("█");
       printf(" %d\n", current->count);
       current = current->next;
     rank++;
    }

    fclose(historyfile);
    return 0;
}