#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"

#define MAX_INPUT_LINE_LENGTH (100)

int execute_command(LinkedList *linked_list, char* command, int first_value, int seconde_value);

int execute_command(LinkedList *linked_list, char* command, int first_value, int seconde_value){
    if ( strcmp(command, "exit") == 0 ){
        free_list(linked_list);
        return true; //Signal main to finish the run
    }  
    if ( strcmp(command, "add_end") == 0 ){
        add_to_end(linked_list, first_value);
    }
    if ( strcmp(command, "add_start") == 0 ){
        add_to_start(linked_list, first_value);
    }
    if ( strcmp(command, "add_after") == 0 ){
        add_after(linked_list, first_value, seconde_value);
    }
    if ( strcmp(command, "index") == 0 ){
        find_index(linked_list, first_value);
    }
    if ( strcmp(command, "del") == 0 ){
        delete_at_index(linked_list, first_value);
    }
    if ( strcmp(command, "print") == 0 ){
        print_list(linked_list);
    }
    
    return false; //We got a regular command
}

int main() {
    int exit = false;
    char line[MAX_INPUT_LINE_LENGTH];
    char command[MAX_INPUT_LINE_LENGTH];
    int first_value = 0;
    int second_value = 0;

    //Initialize the list
    LinkedList *linked_list = malloc(sizeof(struct LinkedList));
    linked_list->head = NULL;
    linked_list->tail = NULL;

    while (!exit){
        fgets(line, sizeof(line), stdin); //Wait for input line from the user
        sscanf(line, "%s %d %d", command, &first_value, &second_value); //Split the input line
        
        //Convert the command to lowercase
        for(int i = 0; command[i]; i++){
            command[i] = tolower(command[i]);
        }

        exit = execute_command(linked_list,command,first_value,second_value);
    }
}