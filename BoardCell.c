/*
 * boardCell.c
 *
 *  Created on: Sep 9, 2017
 *      Author: sasha
 */

#include <stdlib.h>
#include "BoardCell.h"
#include "SPCommon.h"
#include "CHMoves.h"
#include "SimpleButton.h"

//You need a create function:
Widget* createBoardCell(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image,char c){
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	BoardCell* data = (BoardCell*) malloc(sizeof(BoardCell));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_Texture* gameBoardTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	SDL_Surface* glowSurface = SDL_LoadBMP("./pieceGlow.bmp");
	SDL_Texture* glowTexture = SDL_CreateTextureFromSurface(windowRender,glowSurface);
	SDL_Surface* glowBlueSurface = SDL_LoadBMP("./cellBlueGlow.bmp");
	SDL_Texture* glowBlueTexture = SDL_CreateTextureFromSurface(windowRender,glowBlueSurface);
	SDL_Surface* glowRedSurface = SDL_LoadBMP("./cellRedGlow.bmp");
	SDL_Texture* glowRedTexture = SDL_CreateTextureFromSurface(windowRender,glowRedSurface);
	SDL_Surface* glowGreenSurface = SDL_LoadBMP("./cellGreenGlow.bmp");
	SDL_Texture* glowGreenTexture = SDL_CreateTextureFromSurface(windowRender,glowGreenSurface);
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| gameBoardTexture == NULL || glowBlueSurface == NULL
			|| glowBlueTexture == NULL || glowRedSurface == NULL
			|| glowRedTexture == NULL || glowGreenSurface == NULL
			|| glowGreenTexture == NULL ) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(gameBoardTexture); ////It is safe to pass NULL
		SDL_FreeSurface(glowSurface);
		SDL_DestroyTexture(glowTexture);
		SDL_FreeSurface(glowBlueSurface);
		SDL_DestroyTexture(glowBlueTexture);
		SDL_FreeSurface(glowRedSurface);
		SDL_DestroyTexture(glowRedTexture);
		SDL_FreeSurface(glowGreenSurface);
		SDL_DestroyTexture(glowGreenTexture);
		return NULL ;
	}
	SDL_FreeSurface(glowSurface);
	SDL_FreeSurface(loadingSurface);
	SDL_FreeSurface(glowBlueSurface);
	SDL_FreeSurface(glowRedSurface);
	SDL_FreeSurface(glowGreenSurface);

	if(c == '_'){
		data->piece = NULL;
	}else{
		data->piece = createCHPiece(windowRender,location,c);
        if(data->piece == NULL){
    		free(res);
    		free(data);
    		SDL_FreeSurface(loadingSurface);
    		SDL_DestroyTexture(gameBoardTexture);
    		SDL_FreeSurface(glowSurface);
    		SDL_DestroyTexture(glowTexture);
    		SDL_FreeSurface(glowBlueSurface);
			SDL_DestroyTexture(glowBlueTexture);
			SDL_FreeSurface(glowRedSurface);
			SDL_DestroyTexture(glowRedTexture);
			SDL_FreeSurface(glowGreenSurface);
			SDL_DestroyTexture(glowGreenTexture);
    		return NULL ;
        }
	}
	data->regularBoardCellTexture = gameBoardTexture;
	data->regularGlowTexture = glowTexture;
	data->blueGlowTexture = glowBlueTexture;
	data->redGlowTexture = glowRedTexture;
	data->greenGlowTexture = glowGreenTexture;
	data->location = spCopyRect(location);
	data->windowRenderer = windowRender;
	data->glow = CELL_GLOW_COLOR_NONE;
	data->isChosenByUser = 0;
	res->destroyWidget = destroyBoardCell;
	res->drawWidget = drawBoardCell;
	res->handleEvent = handleBoardCellEvent;
	res->data = data;
	return res;

}
void drawGlowCell(Widget* src){
	if (src == NULL ) {
		return;
	}
	BoardCell* castData = (BoardCell*) src->data;
	SDL_RenderCopy(castData->windowRenderer, castData->regularGlowTexture, NULL,
			castData->location);
	SDL_RenderPresent(castData->windowRenderer);
}
//You need this function in order to destroy all data Associate with a button:
void destroyBoardCell(Widget* src){
	if (src == NULL ) {
		return;
	}
	BoardCell* castData = (BoardCell*) src->data;
	if(castData->piece != NULL){
		castData->piece->destroyWidget(castData->piece);
	}
	free(castData->location);
	SDL_DestroyTexture(castData->regularBoardCellTexture);
	SDL_DestroyTexture(castData->regularGlowTexture);
	free(castData);
	free(src);
}

void handleBoardCellEvent(Widget* src, SDL_Event* event){
	static int isDragged = 0;
	if (src == NULL || event == NULL ) {
		return; //Better to return an error value
	}
	BoardCell* castData = (BoardCell*) src->data;
	SDL_Event user_event;
	if (event->type == SDL_MOUSEMOTION ) {
//		SDL_Point point;
//		point.x = event->button.x;
//		point.y = event->button.y;
//		if (SDL_PointInRect(&point, castData->location) && castData->isDragged) {
//			    castData->location->x = point.x - castData->deltaX;
//			    castData->location->y = point.y - castData->deltaY;
//				//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
//						//"We did it", NULL );
//		}
	} else if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_RIGHT) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			castData->isChosenByUser = 1;
			castData->glow = CELL_GLOW_COLOR_REGULAR;
			drawGlowCell(src);
			user_event.type = SDL_USEREVENT;
			user_event.user.code = EVENT_GET_MOVES;
			SDL_PushEvent(&user_event);
		}
	} else if(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && isDragged) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			castData->isChosenByUser = 1;
			isDragged = 0;
			user_event.type = SDL_USEREVENT;
			user_event.user.code = EVENT_MOVE;
			SDL_PushEvent(&user_event);
		}
	}else if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT ) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			if(castData->piece != NULL){
				castData->isChosenByUser = 1;
				isDragged = 1;
				user_event.type = SDL_USEREVENT;
				user_event.user.code = EVENT_BEGIN_DRAG;
				SDL_PushEvent(&user_event);
			}
				//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
						//"We did it", NULL );
		}
	}else if(event->type == SDL_USEREVENT && event->user.code == EVENT_DRAGGED_NOT_ON_BOARD){
		isDragged = 0;
	}
	if(castData->piece != NULL){
		castData->piece->handleEvent(castData->piece,event);
	}
}

void drawBoardCell(Widget* src){
	if (src == NULL ) {
		return;
	}
	BoardCell* castData = (BoardCell*) src->data;
	SDL_RenderCopy(castData->windowRenderer, castData->regularBoardCellTexture, NULL,
			castData->location);
	if(castData->glow == CELL_GLOW_COLOR_REGULAR){
		SDL_RenderCopy(castData->windowRenderer, castData->regularGlowTexture, NULL,
				castData->location);
	}else if(castData->glow == CELL_GLOW_COLOR_BLUE){
		SDL_RenderCopy(castData->windowRenderer, castData->blueGlowTexture, NULL,
				castData->location);
	}else if(castData->glow == CELL_GLOW_COLOR_GREEN){
		SDL_RenderCopy(castData->windowRenderer, castData->greenGlowTexture, NULL,
				castData->location);
	}else if(castData->glow == CELL_GLOW_COLOR_RED){
		SDL_RenderCopy(castData->windowRenderer, castData->redGlowTexture, NULL,
				castData->location);
	}
	if(castData->piece != NULL){
		castData->piece->drawWidget(castData->piece);
	}

}
