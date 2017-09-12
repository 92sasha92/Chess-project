/*
 * boardCell.h
 *
 *  Created on: Sep 9, 2017
 *      Author: sasha
 */

#ifndef BOARDCELL_H_
#define BOARDCELL_H_
#include "Widget.h"
#include "CHPiece.h"




#define CELL_GLOW_COLOR_NONE 0
#define CELL_GLOW_COLOR_REGULAR 1
#define CELL_GLOW_COLOR_BLUE 2
#define CELL_GLOW_COLOR_GREEN 3
#define CELL_GLOW_COLOR_RED 4

typedef struct BoardCell_t{
	SDL_Texture* regularBoardCellTexture;
	SDL_Texture* regularGlowTexture;
	SDL_Texture* blueGlowTexture;
	SDL_Texture* redGlowTexture;
	SDL_Texture* greenGlowTexture;
	int glow;
	int isChosenByUser;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	Widget* piece;
}BoardCell;

//You need a create function:
Widget* createBoardCell(SDL_Renderer* windowRender, SDL_Rect* location,const char* image,char c);

//You need this function in order to destroy all data Associate with a button:
void destroyBoardCell(Widget* src);

void handleBoardCellEvent(Widget* src, SDL_Event* event);

void drawBoardCell(Widget* src);

#endif /* BOARDCELL_H_ */
