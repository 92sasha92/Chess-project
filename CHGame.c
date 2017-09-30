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
	const SDL_MessageBoxColorScheme colorScheme = {{{ 80, 0, 0 }, { 0, 255, 0 }, { 255, 255, 0 }, { 0, 0, 255 }, { 255, 0, 255 }}}; // SDL_MESSAGEBOX_COLORS
	const SDL_MessageBoxData messageboxdata = { SDL_MESSAGEBOX_INFORMATION, /* .flags */
	NULL, /* .window */
	"Pawn promotion", /* .title */
	"Please replace the pawn by queen, rook, knight, bishop or pawn:", /* .message */
	SDL_arraysize(buttons), /* .numbuttons */
	buttons, /* .buttons */
	&colorScheme /* .colorScheme */
	};
	if (SDL_ShowMessageBox(&messageboxdata, &buttonId) < 0) {
		SDL_Log("ERROR: displaying message box");
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
	} else {
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


CH_GAME_MESSAGE chGameCreateMode1(CHGame* src, int difficulty, int userColor) {
	if (src->gameMode == 1) {
		if (userColor != CH_GAME_BLACK_PLAYER_SYMBOL
				&& userColor != CH_GAME_WHITE_PLAYER_SYMBOL)
			return CH_GAME_INVALID_ARGUMENT;
		src->userColor = userColor;
		if (difficulty < 1 || 4 < difficulty)
			return CH_GAME_INVALID_ARGUMENT;
		src->difficulty = difficulty;
		src->list = spArrayListCreate(6); /* every undo move include computer and user moves */
		if (!(src->list)) {
			free(src);
			printf("ERROR: malloc has failed\n");
			return CH_GAME_MEMORY_PROBLEM;
		}
	} else {
		src->list = NULL;
		src->difficulty = 0;
	}
	return CH_GAME_SUCCESS;
}


CHGame* chGameCreate(int gameMode, int userColor, int difficulty,
		int currentTurn) {
	CHGame* src = (CHGame*) malloc(sizeof(CHGame)); /* allocate place in memory */
	if (!src) {
		printf("ERROR: malloc has failed\n");
		return NULL ;
	}
	defaultBoard(src);
	if (gameMode != 1 && gameMode != 2) {
		free(src);
		return NULL ;
	}
	src->gameMode = gameMode; // set game mode
	if (currentTurn != CH_GAME_BLACK_PLAYER_SYMBOL
			&& currentTurn != CH_GAME_WHITE_PLAYER_SYMBOL) {
		free(src);
		return NULL ;
	}
	src->currentTurn = currentTurn; // set current turn
	if (chGameCreateMode1(src, difficulty, userColor) != CH_GAME_SUCCESS) {
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


CH_GAME_MESSAGE printMoves(CHGame* src, CHMovesList *list, char c, int fRow, int fCol, Widget *gameBoardWidget, bool isGuiMode) {
	CHMovesList *node = list;
	int numOfMoves = 0;
	int i = 0;
	int isRed = 0;
	while (node != NULL ) {
		node = node->next;
		numOfMoves++;
	}
	node = list;
	CHNodeForSort *arr = (CHNodeForSort *) malloc(sizeof(CHNodeForSort) * numOfMoves);
	if (arr == NULL) {
		printf ("ERROR: problem occurred when malloc\n");
		return CH_GAME_MEMORY_PROBLEM;
	}
	while (node != NULL ) {
		arr[i].row = node->row;
		arr[i].col = node->col;
		node = node->next;
		i++;
	}
	qsort(arr, numOfMoves, sizeof(CHNodeForSort), cmpfunc); // sort the list
	for (i = 0; i < numOfMoves; i++) {
		isRed = 0;
		printf("<%d,%c>", arr[i].row + 1, arr[i].col + 65);
		if (isGuiMode) {
			setGlowCell(gameBoardWidget, arr[i].row, arr[i].col, CELL_GLOW_COLOR_BLUE);
		}
		if (src->difficulty < 3) { // condition to sign the moves by colors or signs if it is not gui mode
			if (!isMyPieceSafe(src->gameBoard, c, fRow, fCol, arr[i].row, arr[i].col, src->currentTurn, arr[i].row, arr[i].col, REGULAR_PIECE_MODE)) {
				printf("*");
				if (isGuiMode) { setGlowCell(gameBoardWidget, arr[i].row, arr[i].col, CELL_GLOW_COLOR_RED);
					isRed = 1;
				}
			}
			if (!isThePieceMyColor(src->gameBoard[arr[i].row][arr[i].col], src->currentTurn) && src->gameBoard[arr[i].row][arr[i].col] != CH_GAME_EMPTY_ENTRY) {
				printf("^");
				if (isGuiMode && !isRed) {
					setGlowCell(gameBoardWidget,arr[i].row,arr[i].col,CELL_GLOW_COLOR_GREEN);
				}
			}
		}
		printf("\n");
	}
	free(arr);
	return CH_GAME_SUCCESS;
}


CH_GAME_MESSAGE chGameGetMoves(CHGame* src, int fRow, int fCol, Widget *widget,bool isGuiMode) {
	bool isCorrectCol = false;
	char c;
	CHMovesList *list;
	if (src == NULL )
		return CH_GAME_INVALID_ARGUMENT;
	c = src->gameBoard[fRow][fCol]; // the current piece
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
	if (list->isValid != true) { // checks if the move is valid
		destroyMoveList(list);
		return CH_GAME_NO_MOVES;
	}
	if (printMoves(src, list, c, fRow, fCol, widget, isGuiMode) == CH_GAME_MEMORY_PROBLEM){
		destroyMoveList(list);
		return CH_GAME_MEMORY_PROBLEM;
	}
	destroyMoveList(list);
	return CH_GAME_SUCCESS;
}


CH_GAME_MESSAGE chPawnPromotion(CHGame* src, int row, int col,  bool isGuiMode) {
	if (src == NULL)
		return CH_GAME_INVALID_ARGUMENT;
    char str[MAX_LINE_LENGTH], delimiter[8] = " \t\r\n", strCopy[MAX_LINE_LENGTH], *cur;
    bool isNotValid = true;
    if(isGuiMode){
        showPawnPromotionMessage(src, row, col);
        return CH_GAME_SUCCESS;
    }
    while (isNotValid) {
        printf("Pawn promotion- please replace the pawn by queen, rook, knight, bishop or pawn:\n");
        fgets(str, MAX_LINE_LENGTH, stdin); // read to which piece to promote to
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
	return CH_GAME_SUCCESS;
}


CH_GAME_MESSAGE chGameSetMove(CHGame* src, char piece, int fRow, int fCol, int toRow, int toCol, bool is_alphaBeta_func, bool isGuiMode) {
	if (src == NULL )
		return CH_GAME_INVALID_ARGUMENT;
	bool isCorrectCol = false;
	char c;
	CHMovesList *list;
	CHMoveNode *node = (CHMoveNode *) malloc(sizeof(CHMoveNode)); // create node for the move
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
	if(!list){
		free(node);
		return CH_GAME_MEMORY_PROBLEM;
	}
	if (isValidMove(list, toRow, toCol)) {
		src->gameBoard[toRow][toCol] = piece;
		src->gameBoard[fRow][fCol] = CH_GAME_EMPTY_ENTRY;
	} else {
		free(node);
		destroyMoveList(list);
		return CH_GAME_INVALID_MOVE;
	}
	destroyMoveList(list);
	if ((((src->gameBoard[toRow][toCol] == CH_BLACK_PAWN) && (toRow == 0)) // checks if pawn promotion is valid
			|| ((src->gameBoard[toRow][toCol] == CH_WHITE_PAWN)
					&& (toRow == CH_GAME_N_ROWS - 1))) && !is_alphaBeta_func) {
		if (chPawnPromotion(src, toRow, toCol,isGuiMode) == CH_GAME_INVALID_ARGUMENT) {
			free(node);
			return CH_GAME_INVALID_ARGUMENT;
		}
	}
	spArrayListAddFirst(src->list, *node); // add the move to the moves list
	free(node);
	return CH_GAME_SUCCESS;
}


CH_GAME_MESSAGE isCheck(CHGame* src, int is_computer, bool isGuiMode) {
	int kRow = 0, kCol = 0;
	if (src == NULL )
		return CH_GAME_INVALID_ARGUMENT;
	findKing(src->gameBoard, src->currentTurn, &kRow, &kCol);
	if (!isMyPieceSafe(src->gameBoard, CH_GAME_EMPTY_ENTRY, 0, 0, 0, 0,
			src->currentTurn, kRow, kCol, KING_MODE)) { // checks if the king is safe
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
	findKing(src->gameBoard, src->currentTurn, &kRow, &kCol); // find the king location
	for (i = 0; i < CH_GAME_N_ROWS; i++) {
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			curC = src->gameBoard[i][j];
			if (isThePieceMyColor(curC, src->currentTurn)) {
				list = createMoveList(src->gameBoard, curC, i, j,
						src->currentTurn); // checks if the king is safe
				if(!list){
					return CH_GAME_MEMORY_PROBLEM;
				}
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
		printf("ERROR: malloc has failed\n");
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
	src->gameBoard[src->list->elements->to_row][src->list->elements->to_col] = src->list->elements->piece_eaten; // set back the pieces moved
	src->gameBoard[src->list->elements->from_row][src->list->elements->from_col] = src->list->elements->current_piece;
	if (src->currentTurn == CH_GAME_BLACK_PLAYER_SYMBOL) { /* set the currentPlayer turn */
		src->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
	} else {
		src->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
	}
	spArrayListRemoveFirst(src->list); /* remove the first argument */
	return CH_GAME_SUCCESS;
}
