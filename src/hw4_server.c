#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "hw4.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int listenfd, connfd;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};
    ChessGame game;
    socklen_t addrlen = sizeof(address);
    int opt = 1;

    // Create socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(listenfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listenfd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
    if ((connfd = accept(listenfd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    initialize_game(&game);
    display_chessboard(&game);

    while (1) {
        // Read client command
        if (read(connfd, buffer, BUFFER_SIZE - 1) < 0) {
            perror("read failed");
            break;
        }
        
        if (receive_command(&game, buffer, connfd, false) == COMMAND_FORFEIT) break;

        printf("Enter a valid command: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }

        int result = send_command(&game, buffer, connfd, false);
        while (result == COMMAND_ERROR) {
            printf("Invalid command, please re-enter: ");
            if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                printf("Error reading input\n");
                continue;
            }
            result = send_command(&game, buffer, connfd, false);
        }

        if (result == COMMAND_FORFEIT) break;
    }

    // Cleanup
    close(connfd);
    close(listenfd);
    return 0;
}
