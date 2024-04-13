
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
char buffer[BUFFER_SIZE] = { 0 };
int opt = 1;

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

while (1) {
// Fill this in
INFO("Enter a valid command");
char str[200];
fgets(str, 200, stdin);
int result = send_command(&game, str, connfd, true);
while(result == COMMAND_ERROR || result == COMMAND_ERROR || result == COMMAND_SAVE){
INFO("You entered wrong command, please Enter a valid command");
result = send_command(&game, str, connfd, true);
}
printf("++++++++++++++++++++++++++++++%d", result);
if(result == COMMAND_FORFEIT){
break;
}
read(connfd, buffer, 1024 - 1);
result = receive_command(&game, buffer, connfd, true);
printf("///////////////////////////////%d", result);
if(result == COMMAND_FORFEIT){
break;
}
}

// Please ensure that the following lines of code execute just before your program terminates.
// If necessary, copy and paste it to other parts of your code where you terminate your program.
FILE *temp = fopen("./fen.txt", "w");
char fen[200];
chessboard_to_fen(fen, &game);
fprintf(temp, "%s", fen);
fclose(temp);
setsockopt(connfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
setsockopt(connfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
close(connfd);
return 0;
}