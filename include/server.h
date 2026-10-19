#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "rate_limiter.h"

#define PORT 8080

// Struct to pass multiple arguments securely to client handler threads
typedef struct {
    int client_socket;
    TokenBucket *bucket;
} ThreadArgs;

// Function prototypes
int setup_server_socket();
void *handle_client(void *args_ptr);

#endif
