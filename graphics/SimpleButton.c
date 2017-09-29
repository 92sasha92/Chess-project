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

SDL_FORCE_INLINE SDL_bool PointInRect(const SDL_Point *p, const SDL_Rect *r) {
    return ((p->x >= r->x + btnDelta) && (p->x < (r->x + r->w - btnDelta)) &&
             (p->y >= r->y + btnDelta) && (p->y < (r->y + r->h - btnDelta))) ? SDL_TRUE : SDL_FALSE;
}


//You need a create function:
Widget* createSimpleButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image,const char* image2,BtnType type,int isPressed,int isActive) {
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	SimpleButton* data = (SimpleButton*) malloc(sizeof(SimpleButton));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_SetColorKey(loadingSurface, SDL_TRUE,SDL_MapRGB(loadingSurface->format, 255, 255, 255));
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	SDL_Surface* loadingGlowSurface; //We use the surface as a temp var;
	if (type == CH_BTN_BLACK_COLOR || type == CH_BTN_WHITE_COLOR) {
		if (type == CH_BTN_BLACK_COLOR ) {
			loadingGlowSurface = SDL_LoadBMP("./graphics/images/kingBlackGlowBtn.bmp");
		} else {
			loadingGlowSurface = SDL_LoadBMP("./graphics/images/kingWhiteGlowBtn.bmp");
		}
	} else {
		loadingGlowSurface = SDL_LoadBMP("./graphics/images/btnGlow.bmp");
	}
	SDL_SetColorKey(loadingGlowSurface, SDL_TRUE,SDL_MapRGB(loadingGlowSurface->format, 255, 255, 255));
	SDL_Texture* glowTexture = SDL_CreateTextureFromSurface(windowRender,loadingGlowSurface);
	SDL_Surface* loadingSurface2;
	SDL_Texture* button2Texture;
	if (image2 != NULL) {
		loadingSurface2 = SDL_LoadBMP(image2);
		button2Texture = SDL_CreateTextureFromSurface(windowRender,loadingSurface2);
		SDL_FreeSurface(loadingSurface2);
	}
	if (res == NULL || data == NULL || loadingSurface == NULL || loadingGlowSurface == NULL
			|| buttonTexture == NULL || glowTexture == NULL || (image2 != NULL && button2Texture == NULL)) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_FreeSurface(loadingGlowSurface);
		SDL_DestroyTexture(buttonTexture);
		SDL_DestroyTexture(button2Texture );
		SDL_DestroyTexture(glowTexture);
		////It is safe to pass NULL
		printf("5/n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //Surface is not actually needed after texture is created
	SDL_FreeSurface(loadingGlowSurface);
	data->buttonTexture = buttonTexture;
	data->glowTexture = glowTexture;
	data->location = spCopyRect(location);
	data->windowRenderer = windowRender;
	data->type = type;
	data->isPressed = isPressed;
	data->isActive = isActive;
	if (image2 != NULL) {
		data->isSecondTextureValid = 1;
		data->secondTexture = button2Texture;
	} else {
		data->isSecondTextureValid = 0;
	}
	res->destroyWidget = destroySimpleButton;
	res->drawWidget = drawSimpleButton;
	res->handleEvent = handleSimpleButtonEvent;
	res->data = data;
	return res;
}

//You need this function in order to destroy all data Associate with a button:
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


void initilazeUserEvent(SDL_Event *user_event,SimpleButton* btn) {
	switch (btn->type) {
	case CH_BTN_EXIT:
		btn->isPressed = 0;
		user_event->type = SDL_QUIT;
		break;
	case CH_BTN_NEW_GAME:
		user_event->user.code = EVENT_SHOW_GAME_MODE_SCREEN;
		break;
	case CH_BTN_MAIN_MENU:
		btn->isPressed = 0;
		user_event->user.code = EVENT_GO_TO_MAIN_MENU;
		break;
	case CH_BTN_RESTART:
		btn->isPressed = 0;
		user_event->user.code = EVENT_RESTART;
		break;
	case CH_BTN_SAVE:
		btn->isPressed = 0;
		user_event->user.code = EVENT_SAVE;
		break;
	case CH_BTN_ONE_PLAYER:
		user_event->user.code = EVENT_UPDATE_TO_ONE_PLAYER;
		break;
	case CH_BTN_TWO_PLAYERS:
		user_event->user.code = EVENT_UPDATE_TO_TWO_PLAYERS;
		break;
	case CH_BTN_UNDO:
		btn->isPressed = 0;
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
		btn->isPressed = 0;
		user_event->user.code = EVENT_LOAD_WINDOW;
		break;
	case CH_BTN_NOOB:
		user_event->user.code = EVENT_DIFFICULTY_NOOB;
		break;
	case CH_BTN_EASY:
		user_event->user.code = EVENT_DIFFICULTY_EASY;
		break;
	case CH_BTN_MODERATE:
		user_event->user.code = EVENT_DIFFICULTY_MODERATE;
		break;
	case CH_BTN_HARD:
		user_event->user.code = EVENT_DIFFICULTY_HARD;
		break;
	case CH_BTN_WHITE_COLOR:
		user_event->user.code = EVENT_SET_COLOR_WHITE;
		break;
	case CH_BTN_BLACK_COLOR:
		user_event->user.code = EVENT_SET_COLOR_BLACK;
		break;
	case CH_BTN_GAME_SLOT1:
		user_event->user.code = EVENT_SET_SLOT_1;
		break;
	case CH_BTN_GAME_SLOT2:
		user_event->user.code = EVENT_SET_SLOT_2;
		break;
	case CH_BTN_GAME_SLOT3:
		user_event->user.code = EVENT_SET_SLOT_3;
		break;
	case CH_BTN_GAME_SLOT4:
		user_event->user.code = EVENT_SET_SLOT_4;
		break;
	case CH_BTN_GAME_SLOT5:
		user_event->user.code = EVENT_SET_SLOT_5;
		break;
	}
}

void handleSimpleButtonEvent(Widget* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return; //Better to return an error value
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SDL_Point point;
	SDL_Event user_event;
	point.x = event->button.x;
	point.y = event->button.y;
	if (event->type == SDL_MOUSEMOTION) {
		if (PointInRect(&point, castData->location)) {
			if (!(castData->isPressed) && castData->isActive == BTN_ACTIVE)
				drawSimpleGlow(src);
			    //castData->location->x = point.x;
			   // castData->location->y = point.y;
				//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
						//"We did it", NULL );
		}
	} else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		if (PointInRect(&point, castData->location)) {
			if (castData->isActive == BTN_ACTIVE) {
				castData->isPressed = 1;
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

