#ifndef FREE_LIST_H
#define FREE_LIST_H

typedef struct node {
    int data;
    struct node* next;
} Node;
void free_list(Node* head);

#endif