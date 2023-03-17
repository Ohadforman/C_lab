#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct LinkedList {
    struct Node *head;
    struct Node *tail;
} LinkedList;

Node *create_node(int data);
void add_to_end(LinkedList *list, int value);
void add_to_start(LinkedList *list, int value);
void add_after(LinkedList *list, int val_to_insert, int val_to_search);
void find_index(LinkedList *list, int value);
void delete_at_index(LinkedList *list, int index);
void print_list(LinkedList *list);
void free_list(LinkedList *list);
 
#endif
