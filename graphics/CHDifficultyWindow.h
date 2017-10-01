/*
 * CHDifficultyWindow.h
 *
 *  Created on: Sep 22, 2017
 *      Author: sasha
 */

#ifndef CHDIFFICULTYWINDOW_H_
#define CHDIFFICULTYWINDOW_H_


/**creates all the widgets of the Difficulty window
 * @param renderer - the Difficulty window renderer
 * @return a pointer to an array of all the created widgets pointers or NULL if there was an error
 */
Widget** createSimpleDifficultyWindowWidgets(SDL_Renderer* renderer);

/** creates a Difficulty window and returns the pointer to it
 * @return
 * the pointer to the SPWindow of type CH_WINDOW_DIFFICULTY or NULL if there was an error in creating the window
 */
SPWindow* createSimpleDifficultyWindow();


/**sets the pressed button (according to the difficulty) argument isPressed to 1 and the other buttons to 0
 * @param src - a pointer to the SPWindow
 * @param difficulty - the current difficulty
 */
void pressDifficultyChange(SPWindow* src, int difficulty);

#endif /* CHDIFFICULTYWINDOW_H_ */
