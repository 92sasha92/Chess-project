/*
 * CHStartWindow.h
 *
 *  Created on: Sep 10, 2017
 *      Author: sasha
 */

#ifndef CHSTARTWINDOW_H_
#define CHSTARTWINDOW_H_

#include "SPSimpleMainWindow.h"

/**creates all the widgets of the Start window
 * @param renderer - Start window renderer
 * @return a pointer to an array of all the created widgets pointers or NULL if there was an error
 */
Widget** createSimpleStartWindowWidgets(SDL_Renderer* renderer);

/** creates a Start window and returns the pointer to it
 * @return
 * the pointer to the SPWindow of type CH_WINDOW_START or NULL if there was an error in creating the window
 */
SPWindow* createSimpleStartWindow();

#endif /* CHSTARTWINDOW_H_ */
