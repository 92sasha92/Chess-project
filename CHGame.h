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

#include "Widget.h"

typedef struct ch_nodeForSort{
	int row;
	int col;
}CHNodeForSort;

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
	CH_GAME_FILE_PROBLEM,
  	CH_GAME_BLACK_WINS,
  	CH_GAME_WHITE_WINS,
  	CH_GAME_TIE,
  	CH_GAME_NO_WIN_OR_TIE,
  	CH_GAME_NO_HISTORY
} CH_GAME_MESSAGE;

/** Prints the board of the current game.
 *  @param src - the source game.
 *  @return
 *  CH_GAME_INVALID_ARGUMENT - if src is NULL.
 *  CH_GAME_SUCCESS - Otherwise.
 */
CH_GAME_MESSAGE chGamePrintBoard(CHGame* src);

/**
 * Sets the parameters(difficulty,userColor) and creates a history list for the undo move of the player for a game in '1-player' mode.
 * @userColor - The value can be 0 or 1, where 0 and 1 symbolize the black and white colors, respectively.
 *              if the game set to 'two players mode' the value is ignored.
 * @difficulty - The value can be 1,2,3 or 4. Sets the difficulty level to noob, easy, moderate and hard, respectively.
 * @return
 * 0 - If either a memory allocation failure occurs,difficulty is not 1,2,3 or 4,
 * userColor is not 0 or 1.
 * 1 - Otherwise.
 */
int chGameCreateMode1(CHGame* src,int difficulty,int userColor);

/**
 * Creates a new game with a specified game mode,current Turn,user Color and difficulty(if in 1 player mode).
 * @gameMode - The in with the game will be played.can be wither  1 or 2.
 *              i. 1 � one player mode (a player vs. AI)
 *             ii. 2 � two players mode
 * @userColor - Is relevant only if the game is set to a '1-player'.
 *              The value can be 0 or 1, where 0 and 1 symbolize the black and white colors, respectively.
 *              if the game set to 'two players mode' the value is ignored.
 * @difficulty - Only relevant when the game is set to 1 player mode.
 *               The value can be 1,2,3 or 4. Sets the difficulty level to noob, easy, moderate and hard, respectively.
 * @currentTurn - The turn that is played first 0 or 1(black and white respectively).
 * @return
 * NULL if either a memory allocation failure occurs,gameMode is not 1 or 2,difficulty is not 1,2,3 or 4(if game mode is '1-player'),
 * userColor is not 0 or 1 (if game mode is '1-player').
 * Otherwise, a new game instant is returned.
 */
CHGame* chGameCreate(int gameMode,int userColor,int difficulty,int currentTurn);

/**
 * Frees all memory allocation associated with a given game.
 * If src == NULL the function does nothing.
 *
 * @param src - the source game
 */
void chGameDestroy(CHGame* src);

/** Prints all possible moves of the piece located at position <fRow,fCol>.
 * @param src - The source game.
 * @param fRow - The row of the piece on the board.
 * @param fCol - The column of the piece on the board.
 * @return
 * CH_GAME_INVALID_ARGUMENT - if src is NULL
 * CH_GAME_INVALID_COLOR - If position <x,y> does not contain a piece of the user's color.
 * CH_GAME_MEMORY_PROBLEM - If can't allocate memory for the list of moves
 * CH_GAME_NO_MOVES - if there isn't any moves for this piece
 * CH_GAME_SUCCESS - otherwise
 */
CH_GAME_MESSAGE chGameShowMoves(CHGame* src, int fRow,int fCol,Widget *widget);

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
 * CH_GAME_INVALID_ARGUMENT - if src is NULL
 * CH_GAME_NO_WIN_OR_TIE - if there isn't a tie/checkmate
 * CH_GAME_BLACK_WINS - If there is a checkmate and black won.
 * CH_GAME_WHITE_WINS - If there is a checkmate and white won.
 * CH_GAME_TIE - if there is a tie
 */

CH_GAME_MESSAGE chIsCheckmateOrTie(CHGame* src);



/**
 * Checks if there is a check and prints the according message.
 *
 * @param src - The target game.
 * @return
 * CH_GAME_INVALID_ARGUMENT - if src is NULL.
 * CH_GAME_SUCCESS - otherwise.
 */
CH_GAME_MESSAGE isCheck(CHGame* src);



/**
 * Save the current game state to the specified file.
 *
 * @param src - The target game.
 * @param path - represent the file relative or full path.
 * @return
 * CH_GAME_INVALID_ARGUMENT - If src is NULL.
 * CH_GAME_FILE_PROBLEM - If the file cannot be created or modified.
 * CH_GAME_SUCCESS - otherwise.
 */
CH_GAME_MESSAGE chGameSave(CHGame* src,char *path);

/**
 * Creates a copy of a given game.
 * The new copy has the same status as the src game.
 *
 * @param src - the source game which will be copied
 * @return
 * NULL if either src is NULL or a memory allocation failure occurred.
 * Otherwise, an new copy of the source game is returned.
 *
 */
CHGame* chGameCopy(CHGame* src);

/**
 * Removes a piece that was put in the previous turn to its previous position and changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param src - The source game
 * @return
 * CH_GAME_INVALID_ARGUMENT 	 - if src == NULL
 * CH_GAME_GAME_NO_HISTORY       - if the user invoked this function more then
 *                                 historySize in a row.
 * CH_GAME_GAME_SUCCESS          - on success. The last disc that was put on the
 *                                 board is removed and the current player is changed
 */
CH_GAME_MESSAGE chGameUndoPrevMove(CHGame* src);

#endif /* CHGAME_H_ */
