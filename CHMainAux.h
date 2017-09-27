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
#include "gameBoard.h"
#include "CHMiniMax.h"
#include "save&load.h"

void printTurn(CHGame *src);

void handleMainEvents(CHGame** game, SPWindow** window, SDL_Event *event,
		CHCommand *command, bool *isSaved, int *draw);

void computerTurn(CHGame* game, SPWindow* window, CHMoveNode *best_move,
		bool *isSaved, bool *isTurnChanged);

void handleAllCommands(CHCommand command, CHGame **game, SPWindow** window, bool *isSaved,bool *isTurnChanged, CHMoveNode *best_move, SDL_Event *event);
#endif /* CHMAINAUX_H_ */
