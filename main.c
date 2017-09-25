#include "SEMode.h"
#include "CHGame.h"
#include "CHParser.h"
#include "gameBoard.h"
#include "CHMiniMax.h"
#include "save&load.h"
#define REC_DEPTH 4


char *getPieceName(char piece) {
    switch (piece) {
        case 'm':
        case 'M':
            return "pawn";
        case 'r':
        case 'R':
            return "rook";
        case 'n':
        case 'N':
            return "knight";
        case 'b':
        case 'B':
            return "bishop";
        case 'q':
        case 'Q':
            return "queen";
        case 'k':
        case 'K':
            return "king";
    }
    return "";
}


char *getPlayerName(int player) {
    if (player != 0)
        return "black";
    return "white";
}


void change_turn(CHGame* game,  bool *isTurnChanged) {
    if (game->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
        game->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
    } else {
        game->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
    }
    *isTurnChanged = true;
}


int end_of_move(CHGame* game, CHMoveNode* best_move, bool *isTurnChanged) {
    int winner;
    change_turn(game, isTurnChanged);
    winner = chIsCheckmateOrTie(game);
    if (winner == CH_GAME_INVALID_ARGUMENT) {
        chGameDestroy(game);
        free(best_move);
        return -1;
    } else if (winner != CH_GAME_NO_WIN_OR_TIE) {
        if (winner == CH_GAME_WHITE_WINS)
            printf("Checkmate! white player wins the game\n");
        else if (winner == CH_GAME_BLACK_WINS)
            printf("Checkmate! black player wins the game\n");
        else {
            printf("The game is tied\n");
        }
        chGameDestroy(game);
        free (best_move);
        return -1;
    } else {
        if ((game->gameMode == 1) && (game->currentTurn != game->userColor)) {
            if (isCheck(game, 1) == CH_GAME_INVALID_ARGUMENT) {
                chGameDestroy(game);
                free(best_move);
                return -1;
            }
        } else {
            if (isCheck(game, 0) == CH_GAME_INVALID_ARGUMENT) {
                chGameDestroy(game);
                free (best_move);
                return -1;
            }
        }
    }
    return 1;
}


void printTurn(CHGame *src) {
	if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL)
		printf("white player - enter your move:\n");
	else
		printf("black player - enter your move:\n");
}


int main(int argc, char** argv) {
	SP_BUFF_SET();
	CH_GAME_MESSAGE mes;
    int flag, i, j;
	char strCommand[MAX_LINE_SIZE];
	CHMoveNode *best_move = (CHMoveNode *) malloc(sizeof(CHMoveNode));
    if (!best_move)
        return -1;
	CHCommand command;
	bool isTurnChanged = true;
	int i,j;
	SPWindow* window;
	SDL_Event event;
	GameBoard* castData;
	BoardCell* cellData;
	BoardCell* prevCellData;
	SPSimpleWindow * simpleWindow = NULL;
	Widget *widget = NULL;
	CHGame* tmp = NULL;
	CHGame *game = startSettingsMode();
	if (!game)
		return 0;
	if (GUI_ACTIVE) {
		window = createSimpleWindow(game);
		if (window == NULL ) {
			SDL_Quit();
			return NULL;
		}
		simpleWindow =(SPSimpleWindow *)window->data;
	}
	flag = 1;
	while (true) {
		command.cmd = CH_INVALID_LINE;
		if (game->gameMode == 2 || (game->gameMode == 1 && game->currentTurn == game->userColor)) {
			if (isTurnChanged) {
				chGamePrintBoard(game);
				if (GUI_ACTIVE)
					printTurn(game);
			}
			if (!(GUI_ACTIVE))
				printTurn(game);
			if (GUI_ACTIVE) {
				SDL_WaitEvent(&event);
				if (event.type == SDL_QUIT) {
					command.cmd = CH_QUIT;
				}
				if (event.type == SDL_USEREVENT && event.user.code == EVENT_MOVE) {
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
				} else if (event.type == SDL_USEREVENT&& event.user.code == EVENT_DRAGGED_NOT_ON_BOARD) {
					command.cmd = CH_MOVE;
					command.validArg = false;
					nulifeDrag(((SPSimpleWindow *) window->data)->widgets[6]);
				} else if (event.type
						== SDL_USEREVENT&& event.user.code == EVENT_BEGIN_DRAG) {
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
				} else if (event.type == SDL_USEREVENT&& event.user.code == EVENT_GET_MOVES) {
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
				} else if (event.type == SDL_USEREVENT && event.user.code == EVENT_UNDO) {
					command.cmd = CH_UNDO;
					command.validArg = true;
				} else if (event.type == SDL_USEREVENT && event.user.code == EVENT_GO_TO_MAIN_MENU) {
					command.cmd = CH_RESET;
					command.validArg = true;
				} else if (event.type == SDL_USEREVENT && event.user.code == EVENT_RESTART) {
					tmp = chGameCreate(game->gameMode,game->userColor,game->difficulty,1);
					if ((game->gameMode == 1) && (game->list->actualSize != 0)) {
						updateTextureBtn(((SPSimpleWindow *) window->data)->widgets[0],0);
					}
					chGameDestroy(game);
					game = tmp;
					castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
					for(i = 0; i < 8; i++) {
						for(j = 0; j < 8; j++) {
							cellData = (BoardCell*) castData->gameBoard[i][j]->data;
							if (cellData->piece != NULL) {
								cellData->piece->destroyWidget(cellData->piece);
							}
							if (game->gameBoard[i][j] != CH_GAME_EMPTY_ENTRY) {
								cellData->piece = createCHPiece(cellData->windowRenderer,cellData->location,game->gameBoard[i][j]);
							} else {
								cellData->piece = NULL;
							}
						}
					}
					window->drawWindow(window);
				} else if (event.type == SDL_USEREVENT && event.user.code == EVENT_SAVE) {
					chGuiSave(game);
				} else if (event.type == SDL_USEREVENT && event.user.code == EVENT_LOAD_WINDOW) {
					destroyWindow(window);
					window = createLoadWindow();
					simpleWindow =(SPSimpleWindow *)window->data;
					SDL_WaitEvent(&event);
				} else if (event.user.code == EVENT_SET_SLOT_1) {
					pressSlotChange(window,1);
					if (!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)) {
						updateTextureBtn(simpleWindow->widgets[1],1);
					}
				} else if (event.user.code == EVENT_SET_SLOT_2) {
					pressSlotChange(window,2);
					if(!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)) {
						updateTextureBtn(simpleWindow->widgets[1],1);
					}
				} else if (event.user.code == EVENT_SET_SLOT_3) {
					pressSlotChange(window,3);
					if(!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)) {
						updateTextureBtn(simpleWindow->widgets[1],1);
					}
				} else if (event.user.code == EVENT_SET_SLOT_4) {
					pressSlotChange(window,4);
					if(!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)) {
						updateTextureBtn(simpleWindow->widgets[1],1);
					}
				} else if (event.user.code == EVENT_SET_SLOT_5) {
					pressSlotChange(window,5);
					if (!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)) {
						updateTextureBtn(simpleWindow->widgets[1],1);
					}
				}
			} else {
				fgets(strCommand, MAX_LINE_SIZE, stdin);
				command = chParserPraseLine(strCommand);
			}
			isTurnChanged = false;
			if (command.cmd == CH_MOVE) {
				if (command.validArg) {
					mes = chGameSetMove(game, game->gameBoard[command.fRow][command.fColomn],command.fRow,command.fColomn, command.toRow, command.toColomn,false);
					if (GUI_ACTIVE) {
						castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
						cellData = (BoardCell*) castData->gameBoard[command.toRow][command.toColomn]->data;
						prevCellData = (BoardCell*) castData->gameBoard[command.fRow][command.fColomn]->data;
					}
					if (mes == CH_GAME_INVALID_COLOR) {
						printf("The specified position does not contain your piece\n");
						if (GUI_ACTIVE) {
							((CHPiece*) (prevCellData->piece->data))->location->x = prevCellData->location->x;
							((CHPiece*) (prevCellData->piece->data))->location->y = prevCellData->location->y;
							window->drawWindow(window);
							nulifeDrag(((SPSimpleWindow *) window->data)->widgets[6]);
						}
					} else if (mes == CH_GAME_INVALID_MOVE) {
						printf("Illegal move\n");
						if (GUI_ACTIVE) {
							((CHPiece*) (prevCellData->piece->data))->location->x = prevCellData->location->x;
							((CHPiece*) (prevCellData->piece->data))->location->y = prevCellData->location->y;
							window->drawWindow(window);
							nulifeDrag(((SPSimpleWindow *) window->data)->widgets[6]);
						}
					} else {
						if (GUI_ACTIVE) {
							if (cellData->piece != NULL ) {
								cellData->piece->destroyWidget(cellData->piece);
							}
							cellData->piece = createCHPiece(cellData->windowRenderer,cellData->location,game->gameBoard[command.toRow][command.toColomn]);
							prevCellData->piece->destroyWidget(prevCellData->piece);
							prevCellData->piece = NULL;
							setNoGlowCells(((SPSimpleWindow *) window->data)->widgets[6]);

							window->drawWindow(window);
						}
                        if (end_of_move(game, best_move, &isTurnChanged) == -1) {
            				if (GUI_ACTIVE) {
            					destroyWindow(window);
            					SDL_Quit();
            				}
                            return 0;
                        }
					}
				} else {
					printf("Invalid position on the board\n");
					if (GUI_ACTIVE) {
						castData =
								(GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
						prevCellData =
								(BoardCell*) castData->gameBoard[command.fRow][command.fColomn]->data;
						((CHPiece*) (prevCellData->piece->data))->location->x =
								prevCellData->location->x;
						((CHPiece*) (prevCellData->piece->data))->location->y =
								prevCellData->location->y;
						window->drawWindow(window);
					}
				}
			} else if (command.cmd == CH_GET_MOVES) {
				if (command.validArg) {
					if (GUI_ACTIVE) {
						castData =
								(GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
						cellData =
								(BoardCell*) castData->gameBoard[command.fRow][command.fColomn]->data;
						setNoGlowCells(((SPSimpleWindow *) window->data)->widgets[6]);
					}
					if (GUI_ACTIVE) {
						widget = ((SPSimpleWindow *) window->data)->widgets[6];
					}
					CH_GAME_MESSAGE mes = chGameShowMoves(game, command.fRow,
							command.fColomn, widget);
					if (mes == CH_GAME_INVALID_COLOR) {
						printf(
								"The specified position does not contain your piece\n");
						if (GUI_ACTIVE) {
							window->drawWindow(window);
						}
					} else if (mes == CH_GAME_INVALID_MOVE) {
						printf("Illegal move\n");
					} else {
						if (GUI_ACTIVE) {
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
				if (GUI_ACTIVE) {
					destroyWindow(window);
				}
				game = startSettingsMode();
				if (!game) {
					free(best_move);
					if (GUI_ACTIVE) {
						SDL_Quit();
					}
					return -1;
				}
				if (GUI_ACTIVE) {
					window = createSimpleWindow(game);
					if (window == NULL ) {
						chGameDestroy(game);
						SDL_Quit();
						return -1;
					}
					SDL_WaitEvent(&event);
				}
				if (!(GUI_ACTIVE)) {
					printf("Restarting...\n");
				}
				isTurnChanged = true;
			} else if (command.cmd == CH_QUIT) {
				chGameDestroy(game);
				printf("Exiting...\n");
				if (GUI_ACTIVE) {
					destroyWindow(window);
					SDL_Quit();
				}
				return 0;
			} else if (command.cmd == CH_UNDO) {
                if (game->gameMode == 2) {
                    printf("Undo command not available in 2 players mode\n");
                } else {
                    if (game->list->actualSize >= 2){
                        *best_move = spArrayListGetFirst(game->list);
                        mes = chGameUndoPrevMove(game);
                        if (mes == CH_GAME_SUCCESS) {
                            printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
                                   getPlayerName(game->userColor),
                                   (best_move->to_row + 1),
                                   best_move->to_col + 65,
                                   (best_move->from_row + 1),
                                   best_move->from_col + 65);
                            if (GUI_ACTIVE) {
        						castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
        						cellData = (BoardCell*) castData->gameBoard[best_move->to_row][best_move->to_col]->data;
        						prevCellData = (BoardCell*) castData->gameBoard[best_move->from_row][best_move->from_col]->data;
        						prevCellData->piece = createCHPiece(prevCellData->windowRenderer,prevCellData->location,best_move->current_piece);
        						cellData->piece->destroyWidget(cellData->piece);
         						cellData->piece = NULL;
        						if (best_move->piece_eaten != CH_GAME_EMPTY_ENTRY) {
        							cellData->piece = createCHPiece(cellData->windowRenderer,cellData->location,best_move->piece_eaten);
        						}
        						setNoGlowCells(((SPSimpleWindow *) window->data)->widgets[6]);
        						window->drawWindow(window);
                            }
                            *best_move = spArrayListGetFirst(game->list);
                            CH_GAME_MESSAGE mes = chGameUndoPrevMove(game);
                            if (mes == CH_GAME_SUCCESS) {
                                printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
                                       getPlayerName(!game->userColor),
                                       (best_move->to_row + 1),
                                       best_move->to_col + 65,
                                       (best_move->from_row + 1),
                                       best_move->from_col + 65);
                                if (GUI_ACTIVE) {
            						castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
            						cellData = (BoardCell*) castData->gameBoard[best_move->to_row][best_move->to_col]->data;
            						prevCellData = (BoardCell*) castData->gameBoard[best_move->from_row][best_move->from_col]->data;
            						prevCellData->piece = createCHPiece(prevCellData->windowRenderer,prevCellData->location,best_move->current_piece);
            						cellData->piece->destroyWidget(cellData->piece);
             						cellData->piece = NULL;
            						if (best_move->piece_eaten != CH_GAME_EMPTY_ENTRY) {
            							cellData->piece = createCHPiece(cellData->windowRenderer,cellData->location,best_move->piece_eaten);
            						}
            						setNoGlowCells(((SPSimpleWindow *) window->data)->widgets[6]);
            						if(game->list->actualSize == 0){
            							updateTextureBtn(((SPSimpleWindow *) window->data)->widgets[0],0);
            						}
            						window->drawWindow(window);
                                }
                            }
                        }
                        isTurnChanged = true;
                    } else {
                        printf("Empty history, move cannot be undone\n");
                        isTurnChanged = false;
                    }
                }
            }
		} else {
			mes = alphabeta(chGameCopy(game), game->difficulty,game->currentTurn, best_move);
			chGameSetMove(game,game->gameBoard[best_move->from_row][best_move->from_col],best_move->from_row, best_move->from_col,best_move->to_row, best_move->to_col, true);
			if (GUI_ACTIVE) {
				castData = (GameBoard*) (((SPSimpleWindow *) window->data)->widgets[6]->data);
				prevCellData = (BoardCell*) castData->gameBoard[best_move->from_row][best_move->from_col]->data;
				cellData = (BoardCell*) castData->gameBoard[best_move->to_row][best_move->to_col]->data;
				if (cellData->piece != NULL ) {
					cellData->piece->destroyWidget(cellData->piece);
				}
				cellData->piece = createCHPiece(cellData->windowRenderer,cellData->location,game->gameBoard[best_move->to_row][best_move->to_col]);
				prevCellData->piece->destroyWidget(prevCellData->piece);
				prevCellData->piece = NULL;
				setNoGlowCells(((SPSimpleWindow *) window->data)->widgets[6]);
				if (game->list->actualSize == 2) {
					updateTextureBtn(((SPSimpleWindow *) window->data)->widgets[0],1);
				}
				window->drawWindow(window);
			}
			printf("Computer: move %s at <%d,%c> to <%d,%c>\n",
					getPieceName(best_move->current_piece),
					best_move->from_row + 1, best_move->from_col + 65,
					best_move->to_row + 1, best_move->to_col + 65);
			isTurnChanged = true;
			if (end_of_move(game, best_move, &isTurnChanged) == -1) {
				if (GUI_ACTIVE) {
					destroyWindow(window);
					SDL_Quit();
				}
				return 0;
			}
		}
		if (GUI_ACTIVE) {
			window->handleEventWindow(window, &event);
			if (flag == 1) {
				window->drawWindow(window);
				flag = 0;
			}
		}
	}
	return 0;
}
