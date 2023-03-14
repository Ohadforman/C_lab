#include <stdio.h>
#include <stdlib.h>
#include "linked_list_actions.h"

struct Node *create_node(int data) {
    struct Node *new_node = malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void add_to_start(struct Node **head, int data) {
    struct Node *new_node = create_node(data);
    new_node->next = *head;
    *head = new_node;
}

void add_after(struct Node *head, int existing_data, int data) {
    struct Node *current = head;
    while (current != NULL && current->data != existing_data) {
        current = current->next;
    }
    if (current == NULL) {
        printf("Error: Existing data not found in list.\n");
        exit(1);
    }
    struct Node *new_node = create_node(data);
    new_node->next = current->next;
    current->next = new_node;
}

void print_list(struct Node *head) {
    printf("[");
    struct Node *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}


void free_list(struct Node **head) {
    struct Node *current = *head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

int find_index(struct Node *head, int data) {
    int index = 0;
    struct Node *current = head;
    while (current != NULL) {
        if (current->data == data) {
            return index;
        }
        index++;
        current = current->next;
    }
    return -1;
}

void delete_at_index(struct Node **head, int index) {
    if (*head == NULL) {
        printf("Error: List is empty.\n");
        exit(1);
    }
    if (index == 0) {
        struct Node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }
    struct Node *prev = NULL;
    struct Node *current = *head;
    int i;
    for (i = 0; i < index && current != NULL; i++) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("Error: Index is out of bounds.\n");
        exit(1);
    }
    prev->next = current->next;
    free(current);
}
