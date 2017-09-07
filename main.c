/*
 * main.c
 *
 *  Created on: Aug 30, 2017
 *      Author: sasha
 */

#include "SEMode.h"
#include "CHGame.h"
#include "CHParser.h"


int main(){
	SP_BUFF_SET();
	char strCommand[MAX_LINE_SIZE];
	CHCommand command;
	bool isTurnChanged = true;
	int winner;
	CHGame *game = startSettingsMode();
	if(!game)
		return 0;
	while(true){
		if(game->gameMode == 2 || (game->gameMode == 1 && game->currentTurn == game->userColor)){
			if(isTurnChanged)
				chGamePrintBoard(game);
			if(game->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL)
				printf("white player - enter your move:\n");
			else
				printf("black player - enter your move:\n");
			fgets(strCommand,MAX_LINE_SIZE,stdin);
			command = chParserPraseLine(strCommand);
			isTurnChanged = false;
			if (command.cmd == CH_MOVE) {
				if (command.validArg) {
					CH_GAME_MESSAGE mes = chGameSetMove(game, command.fRow,
							command.fColomn, command.toRow, command.toColomn);
					if (mes == CH_GAME_INVALID_COLOR) {
						printf("The specified position does not contain your piece\n");
					} else if (mes == CH_GAME_INVALID_MOVE) {
						printf("Illegal move\n");
					} else {
						if (game->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
							game->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
							isTurnChanged = true;
						} else {
							game->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
							isTurnChanged = true;
						}
						winner = chIsCheckmateOrTie(game);
						if (winner == -2)
							return -1;
						else if (winner != -1) {
							chGamePrintBoard(game);
							if (winner == CH_GAME_WHITE_PLAYER_SYMBOL)
								printf("Checkmate! white player wins the game\n");
							else if (winner == CH_GAME_BLACK_PLAYER_SYMBOL)
								printf("Checkmate! black player wins the game\n");
							else {
								printf("The game is tied\n");
							}
							chGameDestroy(game);
							printf("Exiting...\n");
							return 0;
						} else {
							if(isCheck(game) == CH_GAME_INVALID_ARGUMENT)
								return -1;
						}
					}
				} else {
					printf("Invalid position on the board\n");
				}
			} else if (command.cmd == CH_GET_MOVES) {
				if (command.validArg) {
					CH_GAME_MESSAGE mes = chGameShowMoves(game, command.fRow,
							command.fColomn);
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
				if (!game)
					return -1;
				printf("Restarting...\n");
			} else if (command.cmd == CH_QUIT) {
				chGameDestroy(game);
				printf("Exiting...\n");
				return 0;
			}
		}
		else{

		}
	}
	return 0;
}
