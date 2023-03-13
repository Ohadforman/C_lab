#include <stdio.h>
#include <stdlib.h>
#include "free_list.h"

void free_list(Node* head){
    Node* current = head;
    while (current != NULL){
        Node* next = current->next;
        free(current);
        current = next;
    }
}