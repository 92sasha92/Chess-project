#include "CHMainAux.h"

int main(int argc, char** argv) {
	SP_BUFF_SET();
	bool isGuiMode = false, isSaved = true, isTurnChanged = true;;
	setGuiMode(argc, argv, &isGuiMode);
	int draw = 1;
	char strCommand[MAX_LINE_SIZE];
	CHMoveNode *best_move = (CHMoveNode *) malloc(sizeof(CHMoveNode));
	if (!best_move)
		return 0;
	CHCommand command;
	SPWindow* window;
	SDL_Event event;
	CHGame *game = startSettingsMode(isGuiMode);
	if (!game){
		free(best_move);
		return 0;
	}
	if (isGuiMode) {
		window = createSimpleWindow(game);
		if (window == NULL ) {
			free(best_move);
			chGameDestroy(game);
			SDL_Quit();
			return 0;
		}
	}
	while (true) {
		command.cmd = CH_INVALID_LINE;
		if (game->gameMode == 2 || (game->gameMode == 1 && game->currentTurn == game->userColor)) {
			if (isTurnChanged) {
				chGamePrintBoard(game);
				if (isGuiMode)
					printTurn(game);
			}
			if (!isGuiMode)
				printTurn(game);
			if (isGuiMode) {
				SDL_WaitEvent(&event);
				handleMainEvents(&game, &window, &event, &command, &isSaved,
						&draw, best_move);
			} else {
				fgets(strCommand, MAX_LINE_SIZE, stdin);
				command = chParserPraseLine(strCommand);
			}
			isTurnChanged = false;
			handleAllCommands(command, &game, &window, &isSaved, &isTurnChanged,
					best_move, &event, isGuiMode);
		} else {
			computerTurn(game, window, best_move, &isSaved, &isTurnChanged, isGuiMode);
		}
		if (isGuiMode) {
			window->handleEventWindow(window, &event);
			if (draw == 1) {
				window->drawWindow(window);
				draw = 0;
			}
		}
	}
	return 0;
}
