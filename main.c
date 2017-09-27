/*
 * main.c
 *
 *  Created on: Aug 30, 2017
 *      Author: sasha
 */

#include "CHMainAux.h"
#define REC_DEPTH 4


int main(int argc, char** argv) {
	SP_BUFF_SET();
	int draw = 1;
	char strCommand[MAX_LINE_SIZE];
	CHMoveNode *best_move = (CHMoveNode *) malloc(sizeof(CHMoveNode));
	if (!best_move)
		return 0;
	CHCommand command;
	bool isTurnChanged = true;
	SPWindow* window;
	SDL_Event event;
	bool isSaved = true;
	CHGame *game = startSettingsMode();
	if (!game)
		return 0;
	if (GUI_ACTIVE) {
		window = createSimpleWindow(game);
		if (window == NULL ) {
			chGameDestroy(game);
			SDL_Quit();
			return 0;
		}
	}
	while (true) {
		command.cmd = CH_INVALID_LINE;
		if (game->gameMode == 2
				|| (game->gameMode == 1 && game->currentTurn == game->userColor)) {
			if (isTurnChanged) {
				chGamePrintBoard(game);
				if (GUI_ACTIVE)
					printTurn(game);
			}
			if (!(GUI_ACTIVE))
				printTurn(game);
			if (GUI_ACTIVE) {
				SDL_WaitEvent(&event);
				handleMainEvents(&game, &window, &event, &command, &isSaved,
						&draw);
			} else {
				fgets(strCommand, MAX_LINE_SIZE, stdin);
				command = chParserPraseLine(strCommand);
			}
			isTurnChanged = false;
			handleAllCommands(command, &game, &window, &isSaved, &isTurnChanged,
					best_move, &event);
		} else {
			computerTurn(game, window, best_move, &isSaved, &isTurnChanged);
		}
		if (GUI_ACTIVE) {
			window->handleEventWindow(window, &event);
			if (draw == 1) {
				window->drawWindow(window);
				draw = 0;
			}
		}
	}
	return 0;
}
