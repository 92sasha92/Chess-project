/*
 * main.c
 *
 *  Created on: Aug 30, 2017
 *      Author: sasha
 */

#include "SEMode.h"
#include "CHGame.h"
#include "CHParser.h"
#include "CHMiniMax.h"

#define REC_DEPTH 4

char getColumn(int num){
    switch (num){
        case 0:
            return 'A';
        case 1:
            return 'B';
        case 2:
            return 'C';
        case 3:
            return 'D';
        case 4:
            return 'E';
        case 5:
            return 'F';
        case 6:
            return 'G';
        case 7:
            return 'H';
    }
}

char *getPieceName(char piece){
    switch (piece){
        case 'm':
        case 'M':
            return "Pawn";
        case 'r':
        case 'R':
            return "Rook";
        case 'n':
        case 'N':
            return "Knight";
        case 'b':
        case 'B':
            return "Bishop";
        case 'q':
        case 'Q':
            return "Queen";
        case 'k':
        case 'K':
            return "King";
    }
}

char *getPlayerName(int player){
    if (player == 0)
        return "Black";
    return "White";
}

void change_turn(CHGame* game,  bool *isTurnChanged){
    if (game->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
        game->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
    } else {
        game->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
    }
    *isTurnChanged = true;
}

int end_of_move(CHGame* game, CHMoveNode* best_move, bool *isTurnChanged){
    int winner;
    change_turn(game, isTurnChanged);
    winner = chIsCheckmateOrTie(game);
    if (winner == CH_GAME_INVALID_ARGUMENT) {
        chGameDestroy(game);
        free(best_move);
        return -1;
    }
    else if (winner != CH_GAME_NO_WIN_OR_TIE) {
        chGamePrintBoard(game);
        if (winner == CH_GAME_WHITE_WINS)
            printf("Checkmate! white player wins the game\n");
        else if (winner == CH_GAME_BLACK_WINS)
            printf("Checkmate! black player wins the game\n");
        else {
            printf("The game is tied\n");
        }
        chGameDestroy(game);
        free (best_move);
        printf("Exiting...\n");
        return -1;
    } else {
        if((game->gameMode == 1) && (game->currentTurn != game->userColor)){
            if (isCheck(game, 1) == CH_GAME_INVALID_ARGUMENT) {
                chGameDestroy(game);
                free(best_move);
                return -1;
            }
        }
        else{
            if(isCheck(game, 0) == CH_GAME_INVALID_ARGUMENT){
                chGameDestroy(game);
                free (best_move);
                return -1;
            }
        }
    }
    return 1;
}


int main() {
    SP_BUFF_SET();
    char strCommand[MAX_LINE_SIZE];
    CHCommand command;
    bool isTurnChanged = true;
    CHMoveNode *best_move = (CHMoveNode *) malloc(sizeof(CHMoveNode));
    if (!best_move) {
        return -1;
    }
    CHGame *game = startSettingsMode();
    if (!game) {
        free(best_move);
        return 0;
    }
    while (true) {
        if (game->gameMode == 2 || (game->gameMode == 1 && game->currentTurn == game->userColor)) {
            if (isTurnChanged)
                chGamePrintBoard(game);
            if (game->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL)
                printf("white player - enter your move:\n");
            else
                printf("black player - enter your move:\n");
            fgets(strCommand, MAX_LINE_SIZE, stdin);
            command = chParserPraseLine(strCommand);
            isTurnChanged = false;
            if (command.cmd == CH_MOVE) {
                if (command.validArg) {
                    CH_GAME_MESSAGE mes = chGameSetMove(game, command.fRow,
                                                        command.fColomn, command.toRow, command.toColomn, false);
                    if (mes == CH_GAME_INVALID_COLOR) {
                        printf("The specified position does not contain your piece\n");
                    } else if (mes == CH_GAME_INVALID_MOVE) {
                        printf("Illegal move\n");
                    } else {
                        if (end_of_move(game, best_move, &isTurnChanged) == -1) {
                            return -1;
                        }
                    }
                } else {
                    printf("Invalid position on the board\n");
                }
            } else if (command.cmd == CH_GET_MOVES) {
                if (command.validArg) {
                    CH_GAME_MESSAGE mes = chGameShowMoves(game, command.fRow, command.fColomn);
                    if (mes == CH_GAME_INVALID_COLOR) {
                        printf("The specified position does not contain your piece\n");
                    } else if (mes == CH_GAME_INVALID_MOVE) {
                        printf("Illegal move\n");
                    }
                } else
                    printf("Invalid position on the board\n");
            } else if (command.cmd == CH_SAVE) {
                if (chGameSave(game, command.path) == CH_GAME_FILE_PROBLEM)
                    printf("File cannot be created or modified\n");
            } else if (command.cmd == CH_RESET) {
                chGameDestroy(game);
                game = startSettingsMode();
                if (!game) {
                    chGameDestroy(game);
                    free(best_move);
                    return -1;
                }
                printf("Restarting...\n");
                isTurnChanged = true;
            } else if (command.cmd == CH_QUIT) {
                chGameDestroy(game);
                free(best_move);
                printf("Exiting...\n");
                return 0;
            } else if (command.cmd == CH_UNDO) {
                if (game->gameMode == 2) {
                    printf("Undo command not available in 2 players mode\n");
                } else {
                    if (game->list->actualSize >= 2){
                        *best_move = spArrayListGetFirst(game->list);
                        CH_GAME_MESSAGE mes = chGameUndoPrevMove(game);
                        if (mes == CH_GAME_SUCCESS) {
                            printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
                                   getPlayerName(game->userColor),
                                   (best_move->to_row + 1),
                                   getColumn(best_move->to_col),
                                   (best_move->from_row + 1),
                                   getColumn(best_move->from_col));
                            *best_move = spArrayListGetFirst(game->list);
                            CH_GAME_MESSAGE mes = chGameUndoPrevMove(game);
                            if (mes == CH_GAME_SUCCESS) {
                                printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
                                       getPlayerName(!game->userColor),
                                       (best_move->to_row + 1),
                                       getColumn(best_move->to_col),
                                       (best_move->from_row + 1),
                                       getColumn(best_move->from_col));
                            }
                        }
                        isTurnChanged = true;
                    } else {
                        printf("Empty history, move cannot be undone\n");
                        isTurnChanged = false;
                    }
                }
            }
        }else {
            best_move = alphabeta(chGameCopy(game), game->difficulty, game->currentTurn, best_move); ////////insted of REC_DEPTH should be game->difficulty////////
            chGameSetMove(game, best_move->from_row,
                          best_move->from_col, best_move->to_row, best_move->to_col, true);
            if (end_of_move(game, best_move, &isTurnChanged) == -1) {
                return -1;
            }
            printf("Computer: move %s at <%d,%c> to <%d,%c>\n",
                   getPieceName(best_move->current_piece),
                   best_move->from_row + 1,
                   getColumn(best_move->from_col),
                   best_move->to_row + 1,
                   getColumn(best_move->to_col));
            isTurnChanged = true;
        }
    }
    return 0;
}
