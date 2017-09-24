/*
 * gameBoard.h
 *
 *  Created on: Sep 9, 2017
 *      Author: sasha
 */

#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_
#include "Widget.h"
#include "BoardCell.h"
#include "CHGame.h"
#define startY 60
#define startX 390

typedef struct gameBoard_t{
	SDL_Texture* gameBoardTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	Widget* gameBoard[8][8];
}GameBoard;

//You need a create function:
Widget* createGameBoard(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image,CHGame *game);
void setGlowCell(Widget* src,int i,int j,int color);
void setNoGlowCells(Widget* src);
//You need this function in order to destroy all data Associate with a button:
void destroyGameBoard(Widget*);

void handleGameBoardEvent(Widget* src, SDL_Event* event);

void drawGameBoard(Widget* src);

void nulifeDrag(Widget* src);

#endif /* GAMEBOARD_H_ */
