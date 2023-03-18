#include <stdio.h>
#include "linked_list.h"

#define TRUE    (1)
#define FALSE   (0)
#define INPUT_LINE_LENGTH 100

void *malloc(size_t size);
char *strtok(char *str, const char *delim);
int atoi(const char *str);
char* strcpy(char* destination, const char* source);
int strcmp (const char* str1, const char* str2);
int tolower(int argument);

int execute_command(LinkedList *linked_list, char* command, int first_value, int seconde_value);


int execute_command(LinkedList *linked_list, char* command, int first_value, int seconde_value){
    if ( strcmp(command, "exit") == 0 ){
        free_list(linked_list);
        return TRUE; //Signal main to finish the run
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
    
    return FALSE; //We got a regular command
}

void main() {
    int exit = FALSE;
    char line[INPUT_LINE_LENGTH];
    char command[INPUT_LINE_LENGTH];
    int first_value = 0;
    int second_value = 0;

    //Initialize the list
    LinkedList *linked_list = malloc(sizeof(struct LinkedList));
    linked_list->head = NULL;
    linked_list->tail = NULL;

    while (!exit){
        fgets(line, sizeof(line), stdin); //Wait for input line from the user
        sscanf(line, "%s %d %d", &command, &first_value, &second_value); //Split the input line
        
        //Convert the command to lowercase
        for(int i = 0; command[i]; i++){
            command[i] = tolower(command[i]);
        }

        exit = execute_command(linked_list,command,first_value,second_value);
    }
}