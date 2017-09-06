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


bool isABlackPiece(char c);

bool isAWhitePiece(char c);

void findKing(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],int color,int *kRow,int *kCol);

CHMovesList *createMoveList(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS],char c,int row,int col,int color);

void destroyMoveList(CHMovesList *list);

bool isThePieceMyColor(char c, int color);
bool isMyPieceSafe(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS], char c, int fRow, int fCol, int toRow, int toCol, int color, int mRow, int mCol,int mode);
bool isTheKingSafe(char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS], char c, int fRow, int fCol, int toRow, int toCol, int color, int kRow, int kCol);
#endif /* CHMOVES_H_ */
