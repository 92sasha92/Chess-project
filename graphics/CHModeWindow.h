/*
 * CHModeWindow.h
 *
 *  Created on: Sep 11, 2017
 *      Author: sasha
 */

#ifndef CHMODEWINDOW_H_
#define CHMODEWINDOW_H_


/** changes the buttons argument isPressed so it will show only the current pressed button
 * @param src -  a pointer to the SPWindow of type CH_WINDOW_MODE
 * @param mode - the game mode button that been pressed
 */
void pressModeChange(SPWindow* src,int mode);


/** creates all the widgets of the Mode window
 * @param renderer - the Mode window renderer
 * @return a pointer to an array of all the created widgets pointers or NULL if there was an error
 */
Widget** createSimpleModeWindowWidgets(SDL_Renderer* renderer);

/** creates a Mode window and returns the pointer to it
 * @return
 * the pointer to the SPWindow of type CH_WINDOW_MODE or NULL if there was an error in creating the window
 */
SPWindow* createSimpleModeWindow();

#endif /* CHMODEWINDOW_H_ */
