/*
 * CHPiece.c
 *
 *  Created on: Sep 11, 2017
 *      Author: sasha
 */


#include <stdlib.h>
#include "CHPiece.h"
#include "SPCommon.h"
#include "CHMoves.h"

void drawCHPieceDrag(Widget* src){
	if (src == NULL ) {
		return;
	}
	CHPiece* castData = (CHPiece*) src->data;
	SDL_RenderCopy(castData->windowRenderer, castData->Texture, NULL,castData->location);
	SDL_RenderPresent(castData->windowRenderer);
}


SDL_Texture* createTexturePiece(SDL_Renderer* windowRender,const char* image){
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	if(loadingSurface == NULL){
		return NULL;
	}
	SDL_Texture* gameBoardTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	SDL_FreeSurface(loadingSurface);
	return gameBoardTexture;
}


//You need a create function:
Widget* createCHPiece(SDL_Renderer* windowRender, SDL_Rect* location,char c){
	if (windowRender == NULL || location == NULL || c == CH_GAME_EMPTY_ENTRY) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	CHPiece* data = (CHPiece*) malloc(sizeof(CHPiece));
	SDL_Texture* gameBoardTexture;
	if (c == CH_WHITE_PAWN) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/pawnWhite.bmp");
	} else if (c == CH_BLACK_PAWN) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/pawnBlack.bmp");
	} else if (c == CH_WHITE_ROOK) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/rookWhite.bmp");
	} else if (c == CH_BLACK_ROOK) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/rookBlack.bmp");
	} else if (c == CH_WHITE_KNIGHT) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/knightWhite.bmp");
	} else if (c == CH_BLACK_KNIGHT) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/knightBlack.bmp");
	} else if (c == CH_WHITE_BISHOP) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/bishopWhite.bmp");
	} else if (c == CH_BLACK_BISHOP) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/bishopBlack.bmp");
	} else if (c == CH_WHITE_QUEEN) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/queenWhite.bmp");
	} else if (c == CH_BLACK_QUEEN) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/queenBlack.bmp");
	} else if (c == CH_WHITE_KING) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/kingWhite.bmp");
	} else if (c == CH_BLACK_KING) {
		gameBoardTexture = createTexturePiece(windowRender,"./images/kingBlack.bmp");
	}

	if (res == NULL || data == NULL || gameBoardTexture == NULL) {
		free(res);
		free(data);
		SDL_DestroyTexture(gameBoardTexture); ////It is safe to pass NULL
		printf("4/n");
		return NULL ;
	}
	data->Texture = gameBoardTexture;
	data->location = spCopyRect(location);
	data->windowRenderer = windowRender;
	data->isDragged = 0;
	res->destroyWidget = destroyCHPiece;
	res->drawWidget = drawCHPiece;
	res->handleEvent = handleCHPieceEvent;
	res->data = data;
	return res;
}

//You need this function in order to destroy all data Associate with a button:
void destroyCHPiece(Widget* src){
	if (src == NULL ) {
		return;
	}
	CHPiece* castData = (CHPiece*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->Texture);
	free(castData);
	free(src);
}


void handleCHPieceEvent(Widget* src, SDL_Event* event){
	if (src == NULL || event == NULL ) {
		return; //Better to return an error value
	}
	CHPiece* castData = (CHPiece*) src->data;
	if (event->type == SDL_MOUSEMOTION ) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (castData->isDragged) {
			    castData->location->x = point.x - castData->deltaX;
			    castData->location->y = point.y - castData->deltaY;
			    drawCHPieceDrag(src);
		}
	} else if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;

		if (SDL_PointInRect(&point, castData->location)) {
			castData->isDragged = 1;
			castData->deltaX = point.x - castData->location->x;
			castData->deltaY = point.y - castData->location->y;
				//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
						//"We did it", NULL );
		}
	} else if(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			castData->isDragged = 0;
				//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
						//"We did it", NULL );
		}
	}
	else{
		return;
	}
}


void drawCHPiece(Widget* src){
	if (src == NULL ) {
		return;
	}
	CHPiece* castData = (CHPiece*) src->data;
	SDL_RenderCopy(castData->windowRenderer, castData->Texture, NULL,
			castData->location);
}

