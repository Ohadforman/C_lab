#include "lb_back_end.h"
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOCAL_HOST "127.0.0.1"
#define PORT_MAX 64000
#define PORT_MIN 1024
#define CHUNK_SIZE 1024
#define DOUBLE_CRLF "\r\n\r\n"
#define DOUBLE_CRLF_LEN 4
#define BUFFER_RESIZE_RATIO 2

int bind_and_listen(int* sockfd)
{
  int port;
  struct sockaddr_in server_addr;

  // Create socket
  *sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (*sockfd < 0) {
    perror("socket");
    return -1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);

  // Bind to random port
  while (true) {
    port = rand() % (PORT_MAX - PORT_MIN + 1) + PORT_MIN;
    server_addr.sin_port = htons(port);
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    if (bind(*sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
      if (listen(*sockfd, NUM_SERVERS) != -1) {
        return port;
      } else {
        perror("listen");
      }
    }
  }

  return -1;
}

int write_port_to_file(int number, char* file_name)
{
  FILE* fp;
  fp = fopen(file_name, "w");
  if (fp == NULL) {
    return -1;
  }
  fprintf(fp, "%d", number);
  fclose(fp);

  return 0;
}

int wait_for_connection(int sockfd)
{
  int client_sockfd;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  if ((client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
    perror("accept");
    return -1;
  }

  return client_sockfd;
}

void send_data_to_socket(int sockfd, char** buffer, int buffer_length)
{
  int bytes_sent = 0;
  int total_bytes_sent = 0;

  while (total_bytes_sent < buffer_length) {
    bytes_sent = send(sockfd, *buffer + total_bytes_sent, buffer_length - total_bytes_sent, 0);
    if (bytes_sent < 0) {
      perror("send");
      return;
    }
    total_bytes_sent += bytes_sent;
  }
}

int count_double_crlf(char* buffer)
{
  int count = 0;
  char* tmp = buffer;
  while ((tmp = strstr(tmp, DOUBLE_CRLF))) {
    count++;
    tmp += DOUBLE_CRLF_LEN;
  }

  return count;
}

int recieve_data_on_socket(int sockfd, char** buffer, int num_of_http_sep)
{
  int bytes_received = 0;
  int total_bytes_read = 0;
  int double_crlf_count = 0;
  int buffer_size = CHUNK_SIZE;

  while (true) {
    if (buffer_size == total_bytes_read) {
      *buffer = (char*)realloc(*buffer, buffer_size * BUFFER_RESIZE_RATIO * sizeof(char));
      if (*buffer == NULL) {
        perror("Memory allocation failed.\n");
        return -1;
      }
      memset(*buffer + buffer_size, 0, buffer_size * sizeof(char));
      buffer_size *= BUFFER_RESIZE_RATIO;
    }

    bytes_received = recv(sockfd, *buffer + total_bytes_read, buffer_size - total_bytes_read, 0);
    total_bytes_read += bytes_received;
    double_crlf_count = count_double_crlf(*buffer);
    if (double_crlf_count == num_of_http_sep) {
      break;
    }
  }
  return total_bytes_read;
}

void handle_client(int client_sockfd, int server_sockfd)
{
  int message_length;
  char* request_buffer = (char*)malloc(CHUNK_SIZE * sizeof(char));
  char* response_buffer = (char*)malloc(CHUNK_SIZE * sizeof(char));

  // Check that malloc worked
  if (request_buffer == NULL || response_buffer == NULL) {
    perror("Memory allocation failed.\n");
    return;
  }

  // Initialize the memory to zero using memset
  memset(request_buffer, 0, CHUNK_SIZE * sizeof(char));
  memset(response_buffer, 0, CHUNK_SIZE * sizeof(char));

  // Forward client's request to given server
  message_length = recieve_data_on_socket(client_sockfd, &request_buffer, 1);
  send_data_to_socket(server_sockfd, &request_buffer, message_length);

  // Forward server's response to client
  message_length = recieve_data_on_socket(server_sockfd, &response_buffer, 2);
  send_data_to_socket(client_sockfd, &response_buffer, message_length);

  free(request_buffer);
  free(response_buffer);
}

void run_load_balancer(int* sockfd_servers, int sockfd_client)
{
  int server_num = 0;  // Keeps track of the current server
  while (true) {
    int client_sockfd = wait_for_connection(sockfd_client);

    handle_client(client_sockfd, sockfd_servers[server_num]);
    server_num = (server_num + 1) % NUM_SERVERS;  // Move on to the next server
  }
}
