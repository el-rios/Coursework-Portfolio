/*****************************************************************************
 * client-c.c                                                                 
 * Name: Nicolas Rios
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define SEND_BUFFER_SIZE 2048


/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/
int client(char *server_ip, char *server_port) {

    // Creating a Socket
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[SEND_BUFFER_SIZE];
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("Error in connection.\n");
        exit(1);
    }
    printf("Client Socket is created.\n");

    //Defining Connection Address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(*server_port);
    server_address.sin_addr.s_addr = *server_ip;

    //Connecting to Server
    int connection_status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    //Checking for Connection Error
    if (connection_status < 0)
    {
        perror("ERROR: Not able to connect to remote socket \n\n");
        exit(1);
    }
    printf("Conected to Server.\n");

    while (1)
    {
        printf("Client: \t");
        scanf("%2048s", &buffer[0]);
        send(client_socket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, ":exit") == 0)
        {
            printf("Disconected from server.\n");
            exit(1);
        }

        if (recv(client_socket, buffer, SEND_BUFFER_SIZE, 0) < 0)
        {
            perror("Error in receiving data.\n");
        }
        else
        {
            printf("Server: \t%s\n", buffer);
        }
    }

    return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv) {
  char *server_ip;
  char *server_port;

  if (argc != 3) {
    fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
    exit(EXIT_FAILURE);
  }

  server_ip = argv[1];
  server_port = argv[2];
  return client(server_ip, server_port);
}
