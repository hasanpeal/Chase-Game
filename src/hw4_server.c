#include "hw4.h"

int main() {
    int listenfd, connfd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

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
    // Server accepted connection
    ChessGame game;
initialize_game(&game);
display_chessboard(&game);

char buffer[BUFFER_SIZE];
while (1) {
    int bytes_read = read(connfd, buffer, BUFFER_SIZE - 1);
    if (bytes_read <= 0) break;  // Handle client disconnection
    buffer[bytes_read] = '\0';

    int result = receive_command(&game, buffer, connfd, false);
    if (result == COMMAND_FORFEIT) {
        close(connfd);
        break;
    }

    bool validCommand;
    do {
        printf("Server command: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) break;
        buffer[strcspn(buffer, "\n")] = 0;

        result = send_command(&game, buffer, connfd, false);
        validCommand = (result != COMMAND_ERROR && result != COMMAND_UNKNOWN);
    } while (!validCommand && !feof(stdin));

    if (!validCommand) break;
}

close(listenfd);
close(connfd);
return 0;
}