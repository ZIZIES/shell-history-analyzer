#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "history.h"
#include "util.h"
#include "entry.h"

//------------------ error codes ------------------
//  1   -   bad arguments
//  2   -   couldn't open history file
//-------------------------------------------------

void print_help(void) {
printf("shis - shell history analyzer\n\n");
printf("usage: shis [options]\n\n");
printf("  -c <n>        show top n commands (default 20)\n");
printf("  -f, --full    show every command, no cap\n");
printf("  -h, --help    show this\n\n");
}

int main(int argc, char *argv[]) {
char current_line[512];
int rank = 0;
int limit = 20;

for (int i = 1; i < argc; i++) {
if (strcmp(argv[i], "--full") == 0 || strcmp(argv[i], "-f") == 0) {
limit = -1;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
print_help();
return 0;
    } else if (strcmp(argv[i], "-c") == 0) {
if (i + 1 >= argc) {
printf("-c needs a number lol\n");
return 1;
        }
limit = atoi(argv[i + 1]);
i++;
    } else {
printf("dunno what '%s' is, try -h\n", argv[i]);
return 1;
    }
}

FILE *historyfile = open_history();
if (historyfile == NULL) {
printf("couldn't open ur history file, only bash and zsh for now :(\n");
newline(2);
return 2;
    }

struct Entry *head = NULL;

while (fgets(current_line, sizeof(current_line), historyfile) != NULL) {
char *cmd = strtok(current_line, " \n");
if (cmd == NULL) continue;
if (cmd[0] == '-') continue;
head = add_cmd(head, cmd);
    }

sort_list(head);
struct Entry *current = head;
while (current != NULL && (limit == -1 || rank < limit)) {
printf("%-20.20s ", current->cmd);
for (int i = 0; i < current->count / 5; i++) printf("█");
printf(" %d\n", current->count);
current = current->next;
rank++;
    }

fclose(historyfile);
free_list(head);
return 0;
}