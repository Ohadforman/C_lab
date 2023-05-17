#include "lb_back_end.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SERVER_1 0
#define SERVER_2 1
#define SERVER_3 2

int main()
{
  int sockfd_server, sockfd_client;
  int server_port, client_port;
  server_port = bind_and_listen(&sockfd_server);
  client_port = bind_and_listen(&sockfd_client);

  // Save the ports to files
  write_port_to_file(server_port, "server_port");
  write_port_to_file(client_port, "http_port");

  int sockfd_servers[NUM_SERVERS];
  sockfd_servers[SERVER_1] = wait_for_connection(sockfd_server);
  sockfd_servers[SERVER_2] = wait_for_connection(sockfd_server);
  sockfd_servers[SERVER_3] = wait_for_connection(sockfd_server);
  

  run_load_balancer(sockfd_servers, sockfd_client, client_port);

  return 0;
}
