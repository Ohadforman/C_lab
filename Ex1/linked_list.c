#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void *malloc(size_t size);

struct Node *create_node(int value) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

void add_to_end(LinkedList *list, int value){
    Node *new_node = create_node(value);
    if ( list->tail == NULL ){ //The list is empty
        list->head = new_node;
        list->tail = new_node;
    }
    else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
}

void add_to_start(LinkedList *list, int value) {
    Node *new_node = create_node(value);
    if ( list->head == NULL ){ //The list is empty
        list->head = new_node;
        list->tail = new_node;
    }
    else {
        new_node->next = list->head;
        list->head = new_node;
    }
}

void add_after(LinkedList *list, int val_to_insert, int val_to_search) {
    Node *current = list->head;
    while ( ( current != NULL ) && ( current->data != val_to_search ) ) {
        current = current->next;
    }
    if ( current == NULL ) {
        printf("Error: Given value %d not found in list.\n",val_to_search);
        exit(1);
    }
    Node *new_node = create_node(val_to_insert);
    new_node->next = current->next;
    current->next = new_node;
}

void find_index(LinkedList *list, int value) {
    int index = 0;
    Node *current = list->head;
    while ( current != NULL ) {
        if ( current->data == value ) {
            printf("%d\n",index);
            return;
        }
        index++;
        current = current->next;
    }
    printf("-1\n");
}

void delete_at_index(LinkedList *list, int index) {
    Node *head = list->head;
    
    if ( head == NULL ) {
        printf("Error: List is empty.\n");
        exit(1);
    }

    if ( index == 0 ) {
        if ( head->next == NULL ){ //List contains one node
            list->head = NULL;
            list->tail = NULL;
            free(head);
        }
        else {
            list->head = head->next;
            free(head);
        }
        return;
    }

    Node *prev = NULL;
    Node *current = head;
    int i;
    for (i = 0; i < index && current != NULL; i++) {
        prev = current;
        current = current->next;
    }
    if ( current == NULL ) {
        printf("Error: Index is out of bounds.\n");
        exit(1);
    }

    if ( list->tail == current ){
        list->tail = prev;
    }
    prev->next = current->next;
    free(current);
}

void print_list(LinkedList *list) {
    printf("[");
    Node *current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}

void free_list(LinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}




