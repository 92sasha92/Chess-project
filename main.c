/*
 * main.c
 *
 *  Created on: Aug 30, 2017
 *      Author: sasha
 */

#include "SEMode.h"
#include "CHGame.h"
#include "CHParser.h"
#include "gameBoard.h"

int main(int argc, char** argv){
	SP_BUFF_SET();
	char strCommand[MAX_LINE_SIZE];
	CHCommand command;
	bool isTurnChanged = true;
	int winner,i,j;
	SPWindow* window;
	SDL_Event event;
	GameBoard* castData;
	BoardCell* cellData;
	BoardCell* prevCellData;
	Widget *widget = NULL;
	CHGame *game = startSettingsMode();
	if(!game)
		return 0;
	if(GUI_ACTIVE){
		window = createSimpleWindow(game);
		if (window == NULL ) {
			SDL_Quit();
			return NULL;
		}
	}
		int flag = 1;
		while(true){
			command.cmd = CH_INVALID_LINE;
			if(game->gameMode == 2 || (game->gameMode == 1 && game->currentTurn == game->userColor)){
				if(isTurnChanged){
					chGamePrintBoard(game);
					if(GUI_ACTIVE){
						if(game->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL)
							printf("white player - enter your move:\n");
						else
							printf("black player - enter your move:\n");
					}
				}
				if(!(GUI_ACTIVE)){
					if(game->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL)
						printf("white player - enter your move:\n");
					else
						printf("black player - enter your move:\n");
				}
				if (GUI_ACTIVE) {
					SDL_WaitEvent(&event);
					if (event.type == SDL_QUIT) {
						command.cmd = CH_QUIT;
					}
					if (event.type == SDL_USEREVENT&& event.user.code == EVENT_MOVE) {
						command.cmd = CH_MOVE;
						command.validArg = true;
						GameBoard* castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
						BoardCell* cellData;
						for (i = 0; i < 8; i++) {
							for (j = 0; j < 8; j++) {
								cellData = (BoardCell*) castData->gameBoard[i][j]->data;
								if (cellData->isChosenByUser) {
									command.toRow = i;
									command.toColomn = j;
									cellData->isChosenByUser = 0;
								}
							}
						}
					}
					else if (event.type == SDL_USEREVENT&& event.user.code == EVENT_BEGIN_DRAG) {
						GameBoard* castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
						for (i = 0; i < 8; i++) {
							for (j = 0; j < 8; j++) {
								if (((BoardCell*) castData->gameBoard[i][j]->data)->isChosenByUser) {
									command.fRow = i;
									command.fColomn = j;
									((BoardCell*) castData->gameBoard[i][j]->data)->isChosenByUser = 0;
								}
							}
						}
					}else if (event.type == SDL_USEREVENT&& event.user.code == EVENT_GET_MOVES) {
						command.cmd = CH_GET_MOVES;
						command.validArg = true;
						GameBoard* castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
						for (i = 0; i < 8; i++) {
							for (j = 0; j < 8; j++) {
								if (((BoardCell*) castData->gameBoard[i][j]->data)->isChosenByUser) {
									command.fRow = i;
									command.fColomn = j;
									((BoardCell*) castData->gameBoard[i][j]->data)->isChosenByUser = 0;
								}
							}
						}
					}
			    } else {
				   fgets(strCommand, MAX_LINE_SIZE, stdin);
				   command = chParserPraseLine(strCommand);
			    }
				isTurnChanged = false;
				if (command.cmd == CH_MOVE) {
					if (command.validArg) {
						CH_GAME_MESSAGE mes = chGameSetMove(game, command.fRow,command.fColomn, command.toRow, command.toColomn);
						if(GUI_ACTIVE){
							castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
							cellData = (BoardCell*)castData->gameBoard[command.toRow][command.toColomn]->data;
							prevCellData = (BoardCell*) castData->gameBoard[command.fRow][command.fColomn]->data;
						}
						if (mes == CH_GAME_INVALID_COLOR) {
							printf("The specified position does not contain your piece\n");
							if(GUI_ACTIVE){
								((CHPiece*)(prevCellData->piece->data))->location->x = prevCellData->location->x;
								((CHPiece*)(prevCellData->piece->data))->location->y = prevCellData->location->y;
								window->drawWindow(window);
							}
						} else if (mes == CH_GAME_INVALID_MOVE) {
							printf("Illegal move\n");
							if(GUI_ACTIVE){
								((CHPiece*)(prevCellData->piece->data))->location->x = prevCellData->location->x;
								((CHPiece*)(prevCellData->piece->data))->location->y = prevCellData->location->y;
								window->drawWindow(window);
							}
						} else {
							if(GUI_ACTIVE){
								if (cellData->piece != NULL ) {
									cellData->piece->destroyWidget(cellData->piece);
								}
							    cellData->piece = createCHPiece(cellData->windowRenderer,cellData->location, game->gameBoard[command.toRow][command.toColomn]);
							    prevCellData->piece->destroyWidget(prevCellData->piece);
								prevCellData->piece = NULL;
								setNoGlowCells(((SPSimpleWindow *) window->data)->widgets[6]);
								window->drawWindow(window);
							}
							if (game->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
								game->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
								isTurnChanged = true;
							} else {
								game->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
								isTurnChanged = true;
							}
							winner = chIsCheckmateOrTie(game);
							if (winner == CH_GAME_INVALID_ARGUMENT)
								return -1;
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
						if(GUI_ACTIVE){
							castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
							cellData = (BoardCell*)castData->gameBoard[command.fRow][command.fColomn]->data;
							setNoGlowCells(((SPSimpleWindow *) window->data)->widgets[6]);
						}
						if(GUI_ACTIVE){
							widget = ((SPSimpleWindow *) window->data)->widgets[6];
						}
						CH_GAME_MESSAGE mes = chGameShowMoves(game, command.fRow,command.fColomn,widget);
						if (mes == CH_GAME_INVALID_COLOR) {
							printf("The specified position does not contain your piece\n");
							if(GUI_ACTIVE){
								window->drawWindow(window);
							}
						} else if (mes == CH_GAME_INVALID_MOVE) {
							printf("Illegal move\n");
						}else{
							if(GUI_ACTIVE){
								cellData->glow = CELL_GLOW_COLOR_REGULAR;
								window->drawWindow(window);
							}
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
					if(GUI_ACTIVE){
					    destroyWindow(window);
						SDL_Quit();
					}
					return 0;
				}
			}
			else{
			}
			if(GUI_ACTIVE){
				window->handleEventWindow(window, &event);
				if (flag == 1) {
					window->drawWindow(window);
					flag = 0;
				}
			}
	}
	return 0;
}
