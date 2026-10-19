#include "server.h"
#include "mime.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void *handle_client(void *args_ptr) 
{
    ThreadArgs *args = (ThreadArgs*)args_ptr;
    int client_socket = args->client_socket;
    TokenBucket *bucket = args->bucket;
    free(args);

    char buffer[2048] = {0};
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    
    if (bytes_read <= 0) {
        close(client_socket);
        return NULL;
    }

    if (allow_request(bucket)) 
    {
        char method[16], path[256];
        sscanf(buffer, "%15s %255s", method, path);

        char *filepath = path;
        if (strcmp(filepath, "/") == 0) {
            filepath = "/index.html";
        }

        char *filename = filepath + 1;

        const char *mode = "r";
        if (strstr(filename, ".png") || strstr(filename, ".jpg") || strstr(filename, ".jpeg") || strstr(filename, ".ico")) {
            mode = "rb";
        }

        FILE *file = fopen(filename, mode);
        
        if (file == NULL) {
            char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 13\r\nConnection: close\r\n\r\n404 Not Found";
            ssize_t ret = write(client_socket, not_found, strlen(not_found));
            (void)ret;
            printf("[Thread] Request for %s on socket %d - FILE NOT FOUND (404).\n", path, client_socket);
        } else {
            // Get file size
            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            rewind(file);

            // Get dynamic MIME type using filename
            const char *mime_type = get_mime_type(filename); 

            // Send HTTP Response Header
            char header[512];
            int header_len = snprintf(header, sizeof(header),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %ld\r\n"
                "Connection: close\r\n\r\n",
                mime_type, file_size);
            
            ssize_t ret_hdr = write(client_socket, header, header_len);
            (void)ret_hdr;

            // Stream the raw binary/text bytes in chunks
            char file_buffer[4096];
            size_t bytes_read_file;
            while ((bytes_read_file = fread(file_buffer, 1, sizeof(file_buffer), file)) > 0) {
                ssize_t ret_body = write(client_socket, file_buffer, bytes_read_file);
                (void)ret_body;
            }

            fclose(file);
            printf("[Thread] Request for %s on socket %d ALLOWED (200 OK).\n", path, client_socket);
        }
    } 
    else 
    {
        char *response = "HTTP/1.1 429 Too Many Requests\r\nContent-Type: text/plain\r\nContent-Length: 38\r\nRetry-After: 5\r\nConnection: close\r\n\r\nRate Limit Exceeded. Try again later.\n";
        ssize_t ret_rate = write(client_socket, response, strlen(response));
        (void)ret_rate;
        printf("[Thread] Request on socket %d BLOCKED (Rate Limited).\n", client_socket);
    }
    
    fflush(stdout);
    close(client_socket);
    return NULL;
}
