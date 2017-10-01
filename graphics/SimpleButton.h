#ifndef SIMPLEBUTTON_H_
#define SIMPLEBUTTON_H_

#include "Widget.h"

#define BTN_ACTIVE 1
#define BTN_NOT_ACTIVE 0
#define EVENT_UPDATE_WINDOW 0
#define EVENT_UPDATE_TO_ONE_PLAYER 1
#define EVENT_UPDATE_TO_TWO_PLAYERS 2
#define EVENT_SHOW_GAME_MODE_SCREEN 3
#define EVENT_LOAD_WINDOW 4
#define EVENT_START_GAME 5
#define EVENT_MOVE 6
#define EVENT_BEGIN_DRAG 7
#define EVENT_GET_MOVES 8
#define EVENT_DRAGGED_NOT_ON_BOARD 9
#define EVENT_NEXT 10
#define EVENT_DIFFICULTY_NOOB 11
#define EVENT_DIFFICULTY_EASY 12
#define EVENT_DIFFICULTY_MODERATE 13
#define EVENT_DIFFICULTY_HARD 14
#define EVENT_SET_COLOR_WHITE 15
#define EVENT_SET_COLOR_BLACK 16
#define EVENT_UNDO 17
#define EVENT_BACK 18
#define EVENT_GO_TO_MAIN_MENU 19
#define EVENT_RESTART 20
#define EVENT_SET_SLOT_1 21
#define EVENT_SET_SLOT_2 22
#define EVENT_SET_SLOT_3 23
#define EVENT_SET_SLOT_4 24
#define EVENT_SET_SLOT_5 25
#define EVENT_SAVE 26

typedef enum _btnType {
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
	CH_BTN_START,
	CH_BTN_NOOB,
	CH_BTN_EASY,
	CH_BTN_MODERATE,
	CH_BTN_HARD,
	CH_BTN_WHITE_COLOR,
	CH_BTN_BLACK_COLOR,
	CH_BTN_GAME_SLOT1,
	CH_BTN_GAME_SLOT2,
	CH_BTN_GAME_SLOT3,
	CH_BTN_GAME_SLOT4,
	CH_BTN_GAME_SLOT5
} BtnType;


typedef struct simplebutton_t {
	SDL_Texture* buttonTexture;
	int isSecondTextureValid;
	SDL_Texture* secondTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	SDL_Texture* glowTexture;
	BtnType type;
	int isPressed;
	int isActive;
} SimpleButton;


/** creates a simple button widget and returns the pointer to it
 * @param windowRender - the renderer of the window the button is created on.
 * @param location - the location of the button on the window
 * @param image - the image of the button
 * @param image2 - the second image of the button or NULL if there isn't one.
 * @param type - the type of the button
 * @param isPressed - is the button showed as pressed when created
 * @param isActive - is the button clickable
 * @return
 * the pointer to the button widget or NULL if there was an error in creating the button
 */
Widget* createSimpleButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image, const char* image2, BtnType type, int isPressed, int isActive);

/** checks if the point is inside the button (takes in to account the surrounding transparent pixels that are not the button itself)
 * @param p - a point to be checked
 * @param r - the locating and sizes of the button
 */
int PointInRect(const SDL_Point *p, const SDL_Rect *r);

/** destroy all data Associate with a button:
 * @param src - pointer to a button widget
 */
void destroySimpleButton(Widget* src);

/** initilaze the user event code to one of the slot events codes
 * @param src - pointer to a button
 * @param event - pointer to an event that is a click on one of the slot buttons
 */
void initilazeUserSlotEvent(SDL_Event *user_event, SimpleButton* btn);

/** initilaze the user event code to one of the difficulty events codes
 * @param src - pointer to a button
 * @param event - pointer to an event that is a click on one of the difficulty buttons
 */
void initilazeUserDifficultyEvent(SDL_Event *user_event, SimpleButton* btn);

/** initilaze the user event code
 * @param src - pointer to a button
 * @param event - pointer to an event that is a click on one of the buttons
 */
void initilazeUserEvent(SDL_Event *user_event,SimpleButton* btn);

/** handle button events
 * @param src - pointer to a button widget
 * @param event - pointer to an event
 */
void handleSimpleButtonEvent(Widget* src, SDL_Event* event);

/** draw the button
 * @param src - pointer to a button widget
 */
void drawSimpleButton(Widget* src);

/** draw the button glow
 * @param src - pointer to a button widget
 */
void drawSimpleGlow(Widget* src);

/** change the button texture that beeing presented to the second texture
 * and sets the button isActive argument
 * @param src - pointer to a button widget
 * @param active - is the button will be active or not
 */
void updateTextureBtn(Widget* src, int active);

#endif
