/*
 * CHMainAux.c
 *
 *  Created on: Sep 24, 2017
 *      Author: sasha
 */
#include <stdio.h>
#include <stdlib.h>
#include "CHMainAux.h"

void setGuiMode(int argc, char** argv, bool *isGuiMode){
	if (argc >= 1 && argc <3) {
		if(argc == 2){
			if(strcmp(argv[1],"-g") == 0){
				*isGuiMode = true;
			}else if(strcmp(argv[1],"-c") == 0){
				*isGuiMode = false;
			}else{
				exit(0);
			}
		}
	}
}

char *getPieceName(char piece) {
	switch (piece) {
	case CH_BLACK_PAWN:
	case CH_WHITE_PAWN:
		return "pawn";
	case CH_BLACK_ROOK:
	case CH_WHITE_ROOK:
		return "rook";
	case CH_BLACK_KNIGHT:
	case CH_WHITE_KNIGHT:
		return "knight";
	case CH_BLACK_BISHOP:
	case CH_WHITE_BISHOP:
		return "bishop";
	case CH_BLACK_QUEEN:
	case CH_WHITE_QUEEN:
		return "queen";
	case CH_BLACK_KING:
	case CH_WHITE_KING:
		return "king";
	}
	return "";
}

char *getPlayerName(int player) {
	if (player != 0)
		return "black";
	return "white";
}

void change_turn(CHGame* game, bool *isTurnChanged) {
	if (game->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
		game->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
	} else {
		game->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
	}
	*isTurnChanged = true;
}

int end_of_move(CHGame* game, CHMoveNode* best_move, bool *isTurnChanged,bool isGuiMode) {
	int winner;
	change_turn(game, isTurnChanged);
	winner = chIsCheckmateOrTie(game);
	if (winner == CH_GAME_INVALID_ARGUMENT) {
		free(best_move);
		return -1;
	} else if (winner != CH_GAME_NO_WIN_OR_TIE) {
		if (winner == CH_GAME_WHITE_WINS) {
			printf("Checkmate! white player wins the game\n");
			if (isGuiMode) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info",
						"Checkmate! white player wins the game", NULL );
			}
		} else if (winner == CH_GAME_BLACK_WINS) {
			printf("Checkmate! black player wins the game\n");
			if (isGuiMode) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info",
						"Checkmate! black player wins the game", NULL );
			}
		} else {
			printf("The game is tied\n");
			if (isGuiMode) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info",
						"The game is tied", NULL );
			}
		}
		free(best_move);
		return -1;
	} else {
		if ((game->gameMode == 1) && (game->currentTurn == game->userColor)) {
			if (isCheck(game, 1, isGuiMode) == CH_GAME_INVALID_ARGUMENT) {
				free(best_move);
				return -1;
			}
		} else {
			if (isCheck(game, 0, isGuiMode) == CH_GAME_INVALID_ARGUMENT) {
				free(best_move);
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

void undoGuiHandle(CHGame* game, SPWindow* window, CHMoveNode *best_move) {
	SPSimpleWindow *simpleWindow = (SPSimpleWindow *) window->data;
	GameBoard* board = (GameBoard*) (simpleWindow->widgets[6]->data);
	BoardCell* cellData =
			(BoardCell*) board->gameBoard[best_move->to_row][best_move->to_col]->data;
	BoardCell* prevCellData =
			(BoardCell*) board->gameBoard[best_move->from_row][best_move->from_col]->data;
	prevCellData->piece = createCHPiece(prevCellData->windowRenderer,
			prevCellData->location, best_move->current_piece);
	cellData->piece->destroyWidget(cellData->piece);
	cellData->piece = NULL;
	if (best_move->piece_eaten != CH_GAME_EMPTY_ENTRY) {
		cellData->piece = createCHPiece(cellData->windowRenderer,
				cellData->location, best_move->piece_eaten);
	}
	setNoGlowCells(simpleWindow->widgets[6]);
	if (game->list->actualSize == 0)
		updateTextureBtn(simpleWindow->widgets[0], 0);
	window->drawWindow(window);
}

void undoHandle(CHGame* game, SPWindow* window, CHMoveNode *best_move,
		bool *isTurnChanged,bool isGuiMode) {
	CH_GAME_MESSAGE mes;
	if (game->gameMode == 2) {
		printf("Undo command not available in 2 players mode\n");
	} else if (game->list->actualSize >= 2) {
		*best_move = spArrayListGetFirst(game->list);
		mes = chGameUndoPrevMove(game);
		if (mes == CH_GAME_SUCCESS) {
			printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
					getPlayerName(game->userColor), (best_move->to_row + 1),
					best_move->to_col + 65, (best_move->from_row + 1),
					best_move->from_col + 65);
			if (isGuiMode) {
				undoGuiHandle(game, window, best_move);
			}
			*best_move = spArrayListGetFirst(game->list);
			CH_GAME_MESSAGE mes = chGameUndoPrevMove(game);
			if (mes == CH_GAME_SUCCESS) {
				printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
						getPlayerName(!game->userColor),
						(best_move->to_row + 1), best_move->to_col + 65,
						(best_move->from_row + 1), best_move->from_col + 65);
				if (isGuiMode) {
					undoGuiHandle(game, window, best_move);
				}
			}
		}
		*isTurnChanged = true;
	} else {
		printf("Empty history, move cannot be undone\n");
		*isTurnChanged = false;
	}
}

void returnPieceToPlase(BoardCell* prevCellData, Widget *widgetGameBoard,
		SPWindow* window,bool isGuiMode) {
	CHPiece* piece;
	if (isGuiMode) {
		piece = (CHPiece*) prevCellData->piece->data;
		piece->location->x = prevCellData->location->x;
		piece->location->y = prevCellData->location->y;
		window->drawWindow(window);
		nulifeDrag(widgetGameBoard);
	}
}

void handleMoveCommand(CHGame* game, SPWindow* window, CHMoveNode *best_move,
		CHCommand command, bool *isTurnChanged, bool *isSaved,bool isGuiMode) {
	GameBoard* gameBoard;
	BoardCell* cellData;
	BoardCell* prevCellData;
	SPSimpleWindow *simpleWindow;
	Widget *widgetGameBoard = NULL;
	CH_GAME_MESSAGE mes;
	if (isGuiMode) {
		simpleWindow = (SPSimpleWindow *) window->data;
		widgetGameBoard = simpleWindow->widgets[6];
		gameBoard = (GameBoard*) widgetGameBoard->data;
		prevCellData =
				(BoardCell*) gameBoard->gameBoard[command.fRow][command.fColomn]->data;
	}
	if (command.validArg) {
		mes = chGameSetMove(game,
				game->gameBoard[command.fRow][command.fColomn], command.fRow,
				command.fColomn, command.toRow, command.toColomn, false, isGuiMode);
		if (isGuiMode) {
			cellData =
					(BoardCell*) gameBoard->gameBoard[command.toRow][command.toColomn]->data;
		}
		if (mes == CH_GAME_INVALID_COLOR || mes == CH_GAME_INVALID_MOVE) {
			if (mes == CH_GAME_INVALID_COLOR)
				printf("The specified position does not contain your piece\n");
			else
				printf("Illegal move\n");
			returnPieceToPlase(prevCellData, widgetGameBoard, window, isGuiMode);
		} else {
			if (isGuiMode) {
				if (cellData->piece != NULL ) {
					cellData->piece->destroyWidget(cellData->piece);
				}
				cellData->piece = createCHPiece(cellData->windowRenderer,
						cellData->location,
						game->gameBoard[command.toRow][command.toColomn]);
				prevCellData->piece->destroyWidget(prevCellData->piece);
				prevCellData->piece = NULL;
				setNoGlowCells(widgetGameBoard);
				window->drawWindow(window);
			}
			*isSaved = false;
			if (end_of_move(game, best_move, isTurnChanged, isGuiMode) == -1) {
				chGameDestroy(game);
				if (isGuiMode) {
					destroyWindow(window);
					SDL_Quit();
				}
				exit(1);
			}
		}
	} else {
		printf("Invalid position on the board\n");
		returnPieceToPlase(prevCellData, widgetGameBoard, window, isGuiMode);
	}
}

void handleGetMovesCommand(CHGame* game, SPWindow* window, CHCommand command,bool isGuiMode) {
	GameBoard* gameBoard;
	BoardCell* cellData;
	SPSimpleWindow *simpleWindow;
	Widget *widgetGameBoard = NULL;
	CH_GAME_MESSAGE mes;
	if (command.validArg) {
		if (isGuiMode) {
			simpleWindow = (SPSimpleWindow *) window->data;
			widgetGameBoard = simpleWindow->widgets[6];
			gameBoard = (GameBoard*) widgetGameBoard->data;
			cellData =
					(BoardCell*) gameBoard->gameBoard[command.fRow][command.fColomn]->data;
			setNoGlowCells(widgetGameBoard);
		}
		mes = chGameGetMoves(game, command.fRow, command.fColomn,
				widgetGameBoard, isGuiMode);
		if (mes == CH_GAME_INVALID_COLOR) {
			printf("The specified position does not contain your piece\n");
			if (isGuiMode) {
				window->drawWindow(window);
			}
		} else if (mes == CH_GAME_INVALID_MOVE) {
			printf("Illegal move\n");
		} else {
			if (isGuiMode) {
				cellData->glow = CELL_GLOW_COLOR_REGULAR;
				window->drawWindow(window);
			}
		}
	} else
		printf("Invalid position on the board\n");
}

void handleSlotEvent(SPWindow** window, int *draw, int slot) {
	SPWindow* windowP = *window;
	SPSimpleWindow * simpleWindow = (SPSimpleWindow *) windowP->data;
	pressSlotChange(windowP, slot);
	if (!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)) {
		updateTextureBtn(simpleWindow->widgets[1], 1);
	}
	*draw = 1;
}

void showSaveMessage(CHGame** game, SPWindow** window, CHCommand *command,
		bool *isSaved, int *draw) {
	SPWindow* windowP = *window;
	SPSimpleWindow * simpleWindow = (SPSimpleWindow *) windowP->data;
	int buttonid = 0;
	const SDL_MessageBoxButtonData buttons[] = { {
			SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" }, { /* .flags, .buttonid, .text */
	0, 0, "no" }, { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" }, };
	const SDL_MessageBoxColorScheme colorScheme = { { /* .colors (.r, .g, .b) */
	/* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
	{ 80, 0, 0 },
	/* [SDL_MESSAGEBOX_COLOR_TEXT] */
	{ 0, 255, 0 },
	/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
	{ 255, 255, 0 },
	/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
	{ 0, 0, 255 },
	/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
	{ 255, 0, 255 } } };
	const SDL_MessageBoxData messageboxdata = { SDL_MESSAGEBOX_INFORMATION, /* .flags */
	simpleWindow->window, /* .window */
	"", /* .title */
	"Do you want to save the game?", /* .message */
	SDL_arraysize(buttons), /* .numbuttons */
	buttons, /* .buttons */
	&colorScheme /* .colorScheme */
	};
	if (*isSaved != true) {
		if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
			SDL_Log("error displaying message box");
			return;
		}
		if (buttonid == 2) {
			command->cmd = CH_INVALID_LINE;
			command->validArg = false;
		} else if (buttonid == 1) {
			chGuiSave(*game);
			*isSaved = true;
		}
		SDL_FlushEvents(SDL_QUIT, SDL_USEREVENT);
	}
	*draw = 1;
}



void handleRestartEvent(CHGame** game, SPWindow** window, bool *isSaved) {
	CHGame* gameP = *game;
	int i, j;
	BoardCell* cellData;
	SPWindow* windowP = *window;
	SPSimpleWindow * simpleWindow = (SPSimpleWindow *) windowP->data;
	Widget * widgetGameBoard = simpleWindow->widgets[6];
	GameBoard* gameBoard = (GameBoard*) widgetGameBoard->data;
	CHGame* tmp = chGameCreate(gameP->gameMode, gameP->userColor,
			gameP->difficulty, 1);
	if ((gameP->gameMode == 1) && (gameP->list->actualSize != 0)) {
		updateTextureBtn(simpleWindow->widgets[0], 0);
	}
	chGameDestroy(gameP);
	*game = tmp;
	gameP = *game;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			cellData = (BoardCell*) gameBoard->gameBoard[i][j]->data;
			if (cellData->piece != NULL ) {
				cellData->piece->destroyWidget(cellData->piece);
			}
			if (gameP->gameBoard[i][j] != CH_GAME_EMPTY_ENTRY) {
				cellData->piece = createCHPiece(cellData->windowRenderer,
						cellData->location, gameP->gameBoard[i][j]);
			} else {
				cellData->piece = NULL;
			}
		}
	}
	setNoGlowCells(widgetGameBoard);
	*isSaved = true;
	windowP->drawWindow(windowP);
}

void setFromCommandCoordinates(SPWindow** window, CHCommand *command) {
	int i, j;
	BoardCell* cellData;
	SPWindow* windowP = *window;
	SPSimpleWindow * simpleWindow = (SPSimpleWindow *) windowP->data;
	Widget * widgetGameBoard = simpleWindow->widgets[6];
	GameBoard* gameBoard = (GameBoard*) widgetGameBoard->data;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			cellData = (BoardCell*) gameBoard->gameBoard[i][j]->data;
			if (cellData->isChosenByUser) {
				command->fRow = i;
				command->fColomn = j;
				cellData->isChosenByUser = 0;
			}
		}
	}
}

void handleLoadEvents(CHGame** game, SPWindow** window, int *draw) {
	int slot;
	SPSimpleWindow * simpleWindow = (SPSimpleWindow *) (*window)->data;
	if (simpleWindow->type == CH_WINDOW_MAIN) {
		destroyWindow(*window);
		*window = createLoadWindow();
	} else if (simpleWindow->type == CH_WINDOW_LOAD) {
		slot = getSlotPressed(*window);
		destroyWindow(*window);
		if (slot > 0) {
			chGameDestroy(*game);
			*game = (CHGame*) malloc(sizeof(CHGame)); /* allocate place in memory */
			if (chGuiLoad(*game, slot) != CH_GAME_SUCCESS) {
				exit(1);
			}
			*window = createSimpleWindow(*game);
		}
	}
	*draw = 1;
}

void handleBackEvent(CHGame** game, SPWindow** window, int *draw) {
	destroyWindow(*window);
	*window = createSimpleWindow(*game);
	SPSimpleWindow * simpleWindow = (SPSimpleWindow *) (*window)->data;
	if ((*game)->gameMode == 1 && (*game)->list->actualSize >= 2) {
		updateTextureBtn(simpleWindow->widgets[0], 1);
	}
	*draw = 1;
}

void handleMoveEvent(CHCommand *command, SPWindow** window) {
	int i, j;
	command->cmd = CH_MOVE;
	command->validArg = true;
	BoardCell* cellData = NULL;
	SPSimpleWindow * simpleWindow = (SPSimpleWindow *) (*window)->data;
	GameBoard* gameBoard = (GameBoard*) simpleWindow->widgets[6]->data;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			cellData = (BoardCell*) gameBoard->gameBoard[i][j]->data;
			if (cellData->isChosenByUser) {
				command->toRow = i;
				command->toColomn = j;
				cellData->isChosenByUser = 0;
			}
		}
	}
}

void handleMainEvents(CHGame** game, SPWindow** window, SDL_Event *event,
		CHCommand *command, bool *isSaved, int *draw) {
	SPSimpleWindow * simpleWindow = NULL;
	if (event->type == SDL_QUIT) {
		command->cmd = CH_QUIT;
		showSaveMessage(game, window, command, isSaved, draw);
	}
	if (event->type == SDL_USEREVENT) {
		if (event->user.code == EVENT_MOVE) {
			handleMoveEvent(command, window);
		} else if (event->user.code == EVENT_DRAGGED_NOT_ON_BOARD) {
			command->cmd = CH_MOVE;
			command->validArg = false;
			simpleWindow = (SPSimpleWindow *) (*window)->data;
			nulifeDrag(simpleWindow->widgets[6]);
		} else if (event->user.code == EVENT_BEGIN_DRAG) {
			setFromCommandCoordinates(window, command);
		} else if (event->user.code == EVENT_GET_MOVES) {
			command->cmd = CH_GET_MOVES;
			command->validArg = true;
			setFromCommandCoordinates(window, command);
		} else if (event->user.code == EVENT_UNDO) {
			command->cmd = CH_UNDO;
			command->validArg = true;
		} else if (event->user.code == EVENT_GO_TO_MAIN_MENU) {
			command->cmd = CH_RESET;
			command->validArg = true;
			showSaveMessage(game, window, command, isSaved, draw);
		} else if (event->user.code == EVENT_RESTART) {
			handleRestartEvent(game, window, isSaved);
		} else if (event->user.code == EVENT_SAVE) {
			chGuiSave(*game);
			*isSaved = true;
		} else if (event->user.code == EVENT_LOAD_WINDOW) {
			handleLoadEvents(game, window, draw);
			SDL_WaitEvent(event);
		} else if (event->user.code == EVENT_SET_SLOT_1) {
			handleSlotEvent(window, draw, 1);
		} else if (event->user.code == EVENT_SET_SLOT_2) {
			handleSlotEvent(window, draw, 2);
		} else if (event->user.code == EVENT_SET_SLOT_3) {
			handleSlotEvent(window, draw, 3);
		} else if (event->user.code == EVENT_SET_SLOT_4) {
			handleSlotEvent(window, draw, 4);
		} else if (event->user.code == EVENT_SET_SLOT_5) {
			handleSlotEvent(window, draw, 5);
		} else if (event->user.code == EVENT_BACK) {
			handleBackEvent(game, window, draw);
			SDL_WaitEvent(event);
		}
	}
}

void computerTurn(CHGame* game, SPWindow* window, CHMoveNode *best_move,
		bool *isSaved, bool *isTurnChanged, bool isGuiMode) {
	bool is_pawn_promotion = false;
    GameBoard* castData;
	BoardCell* cellData;
	BoardCell* prevCellData;
	SPSimpleWindow * simpleWindow;
	CHGame *copyGame= chGameCopy(game);
	CH_GAME_MESSAGE mes = alphabeta(copyGame, game->difficulty,
			game->currentTurn, best_move, isGuiMode);
	chGameDestroy(copyGame);
	if (mes != CH_GAME_SUCCESS) {
		if (isGuiMode) {
			destroyWindow(window);
			SDL_Quit();
		}
		exit(0);
	}
    if (best_move->current_piece != game->gameBoard[best_move->from_row][best_move->from_col]) {
        is_pawn_promotion = true;
    }
	chGameSetMove(game,
			best_move->current_piece,
			best_move->from_row, best_move->from_col, best_move->to_row,
			best_move->to_col, true, isGuiMode);
	if (isGuiMode) {
		simpleWindow = (SPSimpleWindow *) window->data;
		castData = (GameBoard*) (simpleWindow->widgets[6]->data);
		prevCellData =
				(BoardCell*) castData->gameBoard[best_move->from_row][best_move->from_col]->data;
		cellData =
				(BoardCell*) castData->gameBoard[best_move->to_row][best_move->to_col]->data;
		if (cellData->piece != NULL ) {
			cellData->piece->destroyWidget(cellData->piece);
		}
		cellData->piece = createCHPiece(cellData->windowRenderer,
				cellData->location,
				game->gameBoard[best_move->to_row][best_move->to_col]);
		prevCellData->piece->destroyWidget(prevCellData->piece);
		prevCellData->piece = NULL;
		setNoGlowCells(simpleWindow->widgets[6]);
		if (game->list->actualSize == 2) {
			updateTextureBtn(simpleWindow->widgets[0], 1);
		}
		*isSaved = false;
		window->drawWindow(window);
	}
    if (is_pawn_promotion) {
        printf("Computer: move pawn <%d,%c> to <%d,%c> and promote to %s\n",
               best_move->from_row + 1, best_move->from_col + 65,
               best_move->to_row + 1, best_move->to_col + 65,
               getPieceName(best_move->current_piece));
    } else {
        printf("Computer: move %s at <%d,%c> to <%d,%c>\n",
               getPieceName(best_move->current_piece), best_move->from_row + 1,
               best_move->from_col + 65, best_move->to_row + 1,
               best_move->to_col + 65);
    }
	*isTurnChanged = true;
	if (end_of_move(game, best_move, isTurnChanged, isGuiMode) == -1) {
		if (isGuiMode) {
			destroyWindow(window);
			SDL_Quit();
		}
		exit(0);
	}
}

void handleResetCommand(CHGame **game, SPWindow** window, CHMoveNode *best_move,
		SDL_Event *event, bool *isTurnChanged,bool isGuiMode) {
	chGameDestroy(*game);
	if (isGuiMode) {
		destroyWindow(*window);
	}
	*game = startSettingsMode(isGuiMode);
	if (!(*game)) {
		free(best_move);
		if (isGuiMode) {
			SDL_Quit();
		}
		exit(0);
	}
	if (isGuiMode) {
		*window = createSimpleWindow(*game);
		if (window == NULL ) {
			chGameDestroy(*game);
			SDL_Quit();
			exit(0);
		}
		SDL_WaitEvent(event);
	}
	if (!(isGuiMode)) {
		printf("Restarting...\n");
	}
	*isTurnChanged = true;
}

void handleAllCommands(CHCommand command, CHGame **game, SPWindow** window,
		bool *isSaved, bool *isTurnChanged, CHMoveNode *best_move,
		SDL_Event *event,bool isGuiMode) {
	if (command.cmd == CH_MOVE) {
		handleMoveCommand(*game, *window, best_move, command, isTurnChanged,
				isSaved, isGuiMode);
	} else if (command.cmd == CH_GET_MOVES) {
		handleGetMovesCommand(*game, *window, command, isGuiMode);
	} else if (command.cmd == CH_SAVE) {
		if (chGameSave(*game, command.path) == CH_GAME_FILE_PROBLEM)
			printf("File cannot be created or modified\n");
	} else if (command.cmd == CH_RESET) {
		handleResetCommand(game, window, best_move, event, isTurnChanged, isGuiMode);
	} else if (command.cmd == CH_QUIT) {
		chGameDestroy(*game);
		free(best_move);
		printf("Exiting...\n");
		if (isGuiMode) {
			destroyWindow(*window);
			SDL_Quit();
		}
		exit(0);
	} else if (command.cmd == CH_UNDO) {
		undoHandle(*game, *window, best_move, isTurnChanged, isGuiMode);
	}
}
