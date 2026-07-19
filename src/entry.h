#ifndef ENTRY_H
#define ENTRY_H

struct Entry {
    char cmd[64];
    int count;
    struct Entry *next;
};

struct Entry *add_cmd(struct Entry *head, char *cmd);

void swap_entries(struct Entry *a, struct Entry *b);

void sort_list(struct Entry *head);

void sort_list(struct Entry *head);

void free_list(struct Entry *head);

#endif