/*
 * CHModeWindow.c
 *
 *  Created on: Sep 11, 2017
 *      Author: sasha
 */

#include <stdlib.h>
#include <stdio.h>
#include "SPSimpleMainWindow.h"
#include "SimpleButton.h"
#define startBtnPosY 100





Widget** createSimpleModeWindowWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*) * 4);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect backR = { .x = 52, .y = 400, .h = 80, .w = 296 };
	SDL_Rect nextR = { .x = 400, .y = 400, .h = 80, .w = 296  };
	SDL_Rect onePlayerR = { .x = 252, .y = 100, .h = 80, .w = 296  };
	SDL_Rect twoPlayersR = { .x = 252, .y = 200, .h = 80, .w = 296  };
	widgets[0] = createSimpleButton(renderer, &backR, "./backBtn.bmp",NULL,CH_BTN_BACK,0,BTN_ACTIVE);
	widgets[1] = createSimpleButton(renderer, &nextR, "./nextBtn.bmp","./startBtn.bmp",CH_BTN_NEXT,0,BTN_ACTIVE);
	widgets[2] = createSimpleButton(renderer, &onePlayerR, "./onePlayerBtn.bmp",NULL,CH_BTN_ONE_PLAYER,1,BTN_ACTIVE);
	widgets[3] = createSimpleButton(renderer, &twoPlayersR, "./twoPlayersBtn.bmp",NULL,CH_BTN_TWO_PLAYERS,0,BTN_ACTIVE);
	if (widgets[0] == NULL || widgets[1] == NULL || widgets[2] == NULL || widgets[3] == NULL) {
		destroyWidget(widgets[0]); //NULL SAFE
		destroyWidget(widgets[1]); //NULL SAFE
		destroyWidget(widgets[2]);
		destroyWidget(widgets[3]);
		free(widgets);
		printf("ERROR:\n");
		return NULL ;
	}
	return widgets;
}


SPWindow* createSimpleModeWindow() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPSimpleWindow* data = malloc(sizeof(SPSimpleWindow));
	SDL_Window* window = SDL_CreateWindow("Tests", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 500, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	SDL_Surface* loadingSurface = SDL_LoadBMP("./background.bmp");
	SDL_Texture* windowTexture = SDL_CreateTextureFromSurface(renderer,loadingSurface);
	Widget** widgets = createSimpleModeWindowWidgets(renderer);
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
	data->numOfWidgets = 4;
	data->window = window;
	data->windowRenderer = renderer;
	data->widgets = widgets;
	data->windowTexture = windowTexture;
	data->type = CH_WINDOW_MODE;
	res->data = (void*) data;
	res->destroyWindow = destroySimpleWindow;
	res->drawWindow = drawSimpleWindow;
	res->handleEventWindow = handleEventSimpleWindow;
	return res;
}