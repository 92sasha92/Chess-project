/*
 * CHFIARGame.c
 *
 *  Created on: Aug 25, 2017
 *      Author: sasha
 */

#include "CHGame.h"
#include "SPBufferset.h"
#include "graphics/SPSimpleMainWindow.h"
#include "graphics/gameBoard.h"
#include "CHMiniMax.h"



void showPawnPromotionMessage(CHGame* game, int row, int col) {
	int buttonId = 0;
	const SDL_MessageBoxButtonData buttons[] = { { 0, 0, "pawn" }, { 1, 1, "knight" }, { 2, 2, "bishop" }, { 3, 3, "rook" }, { 4, 4, "queen" } };
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
	NULL, /* .window */
	"Pawn promotion", /* .title */
	"Please replace the pawn by queen, rook, knight, bishop or pawn:", /* .message */
	SDL_arraysize(buttons), /* .numbuttons */
	buttons, /* .buttons */
	&colorScheme /* .colorScheme */
	};
	if (SDL_ShowMessageBox(&messageboxdata, &buttonId) < 0) {
		SDL_Log("error displaying message box");
		return;
	}
	if(game->gameBoard[row][col] == CH_WHITE_PAWN){
		switch(buttonId){
		case 0:
			game->gameBoard[row][col] = CH_WHITE_PAWN;
			break;
		case 1:
			game->gameBoard[row][col] = CH_WHITE_KNIGHT;
			break;
		case 2:
			game->gameBoard[row][col] = CH_WHITE_BISHOP;
			break;
		case 3:
			game->gameBoard[row][col] = CH_WHITE_ROOK;
			break;
		case 4:
			game->gameBoard[row][col] = CH_WHITE_QUEEN;
			break;
		}
	}else{
		switch(buttonId){
		case 0:
			game->gameBoard[row][col] = CH_BLACK_PAWN;
			break;
		case 1:
			game->gameBoard[row][col] = CH_BLACK_KNIGHT;
			break;
		case 2:
			game->gameBoard[row][col] = CH_BLACK_BISHOP;
			break;
		case 3:
			game->gameBoard[row][col] = CH_BLACK_ROOK;
			break;
		case 4:
			game->gameBoard[row][col] = CH_BLACK_QUEEN;
			break;
		}
	}
	SDL_FlushEvents(SDL_QUIT, SDL_USEREVENT);
}

void defaultBoard(CHGame *src) {
	int i, j;
	for (i = 0; i < CH_GAME_N_ROWS; i++) {
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			if (i == 0) {
				if (j == CH_GAME_N_COLUMNS - 1 || j == 0)
					src->gameBoard[i][j] = CH_WHITE_ROOK;
				if (j == CH_GAME_N_COLUMNS - 2 || j == 1)
					src->gameBoard[i][j] = CH_WHITE_KNIGHT;
				if (j == CH_GAME_N_COLUMNS - 3 || j == 2)
					src->gameBoard[i][j] = CH_WHITE_BISHOP;
				if (j == 3)
					src->gameBoard[i][j] = CH_WHITE_QUEEN;
				if (j == 4)
					src->gameBoard[i][j] = CH_WHITE_KING;
			} else if (i == CH_GAME_N_ROWS - 1) {
				if (j == CH_GAME_N_COLUMNS - 1 || j == 0)
					src->gameBoard[i][j] = CH_BLACK_ROOK;
				if (j == CH_GAME_N_COLUMNS - 2 || j == 1)
					src->gameBoard[i][j] = CH_BLACK_KNIGHT;
				if (j == CH_GAME_N_COLUMNS - 3 || j == 2)
					src->gameBoard[i][j] = CH_BLACK_BISHOP;
				if (j == 3)
					src->gameBoard[i][j] = CH_BLACK_QUEEN;
				if (j == 4)
					src->gameBoard[i][j] = CH_BLACK_KING;
			} else if (i == 1) {
				src->gameBoard[i][j] = CH_WHITE_PAWN;
			} else if (i == CH_GAME_N_ROWS - 2) {
				src->gameBoard[i][j] = CH_BLACK_PAWN;
			} else {
				src->gameBoard[i][j] = CH_GAME_EMPTY_ENTRY; /* restart the board with spaces */
			}
		}
	}
}

int chGameCreateMode1(CHGame* src, int difficulty, int userColor) {
	if (src->gameMode == 1) {
		if (userColor != CH_GAME_BLACK_PLAYER_SYMBOL
				&& userColor != CH_GAME_WHITE_PLAYER_SYMBOL)
			return 0;
		src->userColor = userColor;
		if (difficulty < 1 || 4 < difficulty)
			return 0;
		src->difficulty = difficulty;
		src->list = spArrayListCreate(6); /* every undo move include computer and user moves */
		if (!(src->list)) {
			free(src);
			printf("Error: malloc has failed\n");
			return 0;
		}
	} else {
		src->list = NULL;
		src->difficulty = 0;
	}
	return 1;
}

CHGame* chGameCreate(int gameMode, int userColor, int difficulty,
		int currentTurn) {
	CHGame* src = (CHGame*) malloc(sizeof(CHGame)); /* allocate place in memory */
	if (!src) {
		printf("Error: malloc has failed\n");
		return NULL ;
	}
	defaultBoard(src);
	if (gameMode != 1 && gameMode != 2) {
		free(src);
		return NULL ;
	}
	src->gameMode = gameMode;
	if (currentTurn != CH_GAME_BLACK_PLAYER_SYMBOL
			&& currentTurn != CH_GAME_WHITE_PLAYER_SYMBOL) {
		free(src);
		return NULL ;
	}
	src->currentTurn = currentTurn;
	if (!chGameCreateMode1(src, difficulty, userColor)) {
		free(src);
		return NULL ;
	}
	return src;
}

void chGameDestroy(CHGame* src) {
	if (src != NULL ) {
		if (src->gameMode == 1)
			spArrayListDestroy(src->list); /* free the history list memory */
		free(src); /* free the game memory */
	}
}

CH_GAME_MESSAGE chGamePrintBoard(CHGame* src) {
	int i, j;
	if (src == NULL ) {
		return CH_GAME_INVALID_ARGUMENT;
	}
	for (i = CH_GAME_N_ROWS - 1; i >= 0; i--) {
		printf("%d| ", i + 1);
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			if (j != CH_GAME_N_COLUMNS - 1) {
				printf("%c ", src->gameBoard[i][j]);
			} else {
				printf("%c |\n", src->gameBoard[i][j]);
			}
		}
	}
	printf("  -----------------\n");
	printf("   A B C D E F G H\n");
	return CH_GAME_SUCCESS;
}

bool isValidMove(CHMovesList *list, int toRow, int toCol) {
	CHMovesList *node = list;
	while (node != NULL ) {
		if (node->row == toRow && node->col == toCol)
			return true;
		node = node->next;
	}
	return false;
}

int cmpfunc(const void * a, const void * b) {
	if (((CHNodeForSort*) a)->row - ((CHNodeForSort*) b)->row == 0) {
		return ((CHNodeForSort*) a)->col - ((CHNodeForSort*) b)->col;
	}
	return ((CHNodeForSort*) a)->row - ((CHNodeForSort*) b)->row;
}

void printMoves(CHGame* src, CHMovesList *list, char c, int fRow, int fCol,
		Widget *widget, bool isGuiMode) {
	CHMovesList *node = list;
	int numOfMoves = 0;
	int i = 0;
	int isRed = 0;
	while (node != NULL ) {
		node = node->next;
		numOfMoves++;
	}
	node = list;
	CHNodeForSort *arr = (CHNodeForSort *) malloc(
			sizeof(CHNodeForSort) * numOfMoves);
	while (node != NULL ) {
		arr[i].row = node->row;
		arr[i].col = node->col;
		node = node->next;
		i++;
	}
	qsort(arr, numOfMoves, sizeof(CHNodeForSort), cmpfunc);
	for (i = 0; i < numOfMoves; i++) {
		isRed = 0;
		printf("<%d,%c>", arr[i].row + 1, arr[i].col + 65);
		if (isGuiMode) {
			setGlowCell(widget, arr[i].row, arr[i].col, CELL_GLOW_COLOR_BLUE);
		}
		if (/*src->gameMode == 1 && */src->difficulty < 3) {
			if (!isMyPieceSafe(src->gameBoard, c, fRow, fCol, arr[i].row,
					arr[i].col, src->currentTurn, arr[i].row, arr[i].col,
					REGULAR_PIECE_MODE)) {
				printf("*");
				if (isGuiMode) {
					setGlowCell(widget, arr[i].row, arr[i].col,
							CELL_GLOW_COLOR_RED);
					isRed = 1;
				}
			}
			if (!isThePieceMyColor(src->gameBoard[arr[i].row][arr[i].col],
					src->currentTurn) && src->gameBoard[arr[i].row][arr[i].col] != CH_GAME_EMPTY_ENTRY) {printf("^");
			if(isGuiMode && !isRed) {
				setGlowCell(widget,arr[i].row,arr[i].col,CELL_GLOW_COLOR_GREEN);
			}
		}

	}
		printf("\n");
	}
	free(arr);
}

CH_GAME_MESSAGE chGameGetMoves(CHGame* src, int fRow, int fCol, Widget *widget,bool isGuiMode) {
	bool isCorrectCol = false;
	char c;
	CHMovesList *list;
	if (src == NULL )
		return CH_GAME_INVALID_ARGUMENT;
	c = src->gameBoard[fRow][fCol];
	if (src->currentTurn == 0)
		isCorrectCol = isABlackPiece(c);
	else
		isCorrectCol = isAWhitePiece(c);
	if (!isCorrectCol)
		return CH_GAME_INVALID_COLOR;
	list = createMoveList(src->gameBoard, c, fRow, fCol, src->currentTurn);
	if (!list) {
		destroyMoveList(list);
		return CH_GAME_MEMORY_PROBLEM;
	}
	if (list->isValid != true) {
		destroyMoveList(list);
		return CH_GAME_NO_MOVES;
	}
	printMoves(src, list, c, fRow, fCol, widget, isGuiMode);
	destroyMoveList(list);
	return CH_GAME_SUCCESS;
}

void chPawnPromotion(CHGame* src, int row, int col, bool is_alphaBeta_func, bool isGuiMode) {
	int best_score = INT32_MIN, cur_score;
	char best_piece = ' ';
	if (((src->gameMode == 1) && (src->currentTurn != src->userColor))
			|| (is_alphaBeta_func)) {
		if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
			src->gameBoard[row][col] = CH_WHITE_QUEEN;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_score = cur_score;
				best_piece = CH_WHITE_QUEEN;
			}
			src->gameBoard[row][col] = CH_WHITE_ROOK;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_score = cur_score;
				best_piece = CH_WHITE_ROOK;
			}
			src->gameBoard[row][col] = CH_WHITE_BISHOP;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_score = cur_score;
				best_piece = CH_WHITE_BISHOP;
			}
			src->gameBoard[row][col] = CH_WHITE_KNIGHT;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_score = cur_score;
				best_piece = CH_WHITE_KNIGHT;
			}
			src->gameBoard[row][col] = CH_WHITE_PAWN;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_piece = CH_WHITE_PAWN;
			}
			src->gameBoard[row][col] = best_piece;
		} else {
			src->gameBoard[row][col] = CH_BLACK_QUEEN;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_score = cur_score;
				best_piece = CH_BLACK_QUEEN;
			}
			src->gameBoard[row][col] = CH_BLACK_ROOK;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_score = cur_score;
				best_piece = CH_BLACK_ROOK;
			}
			src->gameBoard[row][col] = CH_BLACK_BISHOP;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_score = cur_score;
				best_piece = CH_BLACK_BISHOP;
			}
			src->gameBoard[row][col] = CH_BLACK_KNIGHT;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_score = cur_score;
				best_piece = CH_BLACK_KNIGHT;
			}
			src->gameBoard[row][col] = CH_BLACK_PAWN;
			cur_score = get_board_score(src->currentTurn, src);
			if (cur_score > best_score) {
				best_piece = CH_BLACK_PAWN;
			}
			src->gameBoard[row][col] = best_piece;
		}
	} else {
		char str[MAX_LINE_LENGTH];
		bool isNotValid = true;
		char delimiter[8] = " \t\r\n";
		char strCopy[MAX_LINE_LENGTH];
		char *cur;
		if(isGuiMode){
			showPawnPromotionMessage(src, row, col);
			return;
		}
		while (isNotValid) {
			printf(
					"Pawn promotion- please replace the pawn by queen, rook, knight, bishop or pawn:\n");
			fgets(str, MAX_LINE_LENGTH, stdin);
			strcpy(strCopy, str);
			cur = strtok(strCopy, delimiter);
			if (strcmp(cur, "pawn") == 0) {
				if (strtok(NULL, delimiter) == NULL ) {
					isNotValid = false;
				} else
					printf("Invalid Type\n");
			} else if (strcmp(cur, "queen") == 0) {
				if (strtok(NULL, delimiter) == NULL ) {
					if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL)
						src->gameBoard[row][col] = CH_WHITE_QUEEN;
					else
						src->gameBoard[row][col] = CH_BLACK_QUEEN;
					isNotValid = false;
				} else
					printf("Invalid Type\n");
			} else if (strcmp(cur, "rook") == 0) {
				if (strtok(NULL, delimiter) == NULL ) {
					if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL)
						src->gameBoard[row][col] = CH_WHITE_ROOK;
					else
						src->gameBoard[row][col] = CH_BLACK_ROOK;
					isNotValid = false;
				} else
					printf("Invalid Type\n");

			} else if (strcmp(cur, "knight") == 0) {
				if (strtok(NULL, delimiter) == NULL ) {
					if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL)
						src->gameBoard[row][col] = CH_WHITE_KNIGHT;
					else
						src->gameBoard[row][col] = CH_BLACK_KNIGHT;
					isNotValid = false;
				} else
					printf("Invalid Type\n");
			} else if (strcmp(cur, "bishop") == 0) {
				if (strtok(NULL, delimiter) == NULL ) {
					if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL)
						src->gameBoard[row][col] = CH_WHITE_BISHOP;
					else
						src->gameBoard[row][col] = CH_BLACK_BISHOP;
					isNotValid = false;
				} else
					printf("Invalid Type\n");
			} else {
				printf("Invalid Type\n");
			}
		}
	}
}

CH_GAME_MESSAGE chGameSetMove(CHGame* src, char peice, int fRow, int fCol,
		int toRow, int toCol, bool is_alphaBeta_func, bool isGuiMode) {
	if (src == NULL )
		return CH_GAME_INVALID_ARGUMENT;
	bool isCorrectCol = false;
	char c;
	CHMovesList *list;
	CHMoveNode *node = (CHMoveNode *) malloc(sizeof(CHMoveNode));
	node->to_col = toCol;
	node->to_row = toRow;
	node->from_col = fCol;
	node->from_row = fRow;
	node->current_piece = src->gameBoard[fRow][fCol];
	node->piece_eaten = src->gameBoard[toRow][toCol];
	c = src->gameBoard[fRow][fCol];
	if (src->currentTurn == 0)
		isCorrectCol = isABlackPiece(c);
	else
		isCorrectCol = isAWhitePiece(c);
	if (!isCorrectCol) {
		free(node);
		return CH_GAME_INVALID_COLOR;
	}
	list = createMoveList(src->gameBoard, c, fRow, fCol, src->currentTurn);
	if (isValidMove(list, toRow, toCol)) {
		src->gameBoard[toRow][toCol] = peice;
		src->gameBoard[fRow][fCol] = CH_GAME_EMPTY_ENTRY;
	} else {
		free(node);
		destroyMoveList(list);
		return CH_GAME_INVALID_MOVE;
	}
	destroyMoveList(list);
	if (((src->gameBoard[toRow][toCol] == CH_BLACK_PAWN) && (toRow == 0))
			|| ((src->gameBoard[toRow][toCol] == CH_WHITE_PAWN)
					&& (toRow == CH_GAME_N_ROWS - 1))) {
		chPawnPromotion(src, toRow, toCol, is_alphaBeta_func,isGuiMode);
		if(isGuiMode){

		}
	}
	spArrayListAddFirst(src->list, *node);
	free(node);
	return CH_GAME_SUCCESS;
}

CH_GAME_MESSAGE isCheck(CHGame* src, int is_computer, bool isGuiMode) {
	int kRow = 0, kCol = 0;
	if (src == NULL )
		return CH_GAME_INVALID_ARGUMENT;
	findKing(src->gameBoard, src->currentTurn, &kRow, &kCol);
	if (!isMyPieceSafe(src->gameBoard, CH_GAME_EMPTY_ENTRY, 0, 0, 0, 0,
			src->currentTurn, kRow, kCol, KING_MODE)) {
		if (is_computer) {
			printf("Check!\n");
			if (isGuiMode) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
						"Check!", NULL );
				SDL_FlushEvents(SDL_QUIT, SDL_USEREVENT);
			}
		} else if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
			printf("Check: white King is threatened!\n");
			if (isGuiMode) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
						"Check: white King is threatened!", NULL );
				SDL_FlushEvents(SDL_QUIT, SDL_USEREVENT);

			}
		} else {
			printf("Check: black King is threatened!\n");
			if (isGuiMode) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
						"Check: black King is threatened!", NULL );
				SDL_FlushEvents(SDL_QUIT, SDL_USEREVENT);
			}
		}
	}
	return CH_GAME_SUCCESS;
}

CH_GAME_MESSAGE chIsCheckmateOrTie(CHGame* src) {
	int i, j, kRow, kCol;
	char curC;
	CHMovesList *list;
	if (src == NULL )
		return CH_GAME_INVALID_ARGUMENT;
	findKing(src->gameBoard, src->currentTurn, &kRow, &kCol);
	for (i = 0; i < CH_GAME_N_ROWS; i++) {
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			curC = src->gameBoard[i][j];
			if (isThePieceMyColor(curC, src->currentTurn)) {
				list = createMoveList(src->gameBoard, curC, i, j,
						src->currentTurn);
				if (list->isValid) {
					destroyMoveList(list);
					return CH_GAME_NO_WIN_OR_TIE;
				}
				destroyMoveList(list);
			}
		}
	}

	if (isMyPieceSafe(src->gameBoard, CH_GAME_EMPTY_ENTRY, 0, 0, 0, 0,
			src->currentTurn, kRow, kCol, KING_MODE)) {
		return CH_GAME_TIE;
	} else {
		if (src->currentTurn == CH_GAME_BLACK_PLAYER_SYMBOL) {
			return CH_GAME_WHITE_WINS;
		} else {
			return CH_GAME_BLACK_WINS;
		}
	}
}

CHGame* chGameCopy(CHGame* src) {
	if (src == NULL ) {
		return NULL ;
	}

	CHGame* new_src = (CHGame*) malloc(sizeof(CHGame)); /* allocate place in memory */
	if (!new_src) {
		chGameDestroy(src);
		printf("Error: malloc has failed\n");
		return NULL ;
	}
	new_src->currentTurn = src->currentTurn;
	for (int i = 0; i < CH_GAME_N_ROWS; i++) {
		for (int j = 0; j < CH_GAME_N_COLUMNS; j++) {
			new_src->gameBoard[i][j] = src->gameBoard[i][j]; /* copy the board */
		}
	}
	new_src->gameMode = src->gameMode;
	new_src->userColor = src->userColor;
	new_src->difficulty = src->difficulty;
	new_src->list = spArrayListCopy(src->list); /* copy the history list */
	return new_src;

}

CH_GAME_MESSAGE chGameUndoPrevMove(CHGame* src) {
	if (src == NULL ) {
		return CH_GAME_INVALID_ARGUMENT;
	}

	if (spArrayListIsEmpty(src->list) || src->list == NULL
			|| src->list->actualSize < 1) {
		return CH_GAME_NO_HISTORY;
	}

	src->gameBoard[src->list->elements->to_row][src->list->elements->to_col] =
			src->list->elements->piece_eaten;
	src->gameBoard[src->list->elements->from_row][src->list->elements->from_col] =
			src->list->elements->current_piece;

	if (src->currentTurn == CH_GAME_BLACK_PLAYER_SYMBOL) { /* set the currentPlayer turn */
		src->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
	} else {
		src->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
	}

	spArrayListRemoveFirst(src->list); /* remove the first argument */

	return CH_GAME_SUCCESS;
}

