#ifndef SPSIMPLEMAINWINDOW_H_
#define SPSIMPLEMAINWINDOW_H_
#include "SPWindow.h"
#include "Widget.h"



typedef enum _windowType {
	CH_WINDOW_MAIN,
	CH_WINDOW_START,
	CH_WINDOW_MODE,
	CH_WINDOW_DIFFICULTY,
	CH_WINDOW_COLOR,
	CH_WINDOW_LOAD
} WindowType;

typedef struct sp_simplewindow_t {
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	//All widgets in our window
	Widget** widgets;
	int numOfWidgets;
	WindowType type;
	SDL_Texture* windowTexture;
} SPSimpleWindow;


/** creates a Main window and returns the pointer to it
 * @return
 * the pointer to the SPWindow of type CH_WINDOW_MAIN or NULL if there was an error in creating the window
 */
SPWindow* createSimpleWindow();

/** destroy all data Associated with a window:
 * @param src - pointer to a SPWindow
 */
void destroySimpleWindow(SPWindow* src);

/** draw the window and present it
 * @param src - pointer to a SPWindow
 */
void drawSimpleWindow(SPWindow* src);

/**
 * handles all window events
 * @param src - pointer to a SPWindow
 * @param event - a pointer to an event
 */
void handleEventSimpleWindow(SPWindow* src, SDL_Event* event);

#endif
