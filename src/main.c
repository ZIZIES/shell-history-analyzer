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

// coldest (least used) to hottest (most used)
static const char *HEAT_COLORS[] = {
    "\033[34m", "\033[36m", "\033[32m", "\033[33m", "\033[31m",
};
#define HEAT_STEPS (int)(sizeof(HEAT_COLORS) / sizeof(HEAT_COLORS[0]))
#define COLOR_RESET "\033[0m"

static const char *heat_color(int count, int max_count) {
    int step = count * HEAT_STEPS / (max_count + 1);
    if (step >= HEAT_STEPS) step = HEAT_STEPS - 1;
    return HEAT_COLORS[step];
}

void print_help(void) {
printf("shis - shell history analyzer\n\n");
printf("usage: shis [options]\n\n");
printf("  -c <n>        show top n commands (default 20)\n");
printf("  -f, --full    show every command, no cap\n");
printf("  -i, --history <path>\n");
printf("                analyze a specific history file\n");
printf("  -h, --help    show this\n\n");
}

int main(int argc, char *argv[]) {
char current_line[512];
int rank = 0;
int limit = 20;
const char *history_path = NULL;

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
    } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--history") == 0) {
if (i + 1 >= argc) {
printf("%s needs a file path\n", argv[i]);
return 1;
        }
history_path = argv[i + 1];
i++;
    } else {
printf("dunno what '%s' is, try -h\n", argv[i]);
return 1;
    }
}

FILE *historyfile = open_history(history_path);
if (historyfile == NULL) {
printf("couldn't open a supported history file; try --history <path>\n");
newline(2);
return 2;
    }

struct Entry *head = NULL;

while (fgets(current_line, sizeof(current_line), historyfile) != NULL) {
char *cmd = strtok(current_line, " \t\r\n");
if (cmd == NULL) continue;
if (cmd[0] == '-') continue;
head = add_cmd(head, cmd);
    }

sort_list(head);

int no_color_env = getenv("NO_COLOR") != NULL;
int use_color = head != NULL && stdout_is_tty() && !no_color_env;
if (head != NULL && !use_color && !no_color_env) {
fprintf(stderr, "note: output isn't a terminal, dropping the colors\n");
    }
int max_count = head != NULL ? head->count : 0;

struct Entry *current = head;
while (current != NULL && (limit == -1 || rank < limit)) {
const char *color = use_color ? heat_color(current->count, max_count) : "";
const char *reset = use_color ? COLOR_RESET : "";
printf("%-20.20s %s", current->cmd, color);
for (int i = 0; i < current->count / 5; i++) printf("█");
printf("%s %d\n", reset, current->count);
current = current->next;
rank++;
    }

fclose(historyfile);
free_list(head);
return 0;
}
