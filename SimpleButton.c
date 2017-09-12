#include <stdlib.h>
#include "SimpleButton.h"
#include "SPCommon.h"
#define btnDelta 9



void updateTextureBtn(Widget* src){
	if (src == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SDL_Texture* tempTexture;
	if(castData->isSecondTextureValid){
		tempTexture = castData->buttonTexture;
		castData->buttonTexture = castData->secondTexture;
		castData->secondTexture = tempTexture;
	}
}

SDL_FORCE_INLINE SDL_bool PointInRect(const SDL_Point *p, const SDL_Rect *r)
{
    return ( (p->x >= r->x + btnDelta) && (p->x < (r->x + r->w - btnDelta)) &&
             (p->y >= r->y + btnDelta) && (p->y < (r->y + r->h - btnDelta)) ) ? SDL_TRUE : SDL_FALSE;
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
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,
			loadingSurface);
	SDL_Surface* loadingSurface2 = SDL_LoadBMP("./btnGlow.bmp"); //We use the surface as a temp var;
	SDL_Texture* glowTexture = SDL_CreateTextureFromSurface(windowRender,
			loadingSurface2);
	SDL_Surface* loadingSurface3;
	SDL_Texture* button2Texture;
	if(image2 != NULL){
		loadingSurface3 = SDL_LoadBMP(image2);
		button2Texture = SDL_CreateTextureFromSurface(windowRender,loadingSurface3);
		SDL_FreeSurface(loadingSurface3);
	}
	if (res == NULL || data == NULL || loadingSurface == NULL || loadingSurface2 == NULL
			|| buttonTexture == NULL || glowTexture == NULL || (image2 != NULL && (loadingSurface3 == NULL || button2Texture == NULL))) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_FreeSurface(loadingSurface2);
		SDL_FreeSurface(loadingSurface3);
		SDL_DestroyTexture(buttonTexture);
		SDL_DestroyTexture(button2Texture );
		SDL_DestroyTexture(glowTexture);
		////It is safe to pass NULL
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //Surface is not actually needed after texture is created
	SDL_FreeSurface(loadingSurface2);
	data->buttonTexture = buttonTexture;
	data->glowTexture = glowTexture;
	data->location = spCopyRect(location);
	data->windowRenderer = windowRender;
	data->type = type;
	data->isPressed = isPressed;
	data->isActive = isActive;
	if(image2 != NULL){
		data->isSecondTextureValid = 1;
		data->secondTexture = button2Texture;
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
	SDL_DestroyTexture(castData->buttonTexture);
	SDL_DestroyTexture(castData->glowTexture);
	free(castData);
	free(src);
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
			if(!(castData->isPressed) && castData->isActive == BTN_ACTIVE)
				drawSimpleGlow(src);
			    //castData->location->x = point.x;
			   // castData->location->y = point.y;
				//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
						//"We did it", NULL );
		}
	} else if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
		if (PointInRect(&point, castData->location)) {
			if (castData->isActive == BTN_ACTIVE) {
				castData->isPressed = 1;
				drawSimpleGlow(src);
				switch (castData->type) {
				case CH_BTN_EXIT:
					user_event.type = SDL_QUIT;
					SDL_PushEvent(&user_event);
					break;
				case CH_BTN_NEW_GAME:
					user_event.type = SDL_USEREVENT;
					user_event.user.code = 3;
					SDL_PushEvent(&user_event);
					break;
				case CH_BTN_MAIN_MENU:
				case CH_BTN_ONE_PLAYER:
					user_event.type = SDL_USEREVENT;
					user_event.user.code = EVENT_UPDATE_TO_ONE_PLAYER;
					SDL_PushEvent(&user_event);
					break;
				case CH_BTN_RESTART:
				case CH_BTN_SAVE:
				case CH_BTN_TWO_PLAYERS:
					user_event.type = SDL_USEREVENT;
					user_event.user.code = EVENT_UPDATE_TO_TWO_PLAYERS;
					SDL_PushEvent(&user_event);
					break;
				case CH_BTN_UNDO:
				case CH_BTN_BACK:
				case CH_BTN_NEXT:
				case CH_BTN_START:
					user_event.type = SDL_USEREVENT;
					user_event.user.code = EVENT_START_GAME;
					SDL_PushEvent(&user_event);
					break;
				case CH_BTN_LOAD:
					break;
				}
			}
		}
	}else if(event->type == SDL_USEREVENT && event->user.code == 1){
		if(castData->type != CH_BTN_ONE_PLAYER)
			castData->isPressed = 0;
		if(castData->type == CH_BTN_START){
			castData->type = CH_BTN_NEXT;
			updateTextureBtn(src);
		}
		user_event.type = SDL_USEREVENT;
		user_event.user.code = EVENT_UPDATE_WINDOW;
		SDL_PushEvent(&user_event);
	}else if(event->type == SDL_USEREVENT && event->user.code == 2){
		if(castData->type != CH_BTN_TWO_PLAYERS)
			castData->isPressed = 0;
		if(castData->type == CH_BTN_NEXT){
			castData->type = CH_BTN_START;
			updateTextureBtn(src);
		}
		user_event.type = SDL_USEREVENT;
		user_event.user.code = EVENT_UPDATE_WINDOW;
		SDL_PushEvent(&user_event);
	}else{
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
	if(castData->isPressed)
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

