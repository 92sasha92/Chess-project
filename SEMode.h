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
CHGame* startSettingsMode(bool isGuiMode);

#endif /* SEMODE_H_ */
