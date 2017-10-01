/*
 * CHLoadWindow.h
 *
 *  Created on: Sep 23, 2017
 *      Author: sasha
 */

#ifndef CHLOADWINDOW_H_
#define CHLOADWINDOW_H_


/**
 * @param src - a pointer to the SPWindow of type CH_WINDOW_LOAD
 * @return the number of the slot that is pressed
 */
int getSlotPressed(SPWindow* src);

/** changes the buttons argument isPressed so it will show only the current pressed button
 * @param src -  a pointer to the SPWindow of type CH_WINDOW_LOAD
 * @param slot - the game slot that been pressed
 */
void pressSlotChange(SPWindow* src,int slot);

/**creates all the widgets of the Load window
 * @param renderer - the Load window renderer
 * @return a pointer to an array of all the created widgets pointers or NULL if there was an error
 */
Widget** createLoadWindowWidgets(SDL_Renderer* renderer,int numOfWidgets);

/** creates a Load window and returns the pointer to it
 * @return
 * the pointer to the SPWindow of type CH_WINDOW_LOAD or NULL if there was an error in creating the window
 */
SPWindow* createLoadWindow();

#endif /* CHLOADWINDOW_H_ */
