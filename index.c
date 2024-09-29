#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// maximum application buffer
#define APP_BUFFER_SIZE 1024 // 1KB (representing the request size)
#define PORT 8080            // server port

int main()
{
    // server and client file descriptors
    // client_fd is used to accept the client connection
    // server_fd is used to create the server socket
    int server_fd, client_fd;

    // define the server socket address
    struct sockaddr_in address;        // server address
    int address_len = sizeof(address); // address length

    // define the application buffer where the request will be stored
    // data will move from the client_fd to the buffer
    char buffer[APP_BUFFER_SIZE] = {0}; // using 0 to initialize the buffer

    // create the server socket
    // AF_INET is the address family for IPv4 (we can use AF_INET6 for IPv6)
    // SOCK_STREAM is the type of socket (TCP) -> streaming socket
    // 0 is the protocol (IP) -> just one protocol available
    // socket(AF_INET, SOCK_STREAM, 0) returns the file descriptor (server_fd)
    // if the socket creation fails, the function returns 0
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the server socket to the port
    address.sin_family = AF_INET;         // address family (IPv4)
    address.sin_addr.s_addr = INADDR_ANY; // listen to any address (0.0.0.0 interface)
    address.sin_port = htons(PORT);       // convert the port to network byte order

    // bind the server_fd to the address
    // if you need to accept multiple processes on the same port, you need to set the SO_REUSEADDR option
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // create the queue for the incoming connections
    // listen for clients, with 10 backlogged connections (10 connection in accept queue)
    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return 0;
}