#include "server.h"

int setup_server_socket() 
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    /* 1. Create the socket */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    /* Set socket options to reuse the port */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    /* 2. Define the address and port */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    /* 3. Bind the socket */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    /* 4. Start listening */
    if (listen(server_fd, 10) < 0) 
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}
