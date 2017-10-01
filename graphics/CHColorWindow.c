/*
 * CHChooseColorWindow.c
 *
 *  Created on: Sep 22, 2017
 *      Author: sasha
 */


#include <stdlib.h>
#include <stdio.h>
#include "SPSimpleMainWindow.h"
#include "CHColorWindow.h"
#include "SimpleButton.h"


void pressColorChange(SPWindow* src,int color) {
	if (src == NULL ) {
		return;
	}
	int i;
	SPSimpleWindow * castData = (SPSimpleWindow *) src->data;
	SimpleButton* castBtnData;
	for (i = 0; i < castData->numOfWidgets; i++) {
		castBtnData = (SimpleButton*) castData->widgets[i]->data;
		if (i == color) {
			castBtnData->isPressed = 1;
		} else {
			castBtnData->isPressed = 0;
		}
	}
}


Widget** createSimpleColorWindowWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	int i,numOfWidgets = 4;
	Widget** widgets = malloc(sizeof(Widget*) * numOfWidgets);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect backR = { .x = 50, .y = 400, .h = 80, .w = 296 };
	SDL_Rect nextR = { .x = 454, .y = 400, .h = 80, .w = 296  };
	SDL_Rect kingWhiteR = { .x = 110, .y = 90, .h = 280, .w = 286  };
	SDL_Rect kingBlackR = { .x = 404, .y = 90, .h = 280, .w = 286  };
	widgets[0] = createSimpleButton(renderer, &kingBlackR, "./graphics/images/kingBlackBtn.bmp",NULL,CH_BTN_BLACK_COLOR,0,BTN_ACTIVE);
	widgets[1] = createSimpleButton(renderer, &kingWhiteR, "./graphics/images/kingWhiteBtn.bmp",NULL,CH_BTN_WHITE_COLOR,1,BTN_ACTIVE);
	widgets[2] = createSimpleButton(renderer, &backR, "./graphics/images/backBtn.bmp",NULL,CH_BTN_BACK,0,BTN_ACTIVE);
	widgets[3] = createSimpleButton(renderer, &nextR, "./graphics/images/startBtn.bmp",NULL,CH_BTN_START,0,BTN_ACTIVE);
	if (widgets[0] == NULL || widgets[1] == NULL || widgets[2] == NULL || widgets[3] == NULL) {
		for(i = 0;i < numOfWidgets;i++){
			destroyWidget(widgets[i]);
		}
		free(widgets);
		return NULL ;
	}
	return widgets;
}


SPWindow* createSimpleColorWindow() {
	int i;
	SPWindow* res = malloc(sizeof(SPWindow));
	SPSimpleWindow* data = malloc(sizeof(SPSimpleWindow));
	SDL_Window* window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 500, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	SDL_Surface* loadingSurface = SDL_LoadBMP("./graphics/images/colorB.bmp");
	SDL_Texture* windowTexture = SDL_CreateTextureFromSurface(renderer,loadingSurface);
	Widget** widgets = createSimpleColorWindowWidgets(renderer);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL
			|| widgets == NULL || windowTexture == NULL) {
		if(res != NULL)
			free(res);
		if(data != NULL)
			free(data);
		if(widgets != NULL){
			for(i = 0;i < 4;i++){
				destroyWidget(widgets[i]);
			}
			free(widgets);
		}
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(windowTexture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	data->widgets = widgets;
	data->numOfWidgets = 4;
	data->window = window;
	data->windowRenderer = renderer;
	data->widgets = widgets;
	data->windowTexture = windowTexture;
	data->type = CH_WINDOW_COLOR;
	res->data = (void*) data;
	res->destroyWindow = destroySimpleWindow;
	res->drawWindow = drawSimpleWindow;
	res->handleEventWindow = handleEventSimpleWindow;
	return res;
}
