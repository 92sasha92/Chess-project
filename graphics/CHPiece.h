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

//You need a create function:
Widget* createCHPiece(SDL_Renderer* windowRender, SDL_Rect* location,
                      char c);

//You need this function in order to destroy all data Associate with a button:
void destroyCHPiece(Widget* src);

void handleCHPieceEvent(Widget* src, SDL_Event* event);

void drawCHPiece(Widget* src);


#endif /* CHPIECE_H_ */


