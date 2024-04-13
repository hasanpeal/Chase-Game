#include "hw4.h"



int main() {
    int listenfd, connfd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = { 0 };
    ChessGame game;

    // Create socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set options to reuse the IP address and IP port if either is already in use
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
        return -1;
    }
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))");
        return -1;
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(listenfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listenfd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    INFO("Server listening on port %d", PORT);
    // Accept incoming connection
    if ((connfd = accept(listenfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    INFO("Server accepted connection");

    initialize_game(&game);
    display_chessboard(&game);

    while (1) {
        // Fill this in
        read(connfd, buffer, 1024 - 1);
        if(receive_command(&game, buffer, connfd, false) == COMMAND_FORFEIT){
            break;
        }
        INFO("Enter a valid command");
        char str[200];
        fgets(str, 200, stdin);
        int result = send_command(&game, str, connfd, false);
        while(result == COMMAND_ERROR || result == COMMAND_ERROR){
            INFO("You entered wrong command, please Enter a valid command");
            result = send_command(&game, str, connfd, false);
        }
        if(result == COMMAND_FORFEIT){
            break;
        }
    }
    setsockopt(connfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    setsockopt(connfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    close(connfd);
    close(listenfd);
    return 0;
}