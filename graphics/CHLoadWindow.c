/*
 * CHLoadWindow.c
 *
 *  Created on: Sep 23, 2017
 *      Author: sasha
 */
#include <stdlib.h>
#include <stdio.h>
#include "SPSimpleMainWindow.h"
#include "SimpleButton.h"
#include "CHLoadWindow.h"

int getSlotPressed(SPWindow* src){
	int i;
	if (src == NULL ) {
		return 0;
	}
	SPSimpleWindow * castData = (SPSimpleWindow *) src->data;
	SimpleButton* castBtnData;
	for(i = 2;i < castData->numOfWidgets;i++){
		castBtnData = (SimpleButton*) castData->widgets[i]->data;
		if(castBtnData->isPressed == 1){
			return (i -1);
		}
	}
	return 0;
}

void pressSlotChange(SPWindow* src,int slot){
	int i;
	if (src == NULL ) {
		return;
	}
	SPSimpleWindow * castData = (SPSimpleWindow *) src->data;
	SimpleButton* castBtnData;
	for(i = 2;i < castData->numOfWidgets;i++){
		castBtnData = (SimpleButton*) castData->widgets[i]->data;
		if(i - 1 == slot){
			castBtnData->isPressed = 1;
		}
		else{
			castBtnData->isPressed = 0;
		}
	}
}

Widget** createLoadWindowWidgets(SDL_Renderer* renderer,int numOfWidgets) {
	if (renderer == NULL ) {
		return NULL ;
	}
	int i,j;
	Widget** widgets = malloc(sizeof(Widget*) * numOfWidgets);
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect slotR = { .x = 252, .y = 10, .h = 80, .w = 296 };
	SDL_Rect backR = { .x = 50, .y = 415, .h = 80, .w = 296 };
	SDL_Rect loadR = { .x = 454, .y = 415, .h = 80, .w = 296  };
	widgets[0] = createSimpleButton(renderer, &backR, "./graphics/images/backBtn.bmp",NULL,CH_BTN_BACK,0,BTN_ACTIVE);
	widgets[1] = createSimpleButton(renderer, &loadR, "./graphics/images/loadGrayBtn.bmp","./graphics/images/loadBtn.bmp",CH_BTN_LOAD,0,BTN_NOT_ACTIVE);
	for(i = 2;i < numOfWidgets;i++){
		switch(i){
			case 2:
				widgets[i] = createSimpleButton(renderer, &slotR, "./graphics/images/gameSlot1.bmp",NULL,CH_BTN_GAME_SLOT1,0,BTN_ACTIVE);
				break;
			case 3:
				widgets[i] = createSimpleButton(renderer, &slotR, "./graphics/images/gameSlot2.bmp",NULL,CH_BTN_GAME_SLOT2,0,BTN_ACTIVE);
				break;
			case 4:
				widgets[i] = createSimpleButton(renderer, &slotR, "./graphics/images/gameSlot3.bmp",NULL,CH_BTN_GAME_SLOT3,0,BTN_ACTIVE);
				break;
			case 5:
				widgets[i] = createSimpleButton(renderer, &slotR, "./graphics/images/gameSlot4.bmp",NULL,CH_BTN_GAME_SLOT4,0,BTN_ACTIVE);
				break;
			case 6:
				widgets[i] = createSimpleButton(renderer, &slotR, "./graphics/images/gameSlot5.bmp",NULL,CH_BTN_GAME_SLOT5,0,BTN_ACTIVE);
				break;
		}
		slotR.y += 80;
		if (widgets[i] == NULL) {
			for(j = 0;j <= i;j++){
				destroyWidget(widgets[j]);
			}
			free(widgets);
			printf("ERROR:\n");
			return NULL ;
		}

	}
	return widgets;
}


SPWindow* createLoadWindow() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPSimpleWindow* data = malloc(sizeof(SPSimpleWindow));
	SDL_Window* window = SDL_CreateWindow("Tests", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 500, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED);
	SDL_Surface* loadingSurface = SDL_LoadBMP("./graphics/images/background.bmp");
	SDL_Texture* windowTexture = SDL_CreateTextureFromSurface(renderer,loadingSurface);
	int numOfWidgets = 0;
	FILE *fp = fopen("./savedGames/readMeForLoad.txt","r");
	fscanf(fp,"%d",&numOfWidgets);
	numOfWidgets += 2;
	Widget** widgets = createLoadWindowWidgets(renderer,numOfWidgets);
	fclose(fp);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL
			|| widgets == NULL || windowTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(windowTexture);
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
	data->numOfWidgets = numOfWidgets;
	data->window = window;
	data->windowRenderer = renderer;
	data->widgets = widgets;
	data->windowTexture = windowTexture;
	data->type = CH_WINDOW_LOAD;
	res->data = (void*) data;
	res->destroyWindow = destroySimpleWindow;
	res->drawWindow = drawSimpleWindow;
	res->handleEventWindow = handleEventSimpleWindow;
	return res;
}
