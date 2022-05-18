/*****************************************************************************
 * server-c.c                                                                 
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
#include <arpa/inet.h>

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
*/
int server(char *server_port) {

    //Create Server Socket
    int server_socket, con;
    struct sockaddr_in server_address;

    int c_socket;
    struct sockaddr_in c_address;

    socklen_t addr_size;

    char buffer[RECV_BUFFER_SIZE];
    pid_t childpid;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error in connection.\n");
        exit(1);
    }
    printf("Server Socket is created.\n");

    //Specify Server Address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //Binding Socket to Address
    con = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if (con < 0)
    {
        perror("Error in Binding.\n\n");
        exit(1);
    }
    printf("Bind to port %s\n", server_port);

    //Listening for Client Requests
    if (listen(server_socket, QUEUE_LENGTH) == 0)
    {
        printf("Waiting for Requests . . . .\n");
    }
    else
    {
        perror("Error in listening.\n");
    }

    //Accepting Client Connections
    while (1)
    {
        c_socket = accept(server_socket, (struct sockaddr*) & c_address, &addr_size);
        if (c_socket < 0)
        {
            exit(1);
        }
        printf("Conection accepted from %s%d\n", inet_ntoa(c_address.sin_addr), ntohs(c_address.sin_port));

        //Creating Child Process
        if ((childpid = fork() == 0))
        {
            close(server_socket);

            while (1)
            {
                recv(c_socket, buffer, RECV_BUFFER_SIZE, 0);
                if (strcmp(buffer, ":exit") == 0)
                {
                    printf("Disconnected from %s%d\n", inet_ntoa(c_address.sin_addr), ntohs(c_address.sin_port));
                    break;
                }
                else
                {
                    printf("Client: %s\n", buffer);
                    send(c_socket, buffer, strlen(buffer), 0);
                    bzero(buffer, sizeof(buffer));
		}
		    
            }
        }
    }

    //Closing Socket
    close(c_socket);




    return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv) {
  char *server_port;

  if (argc != 2) {
    fprintf(stderr, "Usage: ./server-c [server port]\n");
    exit(EXIT_FAILURE);
  }

  server_port = argv[1];
  return server(server_port);
}
