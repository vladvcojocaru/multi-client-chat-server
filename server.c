#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg);
void broadcast_message(char *message, int sender_fd);

int main(){
    // Step 1: initialize variables
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Step 2: create socket and configure server address
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Failed to create socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 

    // Step 3: bind the server_fd to the server address
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("Failed to bind");
        exit(1);
    }

    // Step 4: listen for connections
    if(listen(server_fd, MAX_CLIENTS) < 0){
        perror("Failed to bind");
        exit(1);
    }

    printf("Server listening on port: [%d]\n", PORT);

    // Step 5: starting communication loop
    while(1){
        // Step 6: accept new clients < 0
        if((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) < 0){
            perror("Failed to accept new client");
            break;
        }

        pthread_mutex_lock(&clients_mutex);
        for(int i = 0; i < MAX_CLIENTS; i++){
            if(clients[i] == 0){
                clients[i] = client_fd;
                pthread_t tid;
                int *client_ptr = malloc(sizeof(int));
                *client_ptr = client_fd;
                pthread_create(&tid, NULL, handle_client, client_ptr);
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }

    return 0;
}


void *handle_client(void *arg){
    int client_fd = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while((bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0){
        buffer[bytes_read] = '\0';
        printf("Received message: %s\n", buffer);
        broadcast_message(buffer, client_fd);
    }

    if(bytes_read == 0){
        printf("Client disconnected. \n");
    } else if (bytes_read < 0){
        perror("recv failed");
    }

    close(client_fd);

    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i] == client_fd){
            clients[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    return NULL;
}

void broadcast_message(char *message, int sender_fd){
    pthread_mutex_lock(&clients_mutex);
    
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i] != 0 && clients[i] != sender_fd){
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}