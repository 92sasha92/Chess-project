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

typedef struct BoardCell_t {
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
} BoardCell;


/**
 * frees all the memory allocated for the textures of the input textures
 * @param gameBoardTexture, glowTexture, glowBlueTexture, glowRedTexture, glowGreenTexture - all textures that need to be destroyed
 */
void destroyBoardCellTextures(SDL_Texture* gameBoardTexture, SDL_Texture* glowTexture, SDL_Texture* glowBlueTexture, SDL_Texture* glowRedTexture, SDL_Texture* glowGreenTexture);

/**
 * frees all the memory allocated for the surfaces of the input surfaces
 * @param loadingSurface, glowSurface, glowBlueSurface, glowRedSurface, glowGreenSurface - all surfaces that need to be destroyed
 */
void destroyBoardCellSurfaces(SDL_Surface* loadingSurface, SDL_Surface* glowSurface, SDL_Surface* glowBlueSurface, SDL_Surface* glowRedSurface, SDL_Surface* glowGreenSurface);

/** creates a BoardCell widget and returns the pointer to it
 * @param windowRender - the renderer of the window the BoardCell is created on.
 * @param location - the location of the BoardCell on the window
 * @param image - the image of the BoardCell
 * @param c - the character representing the piece on that BoardCell
 * @return
 * the pointer to the BoardCell widget or NULL if there was an error in creating the BoardCell
 */
Widget* createBoardCell(SDL_Renderer* windowRender, SDL_Rect* location, const char* image, char c);

/** destroy all data Associate with a BoardCell:
 * @param src - pointer to a BoardCell widget
 */
void destroyBoardCell(Widget* src);

/** handle BoardCell events
 * @param src - pointer to a BoardCell widget
 * @param event - pointer to an event
 */
void handleBoardCellEvent(Widget* src, SDL_Event* event);

/** draw the BoardCell
 * @param src - pointer to a BoardCell widget
 */
void drawBoardCell(Widget* src);

/** draw the BoardCell glow
 * @param src - pointer to a BoardCell widget
 */
void drawGlowCell(Widget* src);

#endif /* BOARDCELL_H_ */
