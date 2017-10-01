/*
 * CHColorWindow.h
 *
 *  Created on: Sep 22, 2017
 *      Author: sasha
 */

#ifndef CHCOLORWINDOW_H_
#define CHCOLORWINDOW_H_


/**creates all the widgets of the color window
 * @param renderer - the color window renderer
 * @return a pointer to an array of all the created widgets pointers or NULL if there was an error
 */
Widget** createSimpleColorWindowWidgets(SDL_Renderer* renderer);

/** creates a color window and returns the pointer to it
 * @return
 * the pointer to the SPWindow of type CH_WINDOW_COLOR or NULL if there was an error in creating the window
 */
SPWindow* createSimpleColorWindow();

/**sets the pressed button argument isPressed to 1 and the other color button to 0
 * @param src - a pointer to the SPWindow
 * @param color - the button to set the argument to
 */
void pressColorChange(SPWindow* src, int color);

#endif /* CHCOLORWINDOW_H_ */
