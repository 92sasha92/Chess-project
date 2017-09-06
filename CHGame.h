/*
 * CHGame.h
 *
 *  Created on: Aug 25, 2017
 *      Author: sasha
 */

#ifndef CHGAME_H_
#define CHGAME_H_

#include "SPArrayList.h"
#include "CHMoves.h"


typedef struct ch_fiar_game_t {
	char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS];
	int gameMode;
	int userColor;
	int difficulty;
	int currentTurn;
	SPArrayList *list;
} CHGame;

typedef enum ch_fiar_game_message_t {
	CH_GAME_INVALID_MOVE,
	CH_GAME_INVALID_ARGUMENT,
	CH_GAME_INVALID_COLOR,
	CH_GAME_NO_MOVES,
	CH_GAME_MEMORY_PROBLEM,
	CH_GAME_SUCCESS,
	CH_GAME_FILE_PROBLEM
} CH_GAME_MESSAGE;



CH_GAME_MESSAGE chGamePrintBoard(CHGame* src);

int chGameCreateMode1(CHGame* src,int difficulty,int userColor);
CHGame* chGameCreate(int gameMode,int userColor,int difficulty,int currentTurn);
void chGameDestroy(CHGame* src);
CH_GAME_MESSAGE chGameShowMoves(CHGame* src, int fRow,int fCol);
/**
 * Sets the next move in a given game by specifying the position of the piece and where to move it.
 *
 * @param src - The target game
 * @param fRow - The row of the piece
 * @param fCol - The column of the piece
 * @param toRow - The row to move the piece to
 * @param toCol - The column to move the piece to
 * @return
 * CH_GAME_INVALID_ARGUMENT - if src is NULL
 * CH_GAME_INVALID_COLOR - If position <x,y> does not contain a piece of the user's color.
 * CH_GAME_INVALID_MOVE - If the move is illegal for the piece in the position <x,y>.
 * CH_GAME_SUCCESS - otherwise
 */
CH_GAME_MESSAGE chGameSetMove(CHGame* src, int fRow,int fCol,int toRow,int toCol);


/**
 * Checks if there is a checkmate/tie.
 *
 * @param src - The target game
 * @return
 * -2 - if src is NULL
 * -1 - if there isn't a tie/checkmate
 *  0 - If there is a checkmate and black won.
 *  1 - If there is a checkmate and white won.
 *  2 - if there is a tie
 */
int chIsCheckmateOrTie(CHGame* src);
void isCheck(CHGame* src);

CH_GAME_MESSAGE chGameSave(CHGame* src,char *path);


#endif /* CHGAME_H_ */
