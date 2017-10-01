/*
 * CHPiece.h
 *
 *  Created on: Sep 11, 2017
 *      Author: sasha
 */

#ifndef CHPIECE_H_
#define CHPIECE_H_

#include "Widget.h"

typedef struct CHPiece_t{
	SDL_Texture* Texture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	int isDragged;
	int deltaX;
	int deltaY;
}CHPiece;

/**
 * draws the piece and present it on the window
 * @param src - a pointer to a piece widget
 */
void drawCHPieceDrag(Widget* src);

SDL_Texture* createTexturePiece(SDL_Renderer* windowRender,const char* image);

/** creates a piece widget and returns the pointer to it
 * @param windowRender - the renderer of the window the button is created on.
 * @param location - the location of the button on the window
 * @param c - the character representing the piece to create
 * @return
 * the pointer to the piece widget or NULL if there was an error in creating the piece
 */
Widget* createCHPiece(SDL_Renderer* windowRender, SDL_Rect* location,
		char c);

/**
 * destroy all data Associate with the piece widget
 * @param src - a pointer to a piece widget
 */
void destroyCHPiece(Widget* src);

/** handle piece events
 * @param src - pointer to a button widget
 * @param event - pointer to an event
 */
void handleCHPieceEvent(Widget* src, SDL_Event* event);

/**
 * draws the piece
 * @param src - a pointer to a piece widget
 */
void drawCHPiece(Widget* src);


#endif /* CHPIECE_H_ */
