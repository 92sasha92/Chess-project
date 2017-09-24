/*
 * gameBoard.c
 *
 *  Created on: Sep 9, 2017
 *      Author: sasha
 */

#include <stdlib.h>
#include "gameBoard.h"
#include "SPCommon.h"
#include "SimpleButton.h"
#include "CHGame.h"


Widget* createGameBoard(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image,CHGame *game){
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	int i,j;
	Widget* res = (Widget*) malloc(sizeof(Widget));
	GameBoard* data = (GameBoard*) malloc(sizeof(GameBoard));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_Texture* gameBoardTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);

	SDL_SetTextureBlendMode(gameBoardTexture, SDL_BLENDMODE_BLEND);
	SDL_Rect loc = { .x = startX, .y = startY, .h = 77, .w = 77 };
	char* imageBlack = "blackSquare.bmp";
	char* imageWhite = "whiteSquare.bmp";
	int isWhite = 1;
	int isProblemInMemory = 0;
	for(i = 7;i >=0;i--){
		loc.x = startX;
		if(i != 7)
			loc.y += 77;
		for(j = 0;j < 8;j++){
			loc.x += 77;
			if(isWhite){
				data->gameBoard[i][j] = createBoardCell(windowRender, &loc,imageWhite,game->gameBoard[i][j]);
			}
			else
				data->gameBoard[i][j] = createBoardCell(windowRender, &loc,imageBlack,game->gameBoard[i][j]);
			if(data->gameBoard[i][j] == NULL){
				isProblemInMemory = 1;
				break;
			}
			if(j != 7){
				isWhite = !isWhite;
			}
		}
		if(isProblemInMemory)
			break;
	}
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| gameBoardTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(gameBoardTexture); ////It is safe to pass NULL
		for(i = 0;i < 8;i++){
			for(j = 0;j < 8;j++){
				destroyBoardCell(data->gameBoard[i][j]);
			}
		}
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //Surface is not actually needed after texture is created
	data->gameBoardTexture = gameBoardTexture;
	data->location = spCopyRect(location);
	data->windowRenderer = windowRender;
	res->destroyWidget = destroyGameBoard;
	res->drawWidget = drawGameBoard;
	res->handleEvent = handleGameBoardEvent;
	res->data = data;
	return res;
}

//You need this function in order to destroy all data Associate with a button:
void destroyGameBoard(Widget* src){
	int i,j;
	GameBoard* castData = (GameBoard*) src->data;
	if (src == NULL ) {
		return;
	}
	for(i = 0;i < 8;i++){
		for(j = 0;j < 8;j++){
			destroyBoardCell(castData->gameBoard[i][j]);
		}
	}
	free(castData->location);
	SDL_DestroyTexture(castData->gameBoardTexture);
	free(castData);
	free(src);
}

void handleGameBoardEvent(Widget* src, SDL_Event* event){
	int i,j;
	int static isDragged = 0;
	if(src == NULL || event==NULL){
		return;
	}
	GameBoard* castData = (GameBoard*) src->data;
	SDL_Event user_event;
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			isDragged = 1;
		}
	}
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && isDragged == 1) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		isDragged = 0;
		if (!SDL_PointInRect(&point, castData->location)) {
			user_event.type = SDL_USEREVENT;
			user_event.user.code = EVENT_DRAGGED_NOT_ON_BOARD;
			SDL_PushEvent(&user_event);
		}
	}

	for(i = 0;i < 8;i++){
		for(j = 0;j < 8;j++){
			castData->gameBoard[i][j]->handleEvent(castData->gameBoard[i][j],event);
		}
	}
}


void setGlowCell(Widget* src,int i,int j,int color){
	if (src == NULL ) {
		return;
	}
	GameBoard* castData = (GameBoard*) src->data;
	((BoardCell *)(castData->gameBoard[i][j]->data))->glow = color;
}

void setNoGlowCells(Widget* src){
	if (src == NULL ) {
		return;
	}
	int i,j;
	GameBoard* castData = (GameBoard*) src->data;
	for(i = 0;i < 8;i++){
		for(j = 0;j < 8;j++){
			((BoardCell *)(castData->gameBoard[i][j]->data))->glow = CELL_GLOW_COLOR_NONE;
		}
	}
}
void drawGameBoard(Widget* src){
	if (src == NULL ) {
		return;
	}
	int i,j;
	GameBoard* castData = (GameBoard*) src->data;
	for(i = 0;i < 8;i++){
		for(j = 0;j < 8;j++){
			drawBoardCell(castData->gameBoard[i][j]);
		}
	}
	SDL_RenderCopy(castData->windowRenderer, castData->gameBoardTexture, NULL,
			castData->location);

}

void nulifeDrag(Widget* src){
	if (src == NULL ) {
		return;
	}
	int i,j;
	GameBoard* castData = (GameBoard*) src->data;
	BoardCell* cellData;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			cellData = (BoardCell*) castData->gameBoard[i][j]->data;
			cellData->isChosenByUser = 0;
			if(cellData->piece != NULL){
				((CHPiece* )cellData->piece->data)->isDragged = 0;
			}
		}
	}
}


