#include <stdlib.h>
#include <stdio.h>
#include "SimpleButton.h"
#include "SPCommon.h"
#define btnDelta 9


void updateTextureBtn(Widget* src,int active) {
	if (src == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SDL_Texture* tempTexture;
	if (castData->isSecondTextureValid) {
		tempTexture = castData->buttonTexture;
		castData->buttonTexture = castData->secondTexture;
		castData->secondTexture = tempTexture;
	}
	castData->isActive = active;
}

int PointInRect(const SDL_Point *p, const SDL_Rect *r) {
    return ((p->x >= r->x + btnDelta) && (p->x < (r->x + r->w - btnDelta)) &&
             (p->y >= r->y + btnDelta) && (p->y < (r->y + r->h - btnDelta))) ? 1 : 0;
}


Widget* createSimpleButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image,const char* image2,BtnType type,int isPressed,int isActive) {
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	Widget* res = (Widget*) malloc(sizeof(Widget));
	SimpleButton* data = (SimpleButton*) malloc(sizeof(SimpleButton));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image),* loadingGlowSurface, *loadingSurface2 = NULL;
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	SDL_Texture *button2Texture = NULL, *glowTexture;
	SDL_Rect* copyLocation = spCopyRect(location);
	if (type == CH_BTN_BLACK_COLOR) {
		loadingGlowSurface = SDL_LoadBMP("./graphics/images/kingBlackGlowBtn.bmp");
	} else if (type == CH_BTN_WHITE_COLOR) {
		loadingGlowSurface = SDL_LoadBMP("./graphics/images/kingWhiteGlowBtn.bmp");
	} else {
		loadingGlowSurface = SDL_LoadBMP("./graphics/images/btnGlow.bmp");
	}
	glowTexture = SDL_CreateTextureFromSurface(windowRender,loadingGlowSurface);
	if (image2 != NULL) {
		loadingSurface2 = SDL_LoadBMP(image2);
		button2Texture = SDL_CreateTextureFromSurface(windowRender,loadingSurface2);
		SDL_FreeSurface(loadingSurface2);
	}
	if (res == NULL || data == NULL || loadingSurface == NULL || loadingGlowSurface == NULL
			|| buttonTexture == NULL || glowTexture == NULL || (image2 != NULL && button2Texture == NULL) || copyLocation == NULL) {
		if(res != NULL)
			free(res);
		if(data != NULL)
			free(data);
		if(copyLocation != NULL)
			free(copyLocation);
		SDL_FreeSurface(loadingSurface);
		SDL_FreeSurface(loadingGlowSurface);
		SDL_DestroyTexture(buttonTexture);
		SDL_DestroyTexture(button2Texture );
		SDL_DestroyTexture(glowTexture);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	SDL_FreeSurface(loadingGlowSurface);
	data->buttonTexture = buttonTexture;
	data->glowTexture = glowTexture;
	data->location = copyLocation;
	data->windowRenderer = windowRender;
	data->type = type;
	data->isPressed = isPressed;
	data->isActive = isActive;
	if (image2 != NULL) {
		data->isSecondTextureValid = 1;
		data->secondTexture = button2Texture;
	} else
		data->isSecondTextureValid = 0;
	res->destroyWidget = destroySimpleButton;
	res->drawWidget = drawSimpleButton;
	res->handleEvent = handleSimpleButtonEvent;
	res->data = data;
	return res;
}


void destroySimpleButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	free(castData->location);
	if (castData->isSecondTextureValid) {
		SDL_DestroyTexture(castData->secondTexture);
	}
	SDL_DestroyTexture(castData->buttonTexture);
	SDL_DestroyTexture(castData->glowTexture);
	free(castData);
	free(src);
}



void initilazeUserSlotEvent(SDL_Event *user_event, SimpleButton* btn) {
	if (btn->type == CH_BTN_GAME_SLOT1) {
		user_event->user.code = EVENT_SET_SLOT_1;
	} else if (btn->type == CH_BTN_GAME_SLOT2) {
		user_event->user.code = EVENT_SET_SLOT_2;
	} else if (btn->type == CH_BTN_GAME_SLOT3) {
		user_event->user.code = EVENT_SET_SLOT_3;
	} else if (btn->type == CH_BTN_GAME_SLOT4) {
		user_event->user.code = EVENT_SET_SLOT_4;
	} else if (btn->type == CH_BTN_GAME_SLOT5) {
		user_event->user.code = EVENT_SET_SLOT_5;
	}
}


void initilazeUserDifficultyEvent(SDL_Event *user_event, SimpleButton* btn) {
	if (btn->type == CH_BTN_NOOB) {
		user_event->user.code = EVENT_DIFFICULTY_NOOB;
	} else if (btn->type == CH_BTN_EASY) {
		user_event->user.code = EVENT_DIFFICULTY_EASY;
	} else if (btn->type == CH_BTN_MODERATE) {
		user_event->user.code = EVENT_DIFFICULTY_MODERATE;
	} else if (btn->type == CH_BTN_HARD) {
		user_event->user.code = EVENT_DIFFICULTY_HARD;
	}
}

void initilazeUserEvent(SDL_Event *user_event,SimpleButton* btn) {
	switch (btn->type) {
		case CH_BTN_EXIT:
			user_event->type = SDL_QUIT;
			break;
		case CH_BTN_NEW_GAME:
			user_event->user.code = EVENT_SHOW_GAME_MODE_SCREEN;
			break;
		case CH_BTN_MAIN_MENU:
			user_event->user.code = EVENT_GO_TO_MAIN_MENU;
			break;
		case CH_BTN_RESTART:
			user_event->user.code = EVENT_RESTART;
			break;
		case CH_BTN_SAVE:
			user_event->user.code = EVENT_SAVE;
			break;
		case CH_BTN_ONE_PLAYER:
			user_event->user.code = EVENT_UPDATE_TO_ONE_PLAYER;
			break;
		case CH_BTN_TWO_PLAYERS:
			user_event->user.code = EVENT_UPDATE_TO_TWO_PLAYERS;
			break;
		case CH_BTN_UNDO:
			user_event->user.code = EVENT_UNDO;
			break;
		case CH_BTN_BACK:
			user_event->user.code = EVENT_BACK;
			break;
		case CH_BTN_NEXT:
			user_event->user.code = EVENT_NEXT;
			break;
		case CH_BTN_START:
			user_event->user.code = EVENT_START_GAME;
			break;
		case CH_BTN_LOAD:
			user_event->user.code = EVENT_LOAD_WINDOW;
			break;
		case CH_BTN_NOOB:
		case CH_BTN_EASY:
		case CH_BTN_MODERATE:
		case CH_BTN_HARD:
			initilazeUserDifficultyEvent(user_event, btn);
			break;
		case CH_BTN_WHITE_COLOR:
			user_event->user.code = EVENT_SET_COLOR_WHITE;
			break;
		case CH_BTN_BLACK_COLOR:
			user_event->user.code = EVENT_SET_COLOR_BLACK;
			break;
		case CH_BTN_GAME_SLOT1:
		case CH_BTN_GAME_SLOT2:
		case CH_BTN_GAME_SLOT3:
		case CH_BTN_GAME_SLOT4:
		case CH_BTN_GAME_SLOT5:
			initilazeUserSlotEvent(user_event, btn);
			break;
	}
}

void handleSimpleButtonEvent(Widget* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SDL_Point point;
	point.x = event->button.x;
	point.y = event->button.y;
	SDL_Event user_event;
	if (event->type == SDL_MOUSEMOTION) {
		if (PointInRect(&point, castData->location)) {
			if (!(castData->isPressed) && castData->isActive == BTN_ACTIVE){
				drawSimpleGlow(src);
			}
		}
	} else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		if (PointInRect(&point, castData->location)) {
			if (castData->isActive == BTN_ACTIVE) {
				if (castData->type != CH_BTN_LOAD
						&& castData->type != CH_BTN_UNDO
						&& castData->type != CH_BTN_EXIT
						&& castData->type != CH_BTN_SAVE
						&& castData->type != CH_BTN_RESTART && castData->type != CH_BTN_MAIN_MENU) {
					castData->isPressed = 1;
				}
				user_event.type = SDL_USEREVENT;
				initilazeUserEvent(&user_event,castData);
				SDL_PushEvent(&user_event);
				if (castData->isPressed == 1) {
					drawSimpleGlow(src);
				}
			}
		}
	} else if (event->type == SDL_USEREVENT && event->user.code == EVENT_UPDATE_TO_ONE_PLAYER) {
		if (castData->type != CH_BTN_ONE_PLAYER)
			castData->isPressed = 0;
		if (castData->type == CH_BTN_START) {
			castData->type = CH_BTN_NEXT;
			updateTextureBtn(src,1);
		}
		user_event.type = SDL_USEREVENT;
		user_event.user.code = EVENT_UPDATE_WINDOW;
		SDL_PushEvent(&user_event);
	} else if (event->type == SDL_USEREVENT && event->user.code == EVENT_UPDATE_TO_TWO_PLAYERS) {
		if (castData->type != CH_BTN_TWO_PLAYERS)
			castData->isPressed = 0;
		if (castData->type == CH_BTN_NEXT) {
			castData->type = CH_BTN_START;
			updateTextureBtn(src,1);
		}
		user_event.type = SDL_USEREVENT;
		user_event.user.code = EVENT_UPDATE_WINDOW;
		SDL_PushEvent(&user_event);
	} else {
		return;
	}
}


void drawSimpleButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,
			castData->location);
	if (castData->isPressed)
		SDL_RenderCopy(castData->windowRenderer, castData->glowTexture, NULL,castData->location);
}


void drawSimpleGlow(Widget* src) {
	if (src == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SDL_RenderCopy(castData->windowRenderer, castData->glowTexture, NULL,
			castData->location);
	SDL_RenderPresent(castData->windowRenderer);
}

