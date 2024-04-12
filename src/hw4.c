#include "hw4.h"
//t
void initialize_game(ChessGame *game) {
    game->currentPlayer = WHITE_PLAYER;
    game->moveCount = 0;
    game->capturedCount = 0;
    game->chessboard[0][0] = game->chessboard[0][7] = 'r';
    game->chessboard[0][1] = game->chessboard[0][6] = 'n';
    game->chessboard[0][2] = game->chessboard[0][5] = 'b';
    game->chessboard[7][0] = game->chessboard[7][7] = 'R';
    game->chessboard[7][1] = game->chessboard[7][6] = 'N';
    game->chessboard[7][2] = game->chessboard[7][5] = 'B';
    game->chessboard[0][3] = 'q';
    game->chessboard[0][4] = 'k';
    game->chessboard[7][3] = 'Q';
    game->chessboard[7][4] = 'K';
    for(int i = 0; i < 8; i++)
        game->chessboard[1][i] = 'p';
    for(int i = 0; i < 8; i++)
        game->chessboard[6][i] = 'P';
    for(int i = 2; i < 6; i++)
        for(int j = 0; j < 8; j++)
            game->chessboard[i][j] = '.';
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    memset(fen, 0, strlen(fen) * sizeof(fen[0]));
    int spaces = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            if(game->chessboard[i][j] == '.')
                spaces++;
            else{
                if(spaces > 0){
                    char str[] = {spaces + '0'};
                    strcat(fen, str);
                    spaces = 0;
                }
                char str[] = {game->chessboard[i][j]};
                strcat(fen, str);
            }
        if(spaces > 0){
            char str[] = {spaces + '0'};
            strcat(fen, str);
            spaces = 0;
        }
        if(i != 7)
            strcat(fen, "/");
        else{
            if(game->currentPlayer == WHITE_PLAYER)
                strcat(fen, " w");
            else
            strcat(fen, " b");
        }
    }

}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if(game->chessboard[dest_row][dest_col] != '.' && src_col == dest_col)
            return false;
    if(src_col != dest_col){
        if(src_col > dest_col){
            if(src_col - dest_col > 1)
                return false;
        }
        else{
            if(dest_col - src_col > 1)
                return false;
        }
        if(game->chessboard[dest_row][dest_col] != '.' && game->chessboard[dest_row][dest_col] == game->chessboard[src_row][src_col])
            return false;
        if(game->chessboard[dest_row][dest_col] == '.')
            return false;
    }
    if(islower(piece) != 0){
        if(src_row > dest_row)
            return false;
        if(src_row != 1 && dest_row - src_row != 1)
            return false;
        else if(src_row == 1 && dest_row - src_row  != 1 && dest_row - src_row  != 2)
            return false;
        if(dest_row - src_row  == 2)
            if(game->chessboard[src_row + 1][dest_col] != '.' )
                return false;
    }
    else{
        if(src_row < dest_row)
            return false;
        if(src_row != 6 && src_row - dest_row  != 1)
            return false;
        else if(src_row == 6 && src_row - dest_row  != 1 && src_row - dest_row  != 2)
            return false;
        if(src_row - dest_row  == 2)
            if(game->chessboard[src_row - 1][dest_col] != '.' )
                return false;
    }
    return true;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if(dest_row - src_row != 0 && dest_col - src_col != 0)
        return false;
    if(dest_row - src_row == 0){
        if(dest_col > src_col){
            for(int i = src_col + 1; i < dest_col; i++)
                if(game->chessboard[dest_row][i] != '.')
                    return false;
        }
        else{
            for(int i = src_col - 1; i > dest_col; i--)
                if(game->chessboard[dest_row][i] != '.')
                    return false;
        }
    }
    else{
        if(dest_row > src_row){
            for(int i = src_row + 1; i < dest_row; i++)
                if(game->chessboard[i][dest_col] != '.')
                    return false;
        }
        else{
            for(int i = src_row - 1; i > dest_row; i--)
                if(game->chessboard[i][dest_col] != '.')
                    return false;
        }
    }
    if(game->chessboard[dest_row][dest_col] != '.' && game->chessboard[dest_row][dest_col] == game->chessboard[src_row][src_col])
        return false;

    return true;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    if((src_row + 1 == dest_row && src_col + 2 == dest_col) || (src_row + 1 == dest_row && src_col - 2 == dest_col)
        || (src_row - 1 == dest_row && src_col + 2 == dest_col) || (src_row - 1 == dest_row && src_col - 2 == dest_col)
        || (src_row + 2 == dest_row && src_col + 1 == dest_col) || (src_row + 2 == dest_row && src_col - 1 == dest_col)
        || (src_row - 2 == dest_row && src_col + 1 == dest_col) || (src_row - 2 == dest_row && src_col - 1 == dest_col))
        return true;
    return false;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if(dest_row - src_row == 0 || dest_col - src_col == 0)
        return false;
    if(src_row < dest_row){
        if(src_col < dest_col){
            for(int i = 1; src_row + i < dest_row; i++)
                if(game->chessboard[src_row + i][src_col + i] != '.')
                    return false;
        }
        else{
            for(int i = 1; src_row + i < dest_row; i++)
                if(game->chessboard[src_row + i][src_col - i] != '.')
                    return false;
        }
    }
    else{
        if(src_col < dest_col){
            for(int i = 1; i + dest_row < src_row; i++)
                if(game->chessboard[src_row - i][src_col + i] != '.')
                    return false;
        }
        else{
            for(int i = 1; i + dest_row < src_row; i++)
                if(game->chessboard[src_row - i][src_col - i] != '.')
                    return false;
        }
    }
    if(game->chessboard[dest_row][dest_col] != '.' && game->chessboard[dest_row][dest_col] == game->chessboard[src_row][src_col])
        return false;
    return true;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    return (is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game) 
        || is_valid_rook_move(src_row, src_col, dest_row, dest_col, game));
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    if((src_row == dest_row && src_col + 1 == dest_col) || (src_row == dest_row && src_col - 1 == dest_col)
        || (src_row + 1 == dest_row && src_col == dest_col) || (src_row - 1 == dest_row && src_col == dest_col)
        || (src_row + 1 == dest_row && src_col + 1 == dest_col) || (src_row + 1 == dest_row && src_col - 1 == dest_col)
        || (src_row - 1 == dest_row && src_col + 1 == dest_col) || (src_row - 1 == dest_row && src_col - 1 == dest_col))
        return true;
    return false;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    switch (piece)
    {
        case 'r':
        case 'R':
            return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
            break;
        case 'p':
        case 'P':
            return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
            break;
        case 'n':
        case 'N':
            return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
            break;
        case 'b':
        case 'B':
            return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
            break;
        case 'q':
        case 'Q':
            return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
            break;
        case 'k':
        case 'K':
            return is_valid_king_move(src_row, src_col, dest_row, dest_col);
            break;
        default:
            return false;
    }
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    char *str = strdup(fen);
    char *token = strtok(str, "/");

    initialize_game(game);
   
    int row = 0;
    while (token != NULL)
    {
        int index = 0;
        for(int i = 0; (size_t)i < strlen(token); i++)
            if(isdigit(token[i]))
                for(int j = 0; j < (token[i] - '0'); j++, index++)
                    game->chessboard[row][index] = '.';
            else if(token[i] == ' '){
                if(token[i + 1] == 'b')
                    game->currentPlayer = BLACK_PLAYER;
                else
                    game->currentPlayer = WHITE_PLAYER;
                break;
            }
            else{
                game->chessboard[row][index] = token[i];
                index++;
            }
        token = strtok(NULL, "/");
        row++;
    }
}

int parse_move(const char *move, ChessMove *parsed_move) {
    if(strlen(move) != (size_t)4 && strlen(move) != (size_t)5)
        return PARSE_MOVE_INVALID_FORMAT;
    if(!(isalpha(move[0]) && move[0] >= 'a' && move[0] <= 'h') || !(isalpha(move[2]) && move[2] >= 'a' && move[2] <= 'h'))
        return PARSE_MOVE_INVALID_FORMAT;
    if(!(isdigit(move[1]) && move[1] >= '1' && move[1] <= '8') || !(isdigit(move[3]) && move[3] >= '1' && move[3] <= '8'))
        return PARSE_MOVE_OUT_OF_BOUNDS;
    if(strlen(move) == (size_t)5 && move[3] != '8' && move[3] != '1')
        return PARSE_MOVE_INVALID_DESTINATION;
    if(strlen(move) == (size_t)5 && move[4] != 'q' && move[4] != 'b' && move[4] != 'r' && move[4] != 'n')
        return PARSE_MOVE_INVALID_PROMOTION;

    parsed_move->startSquare[0] = move[0];
    parsed_move->startSquare[1] = move[1];
    parsed_move->startSquare[2] = 0;
    parsed_move->endSquare[0] = move[2];
    parsed_move->endSquare[1] = move[3];
    if(strlen(move) == (size_t)5)
        parsed_move->endSquare[2] = move[4];
    parsed_move->endSquare[3] = 0;

    return 0;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    int src_row, src_col, dest_row, dest_col;
    src_row = move->startSquare[1] - '0';
    src_col = move->startSquare[0] - 'a';
    dest_row = move->endSquare[1] - '0';
    dest_col = move->endSquare[0] - 'a';

    if (validate_move) {
        if((game->currentPlayer == WHITE_PLAYER && !is_client) || (game->currentPlayer == BLACK_PLAYER && is_client))
            return MOVE_OUT_OF_TURN;
        if(game->chessboard[8 - src_row][src_col] == '.')
            return MOVE_NOTHING;
        if((is_client && islower(game->chessboard[8 - src_row][src_col]) == 0)
            || (!is_client && islower(game->chessboard[8 - src_row][src_col]) != 0))
            return MOVE_WRONG_COLOR;
        if((islower(game->chessboard[8 - dest_row][dest_col]) == 0 && islower(game->chessboard[8 - src_row][src_col]) == 0)
            || (islower(game->chessboard[8 - dest_row][dest_col]) != 0 && islower(game->chessboard[8 - src_row][src_col]) != 0))
            return MOVE_SUS;
        if(strlen(move->endSquare) == (size_t)3 && game->chessboard[8 - src_row][src_col] != 'p' && game->chessboard[8 - src_row][src_col - 1] != 'P')
            return MOVE_NOT_A_PAWN;
        if(strlen(move->endSquare) == (size_t)3 && ((game->chessboard[8 - src_row][src_col] == 'p' && dest_row == 1) 
            || (game->chessboard[8 - src_row][src_col] == 'P' && dest_row == 8)))
            return MOVE_MISSING_PROMOTION;
        if(!is_valid_move(game->chessboard[8 - src_row][src_col], src_row, src_col, dest_row, dest_col, game))
            return MOVE_WRONG;
    }
    if(game->chessboard[8 - dest_row][dest_col] != '.'){
        if(game->chessboard[8 - dest_row][dest_col] == 'p' || game->chessboard[8 - dest_row][dest_col] == 'P'){
            bool found = false;
            for(int i = 0; i < game->capturedCount; i++)
                if(game->capturedPieces[i] == game->chessboard[8 - dest_row][dest_col]){
                    found = true;
                    break;
                }
            if(!found)
                game->capturedPieces[game->capturedCount] = game->chessboard[8 - dest_row][dest_col];
        }
        game->capturedCount++;
    }
    game->moves[game->moveCount] = *move;
    game->moveCount++;
    game->chessboard[8 - dest_row][dest_col] = game->chessboard[8 - src_row][src_col];
    game->chessboard[8 - src_row][src_col] = '.';
    if(is_client)
        game->currentPlayer = BLACK_PLAYER;
    else
        game->currentPlayer = WHITE_PLAYER;

    return 0;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    char *str = strdup(message);
    char *token = strtok(str, " ");
    if(strcmp(token, "/move") == 0){
        token = strtok(NULL, " ");
        ChessMove parsed_move;
        if(parse_move(token, &parsed_move) == 0){
            if(make_move(game, &parsed_move, is_client, true) == 0){
                send(socketfd, message, strlen(message), 0);
                return COMMAND_MOVE;
            }
            else
                return COMMAND_ERROR;
        }
        else
            return COMMAND_ERROR;
    }else if(strcmp(token, "/forfeit") == 0){
        send(socketfd, message, strlen(message), 0);
        return COMMAND_FORFEIT;
    }else if(strcmp(token, "/chessboard") == 0){
        display_chessboard(game);
        return COMMAND_DISPLAY;
    }else if(strcmp(token, "/import") == 0){
        if(!is_client){
            token = strtok(NULL, " ");
            fen_to_chessboard(token, game);
            send(socketfd, message, strlen(message), 0);
            return COMMAND_IMPORT;
        }
        else
            return COMMAND_ERROR;
    }else if(strcmp(token, "/load") == 0){
        char *userName;
        int save_number;
        token = strtok(NULL, " ");
        userName = token;
        token = strtok(NULL, " ");
        save_number = atoi(token);
        if(load_game(game, userName, "game_database.txt", save_number) == 0){
            send(socketfd, message, strlen(message), 0);
            return COMMAND_LOAD;
        }
        else
            return COMMAND_ERROR;
    }else if(strcmp(token, "/save") == 0){
        if(save_game(game, "client's username", "game_database.txt") == 0){
            send(socketfd, message, strlen(message), 0);
            return COMMAND_SAVE;
        }
        else
            return COMMAND_ERROR;
    }
    return COMMAND_UNKNOWN;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    char *str = strdup(message);
    char *token = strtok(str, " ");
    if(strcmp(token, "/move") == 0){
        token = strtok(NULL, " ");
        ChessMove parsed_move;
        if(parse_move(token, &parsed_move) == 0){
            if(make_move(game, &parsed_move, is_client, false) == 0){
                return COMMAND_MOVE;
            }
            else
                return COMMAND_ERROR;
        }
        else
            return COMMAND_ERROR;
    }else if(strcmp(token, "/forfeit") == 0){
        close(socketfd);
        return COMMAND_FORFEIT;
    }else if(strcmp(token, "/import") == 0){
        if(!is_client){
            token = strtok(NULL, " ");
            fen_to_chessboard(token, game);
            return COMMAND_IMPORT;
        }
        else
            return COMMAND_ERROR;
    }else if(strcmp(token, "/load") == 0){
        char *userName;
        int save_number;
        token = strtok(NULL, " ");
        userName = token;
        token = strtok(NULL, " ");
        save_number = atoi(token);
        if(load_game(game, userName, "game_database.txt", save_number) == 0){
            return COMMAND_LOAD;
        }
        else
            return COMMAND_ERROR;
    }
    return -1;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    char *un = strdup(username);
    for(int i = 0; (size_t)i < strlen(un); i++)
        if(un[i] == ' ')
            return -1;
    FILE *fptr;
    fptr = fopen(db_filename, "a");
    char fenStr[200];
    chessboard_to_fen(fenStr, game);
    strcat(un, ":");
    strcat(un, fenStr);
    fprintf(fptr, "%s", un);

    // Close the file
    fclose(fptr);
    return 0;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    for(int i = 0; (size_t)i < strlen(username); i++)
        if(username[i] == ' ')
            return -1;
    if(save_number < 1)
        return -1;
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(db_filename, "r");
    if (fp == NULL)
        return -1;

    bool found = false;
    while ((read = getline(&line, &len, fp)) != -1) {
        char *token = strtok(line, ":");
        if(username == token && save_number == 1){
            token = strtok(NULL, ":");
            fen_to_chessboard(token, game);
            found = true;
            break;
        }
        else if(username == token)
            save_number--;
    }

    fclose(fp);
    if(found)
        return 0;
    else
        return -1;
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
