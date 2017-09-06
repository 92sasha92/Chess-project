/*
 * CHMoves.h
 *
 *  Created on: Aug 29, 2017
 *      Author: sasha
 */

#ifndef CHMOVES_H_
#define CHMOVES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CH_GAME_N_ROWS 8
#define CH_GAME_N_COLUMNS 8
#define CH_GAME_EMPTY_ENTRY '_'
#define CH_BLACK_PAWN 'M'
#define CH_BLACK_ROOK 'R'
#define CH_BLACK_KNIGHT 'N'
#define CH_BLACK_BISHOP 'B'
#define CH_BLACK_KING 'K'
#define CH_BLACK_QUEEN 'Q'
#define CH_WHITE_PAWN 'm'
#define CH_WHITE_ROOK 'r'
#define CH_WHITE_KNIGHT 'n'
#define CH_WHITE_BISHOP 'b'
#define CH_WHITE_KING 'k'
#define CH_WHITE_QUEEN 'q'
#define CH_GAME_BLACK_PLAYER_SYMBOL 0
#define CH_GAME_WHITE_PLAYER_SYMBOL 1
#define MAX_LINE_LENGTH 1024
#define KING_MODE 0
#define REGULAR_PIECE_MODE 1


typedef struct ch_list{
	int row;
	int col;
	bool isValid;
	struct ch_list *next;
}CHMovesList;

/** Checks if the char c is a symbol representing a Black piece.
 * @param c - a char representing a piece on the board.
 * @return
 * 1 - if the char representing a Black piece symbol.
 * 0 - Otherwise.
 */
bool isABlackPiece(char c);

/** Checks if the char c is a symbol representing a White piece.
 * @param c - a char representing a piece on the board.
 * @return
 * 1 - if the char representing a White piece symbol.
 * 0 - Otherwise.
 */
bool isAWhitePiece(char c);

/** Finds the king of your color on the current board game and puts his place in the given addresses kRow and kCol.
 * @param kRow - and address of an int that will hold the row of the king when found.
 * @param kCol - and address of an int that will hold the column of the king when found.
 * if the king isn't on the board the game isn't legal so the function wont do anything.
 */
void findKing(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],int color,int *kRow,int *kCol);






/** Creates a move list of possible moves for a given piece on the board game.
 * @param gameBoard - a given game board of the currnet game
 * @param c - a representing symbol of the piece that the move list will be created for.
 * @
 *
 */
CHMovesList *createMoveList(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],char c,int row,int col,int color);

/**
 * Frees all memory allocation of the move list.
 * If list == NULL the function does nothing.
 *
 * @param list - the move list
 */
void destroyMoveList(CHMovesList *list);

/** Checks if the char c is a symbol representing a piece of the color given as a param to the function.
 * @param c - a char representing a piece on the board.
 * @param color - the color to check.
 * @return
 * 1 - if the char c representing a piece of the color given to the function.
 * 0 - Otherwise.
 */
bool isThePieceMyColor(char c, int color);
bool isMyPieceSafe(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS], char c, int fRow, int fCol, int toRow, int toCol, int color, int mRow, int mCol,int mode);
#endif /* CHMOVES_H_ */
