#include <stdlib.h>
#include "SPSimpleMainWindow.h"
#include "SimpleButton.h"
#include "gameBoard.h"
#include "CHGame.h"
//Helper function to create buttons in the simple window;
#define startBtnPosY 150
Widget** createSimpleWindowWidgets(SDL_Renderer* renderer,CHGame *game) {
	if (renderer == NULL || game == NULL ) {
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*) * 7);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect undoR = { .x = 75, .y = startBtnPosY, .h = 80, .w = 296 };
	SDL_Rect saveR = { .x = 75, .y = startBtnPosY + 90, .h = 80, .w = 296  };
	SDL_Rect loadR = { .x = 75, .y = startBtnPosY + 90*2, .h = 80, .w = 296  };
	SDL_Rect restartR = { .x = 75, .y = startBtnPosY+ 90*3, .h = 80, .w = 296 };
	SDL_Rect mainMenuR = { .x = 75, .y = startBtnPosY+ 90*4, .h = 80, .w = 296  };
	SDL_Rect exitR = { .x = 75, .y = startBtnPosY+ 90*5, .h = 80, .w = 296  };
	SDL_Rect boardR = { .x = startX + 51, .y = startY - 25, .h = 667, .w = 668 };
	if(game->gameMode == 2){
		widgets[0] = createSimpleButton(renderer, &undoR, "./undoBtnGray.bmp",NULL,CH_BTN_UNDO,0,BTN_NOT_ACTIVE);
	}else{
		widgets[0] = createSimpleButton(renderer, &undoR, "./undoBtn.bmp",NULL,CH_BTN_UNDO,0,BTN_ACTIVE);
	}
	widgets[1] = createSimpleButton(renderer, &saveR, "./saveBtn.bmp",NULL,CH_BTN_SAVE,0,BTN_ACTIVE);
	widgets[2] = createSimpleButton(renderer, &loadR, "./loadBtn.bmp",NULL,CH_BTN_LOAD,0,BTN_ACTIVE);
	widgets[3] = createSimpleButton(renderer, &restartR, "./restartBtn.bmp",NULL,CH_BTN_RESTART,0,BTN_ACTIVE);
	widgets[4] = createSimpleButton(renderer, &mainMenuR, "./mainMenuBtn.bmp",NULL,CH_BTN_MAIN_MENU,0,BTN_ACTIVE);
	widgets[5] = createSimpleButton(renderer, &exitR, "./exitBtn.bmp",NULL,CH_BTN_EXIT,0,BTN_ACTIVE);
	widgets[6] = createGameBoard(renderer, &boardR, "./board.bmp",game);
	if (widgets[0] == NULL || widgets[1] == NULL || widgets[2] == NULL || widgets[3] == NULL || widgets[4] == NULL || widgets[5] == NULL || widgets[6] == NULL) {
		destroyWidget(widgets[0]); //NULL SAFE
		destroyWidget(widgets[1]); //NULL SAFE
		destroyWidget(widgets[2]);
		free(widgets);
		return NULL ;
	}
	return widgets;
}
SPWindow* createSimpleWindow(CHGame *game) {
	if(game == NULL){
		return NULL ;
	}
	SPWindow* res = malloc(sizeof(SPWindow));
	SPSimpleWindow* data = malloc(sizeof(SPSimpleWindow));
	SDL_Window* window = SDL_CreateWindow("Tests", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 1130, 730, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	SDL_Surface* loadingSurface = SDL_LoadBMP("background.bmp");
	SDL_Texture* windowTexture = SDL_CreateTextureFromSurface(renderer,loadingSurface);
	Widget** widgets = createSimpleWindowWidgets(renderer,game);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL
			|| widgets == NULL || windowTexture == NULL) {
		free(res);
		free(data);
		free(widgets);
		//We first destroy the renderer
		SDL_DestroyRenderer(renderer); //NULL safe
		SDL_DestroyWindow(window); //NULL safe
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	data->widgets = widgets;
	data->numOfWidgets = 7;
	data->window = window;
	data->windowRenderer = renderer;
	data->widgets = widgets;
	data->windowTexture = windowTexture;
	res->data = (void*) data;
	res->destroyWindow = destroySimpleWindow;
	res->drawWindow = drawSimpleWindow;
	res->handleEventWindow = handleEventSimpleWindow;
	return res;
}
void destroySimpleWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPSimpleWindow* data = (SPSimpleWindow*) src->data;
	int i = 0;
	for (; i < data->numOfWidgets; i++) {
		destroyWidget(data->widgets[i]);//
	}
	free(data->widgets);
	SDL_DestroyRenderer(data->windowRenderer);
	SDL_DestroyWindow(data->window);
	free(data);
	free(src);
}
void drawSimpleWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPSimpleWindow* data = (SPSimpleWindow*) src->data;
	//Draw window
	//SDL_SetRenderDrawColor(data->windowRenderer, 255, 255, 255, 255);
	SDL_RenderClear(data->windowRenderer);
	SDL_RenderCopy(data->windowRenderer, data->windowTexture, NULL,
			NULL);
	int i = 0;
	for (; i < data->numOfWidgets; i++) {
		(data->widgets[i])->drawWidget(data->widgets[i]);
	}
	SDL_RenderPresent(data->windowRenderer);
}

void handleEventSimpleWindow(SPWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL){
		return;
	}
	int i;
	SPSimpleWindow* data = (SPSimpleWindow*)src->data;
	if(event->type == SDL_MOUSEMOTION || (event->type == SDL_USEREVENT && event->user.code == EVENT_UPDATE_WINDOW)){
		drawSimpleWindow(src);
	}
	for(i = 0;i<data->numOfWidgets;i++){
		data->widgets[i]->handleEvent(data->widgets[i],event);
	}
	return;
}
