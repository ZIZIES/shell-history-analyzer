#include "entry.h"
#include <string.h>
#include <stdlib.h>

struct Entry *add_cmd(struct Entry *head, char *cmd) {
    struct Entry *current = head;
    while (current != NULL) {
        if (strcmp(current->cmd, cmd) == 0) {
            current->count++;
            return head;
        }
        current = current->next;
    }
    struct Entry *node = malloc(sizeof(struct Entry));
    strncpy(node->cmd, cmd, 63);
    node->cmd[63] = '\0';
    node->count = 1;
    node->next = head;
    return node;
}

void swap_entries(struct Entry *a, struct Entry *b) {
    char tmp2[64];

    int tmp = a->count;
    a->count = b->count;
    b->count = tmp;

    strcpy(tmp2, a->cmd);
    strcpy(a->cmd, b->cmd);
    strcpy(b->cmd, tmp2);
}

void sort_list(struct Entry *head) {
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        struct Entry *current = head;
        while (current != NULL && current->next != NULL) {
            if (current->count < current->next->count) {
                swap_entries(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
    }
}

void free_list(struct Entry *head) {
struct Entry *current = head;
while (current != NULL) {
struct Entry *next = current->next;
free(current);
current = next;
    }
}