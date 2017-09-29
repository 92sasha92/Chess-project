/*
 * CHStartWindow.c
 *
 *  Created on: Sep 10, 2017
 *      Author: sasha
 */
#include <stdlib.h>
#include <stdio.h>
#include "SPSimpleMainWindow.h"
#include "SimpleButton.h"
#define startBtnPosY 100


Widget** createSimpleStartWindowWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*) * 3);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect newGameR = { .x = 52, .y = startBtnPosY, .h = 80, .w = 296 };
	SDL_Rect loadR = { .x = 52, .y = startBtnPosY + 90, .h = 80, .w = 296  };
	SDL_Rect exitR = { .x = 52, .y = startBtnPosY + 90*2, .h = 80, .w = 296  };
	widgets[0] = createSimpleButton(renderer, &newGameR, "./graphics/images/newGameBtn.bmp",NULL,CH_BTN_NEW_GAME,0,BTN_ACTIVE);
	widgets[1] = createSimpleButton(renderer, &loadR, "./graphics/images/loadBtn.bmp",NULL,CH_BTN_LOAD,0,BTN_ACTIVE);
	widgets[2] = createSimpleButton(renderer, &exitR, "./graphics/images/exitBtn.bmp",NULL,CH_BTN_EXIT,0,BTN_ACTIVE);
	if (widgets[0] == NULL || widgets[1] == NULL || widgets[2] == NULL) {
		destroyWidget(widgets[0]); //NULL SAFE
		destroyWidget(widgets[1]); //NULL SAFE
		destroyWidget(widgets[2]);
		free(widgets);
		printf("ERROR:\n");
		return NULL ;
	}
	return widgets;
}


SPWindow* createSimpleStartWindow() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPSimpleWindow* data = malloc(sizeof(SPSimpleWindow));
	SDL_Window* window = SDL_CreateWindow("Tests", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	SDL_Surface* loadingSurface = SDL_LoadBMP("./graphics/images/background.bmp");
	SDL_Texture* windowTexture = SDL_CreateTextureFromSurface(renderer,loadingSurface);
	Widget** widgets = createSimpleStartWindowWidgets(renderer);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL
			|| widgets == NULL || windowTexture == NULL || loadingSurface == NULL) {
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
	data->numOfWidgets = 3;
	data->window = window;
	data->windowRenderer = renderer;
	data->widgets = widgets;
	data->windowTexture = windowTexture;
	data->type = CH_WINDOW_START;
	res->data = (void*) data;
	res->destroyWindow = destroySimpleWindow;
	res->drawWindow = drawSimpleWindow;
	res->handleEventWindow = handleEventSimpleWindow;
	return res;
}


