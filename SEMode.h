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
#include <SDL.h>
#include <SDL_video.h>
#include "CHGame.h"
#include "SEParser.h"
#include "SPBufferset.h"
#include "CHStartWindow.h"
#include "CHModeWindow.h"
#include "SPSimpleMainWindow.h"
#include "SimpleButton.h"


#define MAX_LINE_SIZE 1024
CHGame* startSettingsMode();

#endif /* SEMODE_H_ */
