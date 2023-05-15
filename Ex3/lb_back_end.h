#ifndef LB_BACK_END_H
#define LB_BACK_END_H



int bind_and_listen(int *sockfd);
int wait_for_connection(int sockfd);

int write_port_to_file(int number, char* file_name);
void run_load_balancer(int* sockfd_servers, int sockfd_client, int client_port);


#endif
