#include "server.h"

// Instantiate global token bucket rate limiter
TokenBucket global_bucket;

int main() 
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    /* Initialize the Token Bucket rate limiter:
       - Bucket Capacity: 5 tokens (allows short bursts of up to 5 requests)
       - Refill Rate: 1.0 token per second
    */
    init_bucket(&global_bucket, 5.0, 1.0);

    /* Initialize the listening server socket */
    server_fd = setup_server_socket();

    printf("Server is listening on port %d with Rate Limiter active...\n", PORT);
    fflush(stdout);

    /* Connection loop */
    while(1) 
    {
        int client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) 
        {
            perror("Accept failed");
            continue;
        }

        /* Group arguments for thread safety */
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        if (args == NULL) 
        {
            perror("Malloc failed");
            close(client_socket);
            continue;
        }
        args->client_socket = client_socket;
        args->bucket = &global_bucket;

        /* Spawn client handler worker thread */
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)args) < 0) 
        {
            perror("Could not create thread");
            free(args);
            close(client_socket);
            continue;
        }
        
        pthread_detach(thread_id);
    }

    close(server_fd);
    cleanup_bucket(&global_bucket);
    return 0;
}
