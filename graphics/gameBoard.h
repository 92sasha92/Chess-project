#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_
#include "Widget.h"
#include "BoardCell.h"
#include "../CHGame.h"
#define startY 60
#define startX 390

typedef struct gameBoard_t {
	SDL_Texture* gameBoardTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	Widget* gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS];
} GameBoard;




/** destroy all data Associated with the cells of a GameBoard:
 * @param src - pointer to a GameBoard
 */
void destroyGameBoardCells(GameBoard* src);

/** creates a GameBoard widget and returns the pointer to it
 * @param windowRender - the renderer of the window the GameBoard is created on.
 * @param location - the location of the GameBoard on the window
 * @param image - the image of the GameBoard
 * @param game - the current game
 * @return
 * the pointer to the GameBoard widget or NULL if there was an error in creating the button
 */
Widget* createGameBoard(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image,CHGame *game);


/**
 * set the glow color of the cell in row i and column j
 * @param src - the pointer to the game board widget
 * @param i - the row of the cell
 * @param j - the column of the cell
 * @param color - the color of the glow
 */
void setGlowCell(Widget* src, int i, int j, int color);

/**
 * set all the cells on the game board to be without glow
 * @param src - the pointer to the game board widget
 */
void setNoGlowCells(Widget* src);

/** destroy all data Associated with a GameBoard:
 * @param src - pointer to a GameBoard widget
 */
void destroyGameBoard(Widget*);

/** handle GameBoard events
 * @param src - pointer to a GameBoard widget
 * @param event - pointer to an event
 */
void handleGameBoardEvent(Widget* src, SDL_Event* event);

/** draw the GameBoard
 * @param src - pointer to a GameBoard widget
 */
void drawGameBoard(Widget* src);

/**
 * set all the cells argument "isChosenByUser" to be 0
 */
void nulifeDrag(Widget* src);

#endif /* GAMEBOARD_H_ */
