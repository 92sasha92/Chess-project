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
#define CH_ERROR_MOVE_DEPTH (-2)


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
 * @param gameBoard - a given game board of the current game
 * @param c - a representing symbol of the piece that the move list will be created for.
 * @param row - the row of the piece on the board
 * @param column - the column of the piece on the board
 * @param color - the color of the piece
 * @return
 * a move list such that
 * in every node of the list
 * row - contains a row of the possible move if isValid is true
 * column - contains a column of the possible move if isValid is true
 * isValid - set to true if the argument of the node where initialized
 * next - is a pointer to the next node or NULL if there isn't another one
 *
 * NULL - if there was memory allocation failure
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

/**
 * Checks if the piece in <mRow,mCol> is safe after the move of c from <fRow,fCol> to <toRow,toCol>. If c = '_' then there isn't a move and
 * the arguments fRow,fCol,toRow,toCol doesn't matter. if mode is 0 than the piece that will be checked will be always the king of the current player
 * otherwise any piece of the current player.
 * @param gameBoard - the current game board of the game
 * @param fRow - the row of the char c if its not '_'
 * @param fCol - the column of the char c if its not '_'
 * @param toRow - the row to move char c to if its not '_'
 * @param toCol - the column to move char c to if its not '_'
 * @param color - the color of the current player.
 * @param mRow - the row of the piece to check
 * @param mCol - the column of the piece to check
 * @return
 * 1 - if the piece is safe from the opponent after the move from <fRow,fCol> to <toRow,toCol>(if c != '_')
 * or if the piece is safe in the current board state (if c== '_').
 * 0 - if the piece is threatened by the opponent after the move from <fRow,fCol> to <toRow,toCol>(if c != '_')
 * or if the piece is threatened by the opponent in the current board state (if c== '_').
 */
bool isMyPieceSafe(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS], char c, int fRow, int fCol, int toRow, int toCol, int color, int mRow, int mCol,int mode);
#endif /* CHMOVES_H_ */
