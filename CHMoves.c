#include "CHMoves.h"

bool isABlackPiece(char c) {
	if (c == CH_BLACK_ROOK || c == CH_BLACK_KNIGHT || c == CH_BLACK_BISHOP || c == CH_BLACK_KING || c == CH_BLACK_QUEEN || c == CH_BLACK_PAWN)
		return true;
	return false;
}

bool isAWhitePiece(char c) {
	if (c == CH_WHITE_ROOK || c == CH_WHITE_KNIGHT || c == CH_WHITE_BISHOP || c == CH_WHITE_KING || c == CH_WHITE_QUEEN || c == CH_WHITE_PAWN)
		return true;
	return false;
}

bool isThePieceMyColor(char c, int color) {
	if (color == CH_GAME_WHITE_PLAYER_SYMBOL) {
		return isAWhitePiece(c);
	} else {
		return isABlackPiece(c);
	}
}

void findKing(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],int color,int *kRow,int *kCol){
	int i,j;
	bool isKingFound = false;
	for(i = 0; i < CH_GAME_N_ROWS;i++){
		for(j = 0;j < CH_GAME_N_COLUMNS;j++){
			if((color == CH_GAME_WHITE_PLAYER_SYMBOL && gameBoard[i][j] == CH_WHITE_KING) || (color == CH_GAME_BLACK_PLAYER_SYMBOL && gameBoard[i][j] == CH_BLACK_KING)){
				*kRow = i;
				*kCol = j;
				isKingFound = true;
				break;
			}
		}
		if(isKingFound)
			break;
	}
}





CHMovesList * insertFirst(CHMovesList *list, int row, int col) {
	CHMovesList *node;
	if(!list->isValid){
		node = list;
	}
	else{
		node = (CHMovesList *) malloc(sizeof(CHMovesList));
		if (!node) {
			printf("Error: malloc has failed\n");
			return NULL ;
		}
		node->next = list;
	}
	node->isValid = true;
	node->row = row;
	node->col = col;
	return node;
}



bool isKnightThreat(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],int color, int mRow, int mCol,int mode){
	int i,kRow,kCol;
	char curC;
	int curRow = mRow;
	int curCol = mCol;
	for (i = -2; i < 3; i++) {
		curRow = mRow + i;
		if (curRow < CH_GAME_N_ROWS && curRow >= 0 && i != 0) {
			for (int j = 0; j < 2; j++) {
				if (i == -2 || i == 2)
					if (j == 0)
						curCol = mCol - 1;
					else
						curCol = mCol + 1;
				else if (j == 0)
					curCol = mCol - 2;
				else
					curCol = mCol + 2;
				if (curCol < CH_GAME_N_COLUMNS && curCol >= 0) {
					curC = gameBoard[curRow][curCol];
					if ((color == CH_GAME_BLACK_PLAYER_SYMBOL && curC == CH_WHITE_KNIGHT) || (color == CH_GAME_WHITE_PLAYER_SYMBOL && curC == CH_BLACK_KNIGHT)){
						if(mode){
							findKing(gameBoard,!color,&kRow,&kCol);
							if(isMyPieceSafe(gameBoard, curC, curRow, curCol, mRow, mCol, !color, kRow, kCol,KING_MODE)){
								return false;
							}
						}else{
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}


bool isRookOrQueenThreat(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],int color, int mRow, int mCol,int mode){
	int curRow = mRow + 1;
	int curCol = mCol;
	int i,kCol,kRow;
	char curC ;
	for (i = 0; i < 4; i++) {
		if (i == 1) {
			curRow = mRow - 1;
			curCol = mCol;
		} else if (i == 2) {
			curRow = mRow;
			curCol = mCol + 1;
		} else if (i == 3) {
			curRow = mRow;
			curCol = mCol - 1;
		}
		while (curRow >= 0 && curRow < CH_GAME_N_ROWS && curCol >= 0 && curCol < CH_GAME_N_COLUMNS) {
			curC = gameBoard[curRow][curCol];
			if ((color == CH_GAME_BLACK_PLAYER_SYMBOL && (curC == CH_WHITE_QUEEN || curC == CH_WHITE_ROOK)) || (color == CH_GAME_WHITE_PLAYER_SYMBOL && (curC == CH_BLACK_QUEEN || curC == CH_BLACK_ROOK))){
				if(mode){
					findKing(gameBoard,!color,&kRow,&kCol);
					if(isMyPieceSafe(gameBoard, curC, curRow, curCol, mRow, mCol, !color, kRow, kCol,KING_MODE)){
						return false;
					}
				}else{
					return false;
				}
			}
			if (curC != CH_GAME_EMPTY_ENTRY)
				break;
			if (i == 0)
				curRow++;
			else if (i == 1)
				curRow--;
			else if (i == 2)
				curCol++;
			else
				curCol--;
		}
	}
	return true;
}

bool isBishopOrQueenThreat(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],int color, int mRow, int mCol,int mode){
	int i,kCol,kRow;
	int curRow = mRow + 1;
	int curCol = mCol + 1;
	char curC;
	for(i = 0;i < 4;i++){
		if(i == 1){
			curRow = mRow - 1;
			curCol = mCol - 1;
		}
		else if(i == 2){
			curRow = mRow + 1;
			curCol = mCol - 1;
		}
		else if(i == 3){
			curRow = mRow - 1;
			curCol = mCol + 1;
		}
		while(curRow >= 0 && curRow < CH_GAME_N_ROWS && curCol >= 0 && curCol < CH_GAME_N_COLUMNS){
			curC = gameBoard[curRow][curCol];
			if ((color == CH_GAME_BLACK_PLAYER_SYMBOL && (curC == CH_WHITE_BISHOP || curC == CH_WHITE_QUEEN)) || (color == CH_GAME_WHITE_PLAYER_SYMBOL && (curC == CH_BLACK_BISHOP || curC == CH_BLACK_QUEEN))){
				if(mode){
					findKing(gameBoard,!color,&kRow,&kCol);
					if(isMyPieceSafe(gameBoard, curC, curRow, curCol, mRow, mCol, !color, kRow, kCol,KING_MODE)){
						return false;
					}
				}else{
					return false;
				}
			}
			if (curC != CH_GAME_EMPTY_ENTRY)
				break;
			if(i == 0){
				curRow++;
				curCol++;
			}
			else if(i == 1){
				curRow--;
				curCol--;
			}
			else if(i == 2){
				curRow++;
				curCol--;
			}
			else{
				curRow--;
				curCol++;
			}
		}
	}
	return true;
}

bool isPawnThreat(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],int color, int mRow, int mCol,int mode){
	int curRow,curCol,kRow,kCol;
	char curC;
	if (color == CH_GAME_BLACK_PLAYER_SYMBOL)
		curRow = mRow - 1;
	else
		curRow = mRow + 1;
	curCol = mCol - 1;
	if(curRow >= 0 && curRow < CH_GAME_N_ROWS && curCol >= 0 && curCol < CH_GAME_N_COLUMNS){
		curC = gameBoard[curRow][curCol];
		if((color == CH_GAME_BLACK_PLAYER_SYMBOL && curC == CH_WHITE_PAWN) || (color == CH_GAME_WHITE_PLAYER_SYMBOL && curC == CH_BLACK_PAWN)){
			if(mode){
				findKing(gameBoard,!color,&kRow,&kCol);
				if(isMyPieceSafe(gameBoard, curC, curRow, curCol, mRow, mCol, !color, kRow, kCol,KING_MODE)){
					return false;
				}
			}else{
				return false;
			}
		}
	}
	curCol = mCol + 1;
	if(curRow >= 0 && curRow < CH_GAME_N_ROWS && curCol >= 0 && curCol < CH_GAME_N_COLUMNS){
		curC = gameBoard[curRow][curCol];
		if((color == CH_GAME_BLACK_PLAYER_SYMBOL && curC == CH_WHITE_PAWN) || (color == CH_GAME_WHITE_PLAYER_SYMBOL && curC == CH_BLACK_PAWN)){
			if(mode){
				findKing(gameBoard,!color,&kRow,&kCol);
				if(isMyPieceSafe(gameBoard, curC, curRow, curCol, mRow, mCol, !color, kRow, kCol,KING_MODE)){
					return false;
				}
			}else{
				return false;
			}
		}
	}
	return true;
}

bool isKingThreat(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],int color, int mRow, int mCol,int mode){
	int i;
	int curRow = mRow - 1;
	int curCol = mCol - 1;
	char curC;
	for(i = 0; i < 8;i++){
		if(i == 1){
			curRow = mRow - 1;
			curCol = mCol;
		}else if(i == 2){
			curRow = mRow - 1;
			curCol = mCol + 1;
		}else if(i == 3){
			curRow = mRow;
			curCol = mCol - 1;
		}else if(i == 4){
			curRow = mRow;
			curCol = mCol + 1;
		}else if(i == 5){
			curRow = mRow + 1;
			curCol = mCol - 1;
		}else if(i == 6){
			curRow = mRow + 1;
			curCol = mCol;
		}else if(i == 7){
			curRow = mRow + 1;
			curCol = mCol + 1;
		}
		if(curRow >= 0 && curRow < CH_GAME_N_ROWS && curCol >= 0 && curCol < CH_GAME_N_COLUMNS){
			curC = gameBoard[curRow][curCol];
			if((color == CH_GAME_BLACK_PLAYER_SYMBOL && curC == CH_WHITE_KING) || (color == CH_GAME_WHITE_PLAYER_SYMBOL && curC == CH_BLACK_KING)){
				if(mode){
					if(isMyPieceSafe(gameBoard, curC, curRow, curCol, mRow, mCol, !color, mRow, mCol,KING_MODE)){
						return false;
					}
				}else{
					return false;
				}
			}

		}
	}
	return true;
}

//bool isTheKingSafe(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS], char c, int fRow, int fCol, int toRow, int toCol, int color, int kRow, int kCol) {
//	int i,j;
//	char gameBoardCopy[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS];
//	for(i = 0;i < CH_GAME_N_ROWS;i++){
//		for(j = 0;j < CH_GAME_N_COLUMNS;j++){
//			gameBoardCopy[i][j] = gameBoard[i][j];
//		}
//	}
//	if(c != CH_GAME_EMPTY_ENTRY){
//		gameBoardCopy[fRow][fCol] = CH_GAME_EMPTY_ENTRY;
//		gameBoardCopy[toRow][toCol] = c;
//	}
//	int *reRow,*reCol;
//	if(!isPawnThreat(gameBoardCopy,color,kRow,kCol,0))
//		return false;
//	if(!isKnightThreat(gameBoardCopy,color,kRow,kCol,0))
//		return false;
//	if(!isRookOrQueenThreat(gameBoardCopy,color,kRow,kCol,0))
//		return false;
//	if(!isBishopOrQueenThreat(gameBoardCopy,color,kRow,kCol,0))
//		return false;
//	if(!isKingThreat(gameBoardCopy,color,kRow,kCol,0))
//		return false;
//	return true;
//}

bool isMyPieceSafe(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS], char c, int fRow, int fCol, int toRow, int toCol, int color, int mRow, int mCol,int mode) {
	int i,j;
	char gameBoardCopy[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS];
	for(i = 0;i < CH_GAME_N_ROWS;i++){
		for(j = 0;j < CH_GAME_N_COLUMNS;j++){
			gameBoardCopy[i][j] = gameBoard[i][j];
		}
	}
	if(c != CH_GAME_EMPTY_ENTRY){
		gameBoardCopy[fRow][fCol] = CH_GAME_EMPTY_ENTRY;
		gameBoardCopy[toRow][toCol] = c;
	}
	if(!isPawnThreat(gameBoardCopy,color,mRow,mCol,mode))
		return false;
	if(!isKnightThreat(gameBoardCopy,color,mRow,mCol,mode))
		return false;
	if(!isRookOrQueenThreat(gameBoardCopy,color,mRow,mCol,mode))
		return false;
	if(!isBishopOrQueenThreat(gameBoardCopy,color,mRow,mCol,mode))
		return false;
	if(!isKingThreat(gameBoardCopy,color,mRow,mCol,mode))
		return false;
	return true;
}





CHMovesList *pawnMoves(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],
		char c, int row, int col, CHMovesList *list, int color,int kRow,int kCol) {
	char curC,curCMinus;
	int curRow = row;
	int curCol = col;
	int i,curRowMinus1;
	if (color == CH_GAME_BLACK_PLAYER_SYMBOL)
		curRow = row - 1;
	else
		curRow = row + 1;
	if (curRow >= 0 && curRow < CH_GAME_N_ROWS) {
		for (i = 0; i < 3; i++) {
			if (i == 0)
				curCol = col - 1;
			else if (i == 1)
				curCol = col + 1;
			else
				curCol = col;
			if (curCol >= 0 && curCol < CH_GAME_N_COLUMNS) {
				curC = gameBoard[curRow][curCol];
				if (!isThePieceMyColor(curC, color) && ((i != 2 && curC != CH_GAME_EMPTY_ENTRY) || (i == 2 && curC == CH_GAME_EMPTY_ENTRY)) && isMyPieceSafe(gameBoard,c,row,col,curRow,curCol,color, kRow, kCol,KING_MODE)) {
					list = insertFirst(list, curRow, curCol);
					if (list == NULL )
						return NULL ;
				}
			}
		}
	}
	curCol = col;

	if((color == CH_GAME_BLACK_PLAYER_SYMBOL && row == 6) || (color == CH_GAME_WHITE_PLAYER_SYMBOL && row == 1)){
		if(color == CH_GAME_BLACK_PLAYER_SYMBOL){
			curRow = row - 2;
			curRowMinus1 = row - 1;
		}
		else{
			curRow = row + 2;
			curRowMinus1 = row + 1;
		}
		curC = gameBoard[curRow][curCol];
		curCMinus = gameBoard[curRowMinus1][curCol];
		if ( curCMinus == CH_GAME_EMPTY_ENTRY && curC == CH_GAME_EMPTY_ENTRY && isMyPieceSafe(gameBoard,c,row,col,curRow,curCol,color, kRow, kCol,KING_MODE)) {
			list = insertFirst(list, curRow, curCol);
			if (list == NULL )
				return NULL ;
		}
	}
	return list;
}

CHMovesList *rookMoves(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],
		char c, int row, int col, CHMovesList *list, int color,int kRow,int kCol) {
	int i;
	int curRow = row + 1;
	int curCol = col;
	char curC;
	for (i = 0; i < 4; i++) {
		if (i == 1) {
			curRow = row - 1;
			curCol = col;
		} else if (i == 2) {
			curRow = row;
			curCol = col + 1;
		} else if (i == 3) {
			curRow = row;
			curCol = col - 1;
		}
		while (curRow >= 0 && curRow < CH_GAME_N_ROWS && curCol >= 0 && curCol < CH_GAME_N_COLUMNS) {
			curC = gameBoard[curRow][curCol];
			if (!isThePieceMyColor(curC, color) && isMyPieceSafe(gameBoard,c,row,col,curRow,curCol,color, kRow, kCol,KING_MODE)) {
				list = insertFirst(list, curRow, curCol);
				if (list == NULL )
					return NULL ;
				if(curC != CH_GAME_EMPTY_ENTRY)
					break;
			} else if(curC != CH_GAME_EMPTY_ENTRY)
				break;
			if (i == 0)
				curRow++;
			else if (i == 1)
				curRow--;
			else if (i == 2)
				curCol++;
			else
				curCol--;
		}
	}
	return list;
}

CHMovesList *knightMoves(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],
		char c, int row, int col, CHMovesList *list, int color,int kRow,int kCol) {
	int curRow;
	int curCol;
	char curC;
	for (int i = -2; i < 3; i++) {
		curRow = row + i;
		if (curRow < CH_GAME_N_ROWS && curRow >= 0 && i != 0) {
			for (int j = 0; j < 2; j++) {
				if (i == -2 || i == 2)
					if (j == 0)
						curCol = col - 1;
					else
						curCol = col + 1;
				else if (j == 0)
					curCol = col - 2;
				else
					curCol = col + 2;
				if (curCol < CH_GAME_N_COLUMNS && curCol >= 0) {
					curC = gameBoard[curRow][curCol];
					if (!isThePieceMyColor(curC, color) && isMyPieceSafe(gameBoard,c,row,col,curRow,curCol,color, kRow, kCol,KING_MODE)) {
						list = insertFirst(list, curRow, curCol);
						if (list == NULL )
							return NULL ;
					}
				}
			}
		}
	}

	return list;
}

CHMovesList *bishopMoves(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],
		char c, int row, int col, CHMovesList *list, int color,int kRow,int kCol) {
	int i;
	int curRow = row + 1;
	int curCol = col + 1;
	char curC;
	for (i = 0; i < 4; i++) {
		if (i == 1) {
			curRow = row - 1;
			curCol = col - 1;
		} else if (i == 2) {
			curRow = row + 1;
			curCol = col - 1;
		} else if (i == 3) {
			curRow = row - 1;
			curCol = col + 1;
		}
		while (curRow >= 0 && curRow < CH_GAME_N_ROWS && curCol >= 0 && curCol < CH_GAME_N_COLUMNS) {
			curC = gameBoard[curRow][curCol];
			if (!isThePieceMyColor(curC, color) && isMyPieceSafe(gameBoard,c,row,col,curRow,curCol,color, kRow, kCol,KING_MODE)) {
				list = insertFirst(list, curRow, curCol);
				if (list == NULL )
					return NULL ;
				if(curC != CH_GAME_EMPTY_ENTRY)
					break;
			} else if(curC != CH_GAME_EMPTY_ENTRY)
                break;
			if (i == 0) {
				curRow++;
				curCol++;
			} else if (i == 1) {
				curRow--;
				curCol--;
			} else if (i == 2) {
				curRow++;
				curCol--;
			} else {
				curRow--;
				curCol++;
			}
		}
	}
	return list;
}

CHMovesList *kingMoves(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS], char c, int row, int col, CHMovesList *list, int color) {
	int i;
	int curRow = row - 1;
	int curCol = col - 1;
	char curC;
	for(i = 0; i < 8;i++){
		if(i == 1){
			curRow = row - 1;
			curCol = col;
		}else if(i == 2){
			curRow = row - 1;
			curCol = col + 1;
		}else if(i == 3){
			curRow = row;
			curCol = col - 1;
		}else if(i == 4){
			curRow = row;
			curCol = col + 1;
		}else if(i == 5){
			curRow = row + 1;
			curCol = col - 1;
		}else if(i == 6){
			curRow = row + 1;
			curCol = col;
		}else if(i == 7){
			curRow = row + 1;
			curCol = col + 1;
		}
		if(curRow >= 0 && curRow < CH_GAME_N_ROWS && curCol >= 0 && curCol < CH_GAME_N_COLUMNS){
			curC = gameBoard[curRow][curCol];
			if (!isThePieceMyColor(curC, color) && isMyPieceSafe(gameBoard,c,row,col,curRow,curCol,color, curRow, curCol,KING_MODE)) {
				list = insertFirst(list, curRow, curCol);
				if (list == NULL )
					return NULL ;
			}
		}
	}
	return list;
}

CHMovesList *createMoveList(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],char c, int row, int col, int color) {
	int kRow,kCol;
	CHMovesList *list = (CHMovesList *) malloc(sizeof(CHMovesList));
	if (!list) {
		printf("Error: malloc has failed\n");
		return NULL ;
	}
	list->next = NULL;
	list->isValid = false;
	findKing(gameBoard,color,&kRow,&kCol);
	if (c == CH_WHITE_PAWN || c == CH_BLACK_PAWN) {
		return pawnMoves(gameBoard, c, row, col, list, color,kRow,kCol);
	} else if (c == CH_WHITE_ROOK || c == CH_BLACK_ROOK) {
		return rookMoves(gameBoard, c, row, col, list, color,kRow,kCol);
	} else if (c == CH_WHITE_KNIGHT || c == CH_BLACK_KNIGHT) {
		return knightMoves(gameBoard, c, row, col, list, color,kRow,kCol);
	} else if (c == CH_WHITE_BISHOP || c == CH_BLACK_BISHOP) {
		return bishopMoves(gameBoard, c, row, col, list, color,kRow,kCol);
	} else if (c == CH_WHITE_QUEEN || c == CH_BLACK_QUEEN) {
		list = bishopMoves(gameBoard, c, row, col, list, color,kRow,kCol);
		return rookMoves(gameBoard, c, row, col, list, color,kRow,kCol);
	} else if (c == CH_WHITE_KING || c == CH_BLACK_KING) {
		return kingMoves(gameBoard, c, row, col, list, color);
	}
	return list;
}

void destroyMoveList(CHMovesList *list) {
	CHMovesList *node = list;
	while (list != NULL ) {
		list = list->next;
		free(node);
		node = list;
	}
}

