#ifndef LINKED_LIST_ACTIONS_H
#define LINKED_LIST_ACTIONS_H

struct Node {
    int data;
    struct Node *next;
};

struct Node *create_node(int data);
void add_to_start(struct Node **head, int data);
void add_after(struct Node *head, int existing_data, int data);
void print_list(struct Node *head);
void free_list(struct Node **head);
int find_index(struct Node *head, int data);
void delete_at_index(struct Node **head, int index);
 
#endif
