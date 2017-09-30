/*
 * SEMode.h
 *
 *  Created on: Aug 30, 2017
 *      Author: sasha
 */

#ifndef SEMODE_H_
#define SEMODE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "CHGame.h"
#include "SEParser.h"
#include "SPBufferset.h"
#include "graphics/CHStartWindow.h"
#include "graphics/CHModeWindow.h"
#include "graphics/SPSimpleMainWindow.h"
#include "graphics/SimpleButton.h"
#include "graphics/CHDifficultyWindow.h"
#include "graphics/CHColorWindow.h"
#include "graphics/CHLoadWindow.h"

#define MAX_LINE_SIZE 1024


/** print an error message and terminate the program.
 *
 * @return
*/
void seMessageTerminate();

/** set the game mode. two players or one player against the computer.
 * @param modeNum - the mode number.
 * @return
 */
void seSetGameMode(int modeNum);

/** set the game difficulty. between 1 to 4.
 * @param difficultyNum - the difficulty value.
 * @return
 */
void seSetDifficulty(int difficultyNum);

/** set the game default values. gameMode = 1, gameDifficulty = 2, userColor = 1, currentTurn = 1.
 * @return
 */
void seSetDefault();

/** set the user color. 0 is black and 1 is white.
* @param userColorNum - the user color value.
* @return
*/
void seSetUserColor(int userColorNum);

/** print the current settings of the game
 * @return
 */
void sePrintSettings();

/** event handle of push slot button.
 * @param event - the event struct.
 * @param window - the gme window.
 * @return
 */
void seSetSlotEventHandle(SDL_Event event,SPWindow* window);

/** event handle of push back button.
 * @param event - the event struct.
 * @param window - the gme window.
 * @return
 */
void seBackEventHandle(SPWindow** window,SDL_Event *event);

/** event handle of push difficulty button.
 * @param event - the event struct.
 * @param window - the gme window.
 * @return
 */
void seSetDifficultyEventHandle(SPWindow** window,SDL_Event event);

/** event handle of push next button.
 * @param event - the event struct.
 * @param window - the gme window.
 * @return
 */
void seNextEventHandle(SPWindow** window,SDL_Event *event);

/** event handle of push color button.
 * @param event - the event struct.
 * @param window - the gme window.
 * @return
 */
void seSetColorEventHandle(SPWindow **window, SDL_Event event);

/** shows the mode window.
 * @param event - the event struct.
 * @param window - the gme window.
 * @return
 */
void seShowModeWindowEventHandle(SPWindow** window,SDL_Event *event);

/** load game window handle when push load button.
 * @param event - the event struct.
 * @param window - the gme window.
 * @param src - the game struct.
 * @return
 */
CH_GAME_MESSAGE seLoadWindowEventHandle(SPWindow** window,SDL_Event *event, CHGame** src) ;

/** gui event handler, if an event occurred the function handle it.
 * @return
 */
CHGame *seGuiActiveHandle();

/** load game window in console mode.
 * @param isLoaded - flag to check if the game already loaded.
 * @param command - the current command.
 * @return
 */
CHGame* seloadCommandHandle(bool *isLoaded, SECommand command);

/** read from the user all the settings in order to start the game correctly
 * @param isGuiMode - flag that set to true if gui mode in use.
 * @return
 * an new game instant if there is no error when initializing the game.
 */
CHGame* startSettingsMode(bool isGuiMode);

#endif /* SEMODE_H_ */
