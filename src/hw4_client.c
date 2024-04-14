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
    int connfd = 0;
    struct sockaddr_in serv_addr;

    // Connect to the server
    if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(connfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    initialize_game(&game);
display_chessboard(&game);

char buffer[BUFFER_SIZE];
while (1) {
    bool validCommand = false;
    do {
        printf("Client command: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) break; // Handle EOF or error
        buffer[strcspn(buffer, "\n")] = 0; // Ensure command string is properly terminated

        int commandResult = send_command(&game, buffer, connfd, true);
        if (commandResult == COMMAND_FORFEIT) {
            close(connfd);
            break;
        }
        validCommand = (commandResult != COMMAND_ERROR && commandResult != COMMAND_UNKNOWN && commandResult != COMMAND_SAVE);
    } while (!validCommand && !feof(stdin));

    if (!validCommand) break; // Exit if last command was invalid

    int bytes_read = read(connfd, buffer, BUFFER_SIZE - 1);
    if (bytes_read <= 0) break; // Handle disconnection
    buffer[bytes_read] = '\0';

    if (receive_command(&game, buffer, connfd, true) == COMMAND_FORFEIT) {
        close(connfd);
        break;
    }
}

FILE *temp = fopen("./fen.txt", "w");
if (temp != NULL) {
    char fen[200];
    chessboard_to_fen(fen, &game);
    fprintf(temp, "%s", fen);
    fclose(temp);
}
close(connfd);
return 0;
}