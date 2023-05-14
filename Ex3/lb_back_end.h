#ifndef LB_BACK_END_H
#define LB_BACK_END_H

#define BUFFER_SIZE 1024

int bind_and_listen(int *sockfd);
int wait_for_connection(int sockfd);
int send_message(int sockfd, const char* message);
int write_port_to_file(int number, char* file_name);

#endif
