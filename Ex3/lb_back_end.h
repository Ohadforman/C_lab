#ifndef LB_BACK_END_H
#define LB_BACK_END_H

#define BUFFER_SIZE 1024
#define PORT_MAX 64000
#define PORT_MIN 1024
#define MAX_NUM_CLIENTS 9
#define NUM_SERVERS 3

int bind_and_listen(int *sockfd);
int wait_for_connection(int sockfd);
int send_message(int sockfd, const char* message, int server_num, int* server_counters);
void handle_client(int client_sockfd, int server_sockfd, int server_num, int* server_counters);
int write_port_to_file(int number, char* file_name);
void run_load_balancer(int* sockfd_servers, int sockfd_client, int client_port);


#endif
