/*
 * CHMainAux.h
 *
 *  Created on: Sep 27, 2017
 *      Author: sasha
 */

#ifndef CHMAINAUX_H_
#define CHMAINAUX_H_

#include "SEMode.h"
#include "CHGame.h"
#include "CHParser.h"
#include "graphics/gameBoard.h"
#include "CHMiniMax.h"
#include "saveAndLoad.h"


void setGuiMode(int argc, char** argv, bool *isGuiMode);

void printTurn(CHGame *src);

void handleMainEvents(CHGame** game, SPWindow** window, SDL_Event *event,
		CHCommand *command, bool *isSaved, int *draw);

void computerTurn(CHGame* game, SPWindow* window, CHMoveNode *best_move,
		bool *isSaved, bool *isTurnChanged,bool isGuiMode);

void handleAllCommands(CHCommand command, CHGame **game, SPWindow** window, bool *isSaved,bool *isTurnChanged, CHMoveNode *best_move, SDL_Event *event, bool isGuiMode);
#endif /* CHMAINAUX_H_ */
