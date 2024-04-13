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
    ChessGame game;
    int connfd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(connfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    initialize_game(&game);
    display_chessboard(&game);

    while (1) {
        printf("Enter a valid command: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }

        int result = send_command(&game, buffer, connfd, true);
        while (result == COMMAND_ERROR) {
            printf("Invalid command, please re-enter: ");
            if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                printf("Error reading input\n");
                continue;
            }
            result = send_command(&game, buffer, connfd, true);
        }

        if (result == COMMAND_FORFEIT) break;

        // Read server response
        if (read(connfd, buffer, BUFFER_SIZE - 1) < 0) {
            perror("read failed");
            break;
        }
        
        if (receive_command(&game, buffer, connfd, true) == COMMAND_FORFEIT) break;
    }

    // Cleanup
    close(connfd);
    return 0;
}
