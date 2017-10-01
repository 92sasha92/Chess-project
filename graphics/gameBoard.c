#include <stdlib.h>
#include "gameBoard.h"
#include "SPCommon.h"
#include "SimpleButton.h"
#include "../CHGame.h"
#define CELL_SIZE 77


void destroyGameBoardCells(GameBoard* src){
	int i, j;
	if(src == NULL){
		return;
	}
	for (i = 0; i < CH_GAME_N_ROWS; i++) {
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			destroyBoardCell(src->gameBoard[i][j]);
		}
	}
}

Widget* createGameBoard(SDL_Renderer* windowRender, SDL_Rect* location, const char* image, CHGame *game) {
	if (windowRender == NULL || location == NULL || image == NULL || game == NULL)
		return NULL ;
	int i, j,isWhite = 1, isProblemInMemory = 0;;
	Widget* res = (Widget*) malloc(sizeof(Widget));
	GameBoard* data = (GameBoard*) malloc(sizeof(GameBoard));
	SDL_Rect* copyLocation = spCopyRect(location);
	SDL_Surface* loadingSurface = SDL_LoadBMP(image);
	SDL_Texture* gameBoardTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	SDL_Rect loc = { .x = startX, .y = startY, .h = CELL_SIZE, .w = CELL_SIZE };
	char* imageBlack = "./graphics/images/blackSquare.bmp";
	char* imageWhite = "./graphics/images/whiteSquare.bmp";
	if(data != NULL){
		for (i = CH_GAME_N_ROWS - 1; i >=0; i--) {
			loc.x = startX;
			if (i != CH_GAME_N_ROWS - 1)
				loc.y += CELL_SIZE;
			for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
				loc.x += CELL_SIZE;
				if (isWhite) {
					data->gameBoard[i][j] = createBoardCell(windowRender, &loc,imageWhite,game->gameBoard[i][j]);
				} else
					data->gameBoard[i][j] = createBoardCell(windowRender, &loc,imageBlack,game->gameBoard[i][j]);
				if (data->gameBoard[i][j] == NULL) {
					isProblemInMemory = 1;
					break;
				}
				if (j != CH_GAME_N_COLUMNS - 1)
					isWhite = !isWhite;
			}
			if (isProblemInMemory)
				break;
		}
	}
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| gameBoardTexture == NULL || copyLocation == NULL || isProblemInMemory) {
		if(res != NULL)
			free(res);
		if(data != NULL){
			destroyGameBoardCells(data);
			free(data);
		}
		if(copyLocation != NULL)
			free(copyLocation);
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(gameBoardTexture);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	data->gameBoardTexture = gameBoardTexture;
	data->location = copyLocation;
	data->windowRenderer = windowRender;
	res->destroyWidget = destroyGameBoard;
	res->drawWidget = drawGameBoard;
	res->handleEvent = handleGameBoardEvent;
	res->data = data;
	return res;
}


void destroyGameBoard(Widget* src) {
	GameBoard* castData = (GameBoard*) src->data;
	if (src == NULL ) {
		return;
	}
	destroyGameBoardCells(castData);
	free(castData->location);
	SDL_DestroyTexture(castData->gameBoardTexture);
	free(castData);
	free(src);
}


void handleGameBoardEvent(Widget* src, SDL_Event* event) {
	int i, j;
	static int isDragged = 0;
	if (src == NULL || event==NULL) {
		return;
	}
	GameBoard* castData = (GameBoard*) src->data;
	SDL_Event user_event;
	SDL_Point point;
	point.x = event->button.x;
	point.y = event->button.y;
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		if (SDL_PointInRect(&point, castData->location)) {
			isDragged = 1;
		}
	}
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && isDragged == 1) {
		isDragged = 0;
		if (!SDL_PointInRect(&point, castData->location)) {
			user_event.type = SDL_USEREVENT;
			user_event.user.code = EVENT_DRAGGED_NOT_ON_BOARD;
			SDL_PushEvent(&user_event);
		}
	}
	for (i = 0; i < CH_GAME_N_ROWS; i++) {
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			castData->gameBoard[i][j]->handleEvent(castData->gameBoard[i][j],event);
		}
	}
}


void setGlowCell(Widget* src, int i, int j, int color) {
	if (src == NULL ) {
		return;
	}
	GameBoard* castData = (GameBoard*) src->data;
	((BoardCell *)(castData->gameBoard[i][j]->data))->glow = color;
}


void setNoGlowCells(Widget* src) {
	if (src == NULL ) {
		return;
	}
	int i, j;
	GameBoard* castData = (GameBoard*) src->data;
	for (i = 0; i < CH_GAME_N_ROWS; i++) {
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			((BoardCell *)(castData->gameBoard[i][j]->data))->glow = CELL_GLOW_COLOR_NONE;
		}
	}
}


void drawGameBoard(Widget* src) {
	if (src == NULL ) {
		return;
	}
	int i,j;
	GameBoard* castData = (GameBoard*) src->data;
	for (i = 0; i < CH_GAME_N_ROWS; i++) {
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			drawBoardCell(castData->gameBoard[i][j]);
		}
	}
	SDL_RenderCopy(castData->windowRenderer, castData->gameBoardTexture, NULL,
			castData->location);
}


void nulifeDrag(Widget* src) {
	if (src == NULL ) {
		return;
	}
	int i, j;
	GameBoard* castData = (GameBoard*) src->data;
	BoardCell* cellData;
	for (i = 0; i < CH_GAME_N_ROWS; i++) {
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			cellData = (BoardCell*) castData->gameBoard[i][j]->data;
			cellData->isChosenByUser = 0;
			if (cellData->piece != NULL) {
				((CHPiece* )cellData->piece->data)->isDragged = 0;
			}
		}
	}
}
