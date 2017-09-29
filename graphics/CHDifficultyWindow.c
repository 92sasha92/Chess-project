/*
 * CHDifficultyWindow.c
 *
 *  Created on: Sep 22, 2017
 *      Author: sasha
 */


#include <stdlib.h>
#include <stdio.h>
#include "SPSimpleMainWindow.h"
#include "CHDifficultyWindow.h"
#include "SimpleButton.h"
#define startBtnPosY 100


void pressDifficultyChange(SPWindow* src,int difficulty) {
	int i;
	if (src == NULL ) {
		return;
	}
	SPSimpleWindow * castData = (SPSimpleWindow *) src->data;
	SimpleButton* castBtnData;
	for (i = 0; i < castData->numOfWidgets; i++) {
		castBtnData = (SimpleButton*) castData->widgets[i]->data;
		if (i == difficulty - 1) {
			castBtnData->isPressed = 1;
		} else {
			castBtnData->isPressed = 0;
		}
	}
}


Widget** createSimpleDifficultyWindowWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	int i,numOfWidgets = 6;
	Widget** widgets = malloc(sizeof(Widget*) * numOfWidgets);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect backR = { .x = 50, .y = 400, .h = 80, .w = 296};
	SDL_Rect nextR = { .x = 454, .y = 400, .h = 80, .w = 296};
	SDL_Rect noobR = { .x = 100, .y = 160, .h = 80, .w = 296};
	SDL_Rect easyR = { .x = 100, .y = 240, .h = 80, .w = 296};
	SDL_Rect moderateR = { .x = 404, .y = 160, .h = 80, .w = 296};
	SDL_Rect hardR = { .x = 404, .y = 240, .h = 80, .w = 296};

	widgets[0] = createSimpleButton(renderer, &noobR, "./graphics/images/noobBtn.bmp",NULL,CH_BTN_EASY,0,BTN_ACTIVE);
	widgets[1] = createSimpleButton(renderer, &easyR, "./graphics/images/easyBtn.bmp",NULL,CH_BTN_NOOB,1,BTN_ACTIVE);
	widgets[2] = createSimpleButton(renderer, &moderateR, "./graphics/images/moderateBtn.bmp",NULL,CH_BTN_MODERATE,0,BTN_ACTIVE);
	widgets[3] = createSimpleButton(renderer, &hardR, "./graphics/images/hardBtn.bmp",NULL,CH_BTN_HARD,0,BTN_ACTIVE);
	widgets[4] = createSimpleButton(renderer, &backR, "./graphics/images/backBtn.bmp",NULL,CH_BTN_BACK,0,BTN_ACTIVE);
	widgets[5] = createSimpleButton(renderer, &nextR, "./graphics/images/nextBtn.bmp",NULL,CH_BTN_NEXT,0,BTN_ACTIVE);
	if (widgets[0] == NULL || widgets[1] == NULL || widgets[2] == NULL || widgets[3] == NULL || widgets[4] == NULL || widgets[5] == NULL) {
		for (i = 0; i < numOfWidgets; i++) {
			destroyWidget(widgets[i]);
		}
		free(widgets);
		printf("ERROR:\n");
		return NULL ;
	}
	return widgets;
}


SPWindow* createSimpleDifficultyWindow() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPSimpleWindow* data = malloc(sizeof(SPSimpleWindow));
	SDL_Window* window = SDL_CreateWindow("Tests", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 500, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	SDL_Surface* loadingSurface = SDL_LoadBMP("./graphics/images/difficultyB.bmp");
	SDL_Texture* windowTexture = SDL_CreateTextureFromSurface(renderer,loadingSurface);
	Widget** widgets = createSimpleDifficultyWindowWidgets(renderer);
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
	data->numOfWidgets = 6;
	data->window = window;
	data->windowRenderer = renderer;
	data->widgets = widgets;
	data->windowTexture = windowTexture;
	data->type = CH_WINDOW_DIFFICULTY;
	res->data = (void*) data;
	res->destroyWindow = destroySimpleWindow;
	res->drawWindow = drawSimpleWindow;
	res->handleEventWindow = handleEventSimpleWindow;
	return res;
}
