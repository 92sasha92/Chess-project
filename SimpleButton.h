#ifndef SIMPLEBUTTON_H_
#define SIMPLEBUTTON_H_

#include "Widget.h" //A button is a widget also

#define BTN_ACTIVE 1
#define BTN_NOT_ACTIVE 0
#define EVENT_UPDATE_WINDOW 0
#define EVENT_UPDATE_TO_ONE_PLAYER 1
#define EVENT_UPDATE_TO_TWO_PLAYERS 2
#define EVENT_START_GAME 5
#define EVENT_MOVE 6
#define EVENT_BEGIN_DRAG 7
#define EVENT_GET_MOVES 8
typedef enum _btnType{
	CH_BTN_EXIT,
	CH_BTN_NEW_GAME,
	CH_BTN_LOAD,
	CH_BTN_ONE_PLAYER,
	CH_BTN_TWO_PLAYERS,
	CH_BTN_RESTART,
	CH_BTN_SAVE,
	CH_BTN_MAIN_MENU,
	CH_BTN_UNDO,
	CH_BTN_BACK,
	CH_BTN_NEXT,
	CH_BTN_START
}BtnType;






typedef struct simplebutton_t{
	SDL_Texture* buttonTexture;
	int isSecondTextureValid;
	SDL_Texture* secondTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	SDL_Texture* glowTexture;
	BtnType type;
	int isPressed;
	int isActive;
}SimpleButton;

//You need a create function:
Widget* createSimpleButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image,const char* image2,BtnType type,int isPressed,int isActive);

//You need this function in order to destroy all data Associate with a button:
void destroySimpleButton(Widget*);

void handleSimpleButtonEvent(Widget* src, SDL_Event* event);

void drawSimpleButton(Widget* src);

void drawSimpleGlow(Widget* src);

#endif
