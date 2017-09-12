#ifndef SPSIMPLEMAINWINDOW_H_
#define SPSIMPLEMAINWINDOW_H_
#include "SPWindow.h"
#include "Widget.h"
#define GUI_ACTIVE 1


typedef enum _windowType{
	CH_WINDOW_MAIN,
	CH_WINDOW_START,
	CH_WINDOW_MODE,
	CH_WINDOW_DIFFICULTY,
	CH_WINDOW_COLOR,
	CH_WINDOW_LOAD
}WindowType;

typedef struct sp_simplewindow_t {
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	//All widgets in our window
	Widget** widgets;
	int numOfWidgets;
	WindowType type;
	SDL_Texture* windowTexture;
}SPSimpleWindow;

SPWindow* createSimpleWindow();
void destroySimpleWindow(SPWindow* src);
void drawSimpleWindow(SPWindow* src);
void handleEventSimpleWindow(SPWindow* src, SDL_Event* event);
#endif
