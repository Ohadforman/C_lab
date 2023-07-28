#include "lb_back_end.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  int sockfd_server, sockfd_client;
  int server_port, client_port;
  int sockfd_servers[NUM_SERVERS];

  // Bind and listen on server and client ports
  srand(time(NULL));
  server_port = bind_and_listen(&sockfd_server);
  client_port = bind_and_listen(&sockfd_client);

  // Save the assigned ports to files
  write_port_to_file(server_port, "server_port");
  write_port_to_file(client_port, "http_port");

  // Waits and accepts incoming connections
  for (int i = 0; i < NUM_SERVERS; i++) {
    sockfd_servers[i] = wait_for_connection(sockfd_server);
  }

  run_load_balancer(sockfd_servers, sockfd_client);

  return 0;
}