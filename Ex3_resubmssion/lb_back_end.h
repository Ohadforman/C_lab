#ifndef LB_BACK_END_H
#define LB_BACK_END_H

#define NUM_SERVERS 3

int bind_and_listen(int* sockfd);
int write_port_to_file(int number, char* file_name);
int wait_for_connection(int sockfd);
void run_load_balancer(int* sockfd_servers, int sockfd_client);

#endif
