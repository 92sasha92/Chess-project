/*
 * SettingsMode.c
 *
 *  Created on: Aug 26, 2017
 *      Author: sasha
 */
#include "SEMode.h"
#include "saveAndLoad.h"

int gameMode = 1;
int gameDifficulty = 2;
int userColor = 1;
int currentTurn = 1;


void seMessageTerminate(){
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ERROR",
			"A problem occurred, the program is terminated", NULL );
	SDL_Quit();
	exit(0);
}

void seSetGameMode(int modeNum){
	if (modeNum != 1 &&  modeNum != 2){
		printf("Wrong game mode\n");
	}
	else{
		gameMode = modeNum;
		printf("Game mode is set to ");
		if(gameMode == 1){
			printf("1 player\n");
		}
		else{
			printf("2 players\n");
		}
	}
}

void seSetDifficulty(int difficultyNum){
	if(difficultyNum <= 0 || 5 < difficultyNum){
		printf("Wrong difficulty level. The value should be between 1 to 5\n");
	}
	else if(difficultyNum == 5){
		printf("Expert level not supported, please choose a value between 1 to 4:\n");
	}
	else{
		gameDifficulty = difficultyNum;
	}
}

void seSetDefault(){
	gameMode = 1;
	gameDifficulty = 2;
	userColor = 1;
	currentTurn = 1;
}

void seSetUserColor(int userColorNum){
	if(userColorNum == 0 || userColorNum == 1){
		userColor = userColorNum;
	}
}

void sePrintSettings(){
	if(gameMode == 1){
		printf("SETTINGS:\n");
		printf("GAME_MODE: 1\n");
		printf("DIFFICULTY_LVL: %d\n",gameDifficulty);
		if(userColor == 0)
			printf("USER_CLR: BLACK\n");
		else
			printf("USER_CLR: WHITE\n");

	}else{
		printf("SETTINGS:\n");
		printf("GAME_MODE: 2\n");
	}
}


void seSetSlotEventHandle(SDL_Event event,SPWindow* window){
	SPSimpleWindow *simpleWindow =(SPSimpleWindow *)window->data;
	if(event.user.code == EVENT_SET_SLOT_1){
		pressSlotChange(window,1);
		if(!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)){
			updateTextureBtn(simpleWindow->widgets[1],1);
		}
	}else if(event.user.code == EVENT_SET_SLOT_2){
		pressSlotChange(window,2);
		if(!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)){
			updateTextureBtn(simpleWindow->widgets[1],1);
		}
	}else if(event.user.code == EVENT_SET_SLOT_3){
		pressSlotChange(window,3);
		if(!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)){
			updateTextureBtn(simpleWindow->widgets[1],1);
		}
	}else if(event.user.code == EVENT_SET_SLOT_4){
		pressSlotChange(window,4);
		if(!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)){
			updateTextureBtn(simpleWindow->widgets[1],1);
		}
	}else if(event.user.code == EVENT_SET_SLOT_5){
		pressSlotChange(window,5);
		if(!(((SimpleButton*) simpleWindow->widgets[1]->data)->isActive)){
			updateTextureBtn(simpleWindow->widgets[1],1);
		}
	}
}

void seBackEventHandle(SPWindow** window,SDL_Event *event){
	SPSimpleWindow *simpleWindow =(SPSimpleWindow *)(*window)->data;
	if(simpleWindow->type == CH_WINDOW_MODE || simpleWindow->type == CH_WINDOW_LOAD){
		destroyWindow(*window);
		*window = createSimpleStartWindow();
		if (*window == NULL ) {
			printf("ERROR: unable to create start window: %s\n", SDL_GetError());
			seMessageTerminate();
		}
	}else if(simpleWindow->type == CH_WINDOW_DIFFICULTY){
		destroyWindow(*window);
		*window = createSimpleModeWindow();
		if (*window == NULL ) {
			printf("ERROR: unable to create difficulty window: %s\n", SDL_GetError());
			seMessageTerminate();
		}
	}else if(simpleWindow->type == CH_WINDOW_COLOR){
		destroyWindow(*window);
		*window = createSimpleDifficultyWindow();
		if (*window == NULL ) {
			printf("ERROR: unable to create difficulty window: %s\n", SDL_GetError());
			seMessageTerminate();
		}
		pressDifficultyChange(*window,gameDifficulty);
	}
	SDL_WaitEvent(event);
}

void seSetDifficultyEventHandle(SPWindow** window,SDL_Event event){
	if(event.user.code == EVENT_DIFFICULTY_NOOB){
		seSetDifficulty(1);
		pressDifficultyChange(*window,1);
	}else if(event.user.code == EVENT_DIFFICULTY_EASY){
		seSetDifficulty(2);
		pressDifficultyChange(*window,2);
	}else if(event.user.code == EVENT_DIFFICULTY_MODERATE){
		seSetDifficulty(3);
		pressDifficultyChange(*window,3);
	}else if(event.user.code == EVENT_DIFFICULTY_HARD){
		seSetDifficulty(4);
		pressDifficultyChange(*window,4);
	}
}

void seNextEventHandle(SPWindow** window,SDL_Event *event){
	SPSimpleWindow *simpleWindow =(SPSimpleWindow *)(*window)->data;
	if(simpleWindow->type == CH_WINDOW_MODE){
		destroyWindow(*window);
		*window = createSimpleDifficultyWindow();
		if (*window == NULL ) {
			printf("ERROR: unable to create difficulty window: %s\n", SDL_GetError());
			seMessageTerminate();
		}
		pressDifficultyChange(*window,gameDifficulty);
		SDL_WaitEvent(event);
	}else if(simpleWindow->type == CH_WINDOW_DIFFICULTY){
		destroyWindow(*window);
		*window = createSimpleColorWindow();
		if (*window == NULL ) {
			printf("ERROR: unable to create choose color window: %s\n", SDL_GetError());
			seMessageTerminate();
		}
		pressColorChange(*window,userColor);
		SDL_WaitEvent(event);
	}
}

void seSetColorEventHandle(SPWindow** window,SDL_Event event){
    if(event.user.code == EVENT_SET_COLOR_WHITE){
    	seSetUserColor(1);
		pressColorChange(*window,1);
    }else if(event.user.code == EVENT_SET_COLOR_BLACK){
    	seSetUserColor(0);
		pressColorChange(*window,0);
	}
}

void seShowModeWindowEventHandle(SPWindow** window,SDL_Event *event){
	destroyWindow(*window);
	*window = createSimpleModeWindow();
	if (*window == NULL ) {
		printf("ERROR: unable to create mode window: %s\n", SDL_GetError());
		seMessageTerminate();
	}
	pressModeChange(*window,gameMode);
	SDL_WaitEvent(event);
}

CH_GAME_MESSAGE seLoadWindowEventHandle(SPWindow** window,SDL_Event *event, CHGame** src){
	SPSimpleWindow *simpleWindow =(SPSimpleWindow *)(*window)->data;
	int slot;
	CH_GAME_MESSAGE mes;
	if(simpleWindow->type == CH_WINDOW_START){
		destroyWindow(*window);
		*window = createLoadWindow();
		if(*window == NULL){
			printf("ERROR: unable to create load window: %s\n", SDL_GetError());
			seMessageTerminate();
		}
		SDL_WaitEvent(event);
	}else if(simpleWindow->type == CH_WINDOW_LOAD){
		slot = getSlotPressed(*window);
		if(slot > 0){
			*src = (CHGame*) malloc(sizeof(CHGame)); /* allocate place in memory */
			mes = chGuiLoad(*src,slot);
			if(mes == CH_GAME_FILE_PROBLEM || mes == CH_GAME_INVALID_ARGUMENT){
				free(*src);
				*src = NULL;
			}else if(mes == CH_GAME_MEMORY_PROBLEM){
				*src = NULL;
			}else{
				destroyWindow(*window);
				return CH_GAME_LOAD_SUCCESS;
			}
			return mes;
		}
	}
	return CH_GAME_SUCCESS;
}

CHGame *seGuiActiveHandle(){
	SPWindow* window;
	CHGame* src;
	SDL_Event event;
	bool drawWindow = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return NULL;
	}
	window = createSimpleStartWindow();
	if (window == NULL ) {
		printf("ERROR: unable to create start window: %s\n", SDL_GetError());
		seMessageTerminate();
		return NULL;
	}
	while (1) {
		SDL_WaitEvent(&event);
		if(event.type == SDL_QUIT){
			destroyWindow(window);
			SDL_Quit();
			return NULL;
		}else if(event.type == SDL_USEREVENT){
			if(event.user.code == EVENT_SHOW_GAME_MODE_SCREEN){
				seShowModeWindowEventHandle(&window, &event);
			}else if(event.user.code == EVENT_LOAD_WINDOW){
				if(seLoadWindowEventHandle(&window, &event, &src) == CH_GAME_LOAD_SUCCESS)
					return src;
			}else if(event.user.code == EVENT_UPDATE_TO_ONE_PLAYER){
				seSetGameMode(1);
			}else if(event.user.code == EVENT_UPDATE_TO_TWO_PLAYERS){
				seSetGameMode(2);
			}else if(event.user.code == EVENT_START_GAME){
				destroyWindow(window);
				break;
			}else if(event.user.code == EVENT_NEXT){
				seNextEventHandle(&window, &event);
			}else if(event.user.code == EVENT_BACK){
				seBackEventHandle(&window,&event);
			}else if(event.user.code >= EVENT_DIFFICULTY_NOOB && event.user.code <= EVENT_DIFFICULTY_HARD){
				seSetDifficultyEventHandle(&window, event);
			}else if(event.user.code >= EVENT_SET_COLOR_WHITE && event.user.code <= EVENT_SET_COLOR_BLACK){
				seSetColorEventHandle(&window, event);
			}else if(event.user.code >= EVENT_SET_SLOT_1 && event.user.code <= EVENT_SET_SLOT_5){
				seSetSlotEventHandle(event,window);
			}
			drawWindow = true;
		}
		window->handleEventWindow(window,&event);
		if(drawWindow == true){
			window->drawWindow(window);
			drawWindow = false;
		}
	}
	src = chGameCreate(gameMode,userColor,gameDifficulty,currentTurn);
	return src;
}

CHGame* seloadCommandHandle(bool *isLoaded, SECommand command){
	CHGame* src;
	if(command.validArg){
		src = (CHGame*) malloc(sizeof(CHGame)); /* allocate place in memory */
		if (!src){
			printf("ERROR: malloc has failed\n");
			return NULL;
		}
		if(load(command.path,src,&currentTurn,&gameMode,&gameDifficulty,&userColor)!= CH_GAME_SUCCESS){
			seSetDefault();
			free(src);
			return NULL;
		}
		else{
			*isLoaded = true;
			return src;
		}
	}
	else
		printf("ERROR: invalid argument\n");
	return NULL;
}

CHGame* startSettingsMode(bool isGuiMode) {
	char strCommand[MAX_LINE_SIZE];
	SECommand command;
	CHGame* src = NULL;
	bool isLoaded = 0;
	printf(
			"Specify game setting or type 'start' to begin a game with the current setting:\n");
	if (isGuiMode) {
		return seGuiActiveHandle();
	} else {
		while (true) {
			fgets(strCommand, MAX_LINE_SIZE, stdin);
			command = seParserPraseLine(strCommand);
			if(command.cmd == SE_GAME_MODE) {
				if(command.validArg)
					seSetGameMode(command.arg);
				else
				   printf("ERROR: invalid argument\n");
			} else if(command.cmd == SE_DIFFICULTY) {
				if(gameMode == 1)
					seSetDifficulty(command.arg);
				else
					printf("ERROR: invalid Command\n");
			} else if(command.cmd == SE_USER_COLOR){
				if(gameMode == 1){
					if(command.validArg)
						seSetUserColor(command.arg);
				}
				else
					printf("ERROR: invalid Command\n");
			} else if(command.cmd == SE_LOAD){
				src = seloadCommandHandle(&isLoaded, command);
			} else if(command.cmd == SE_DEFAULT){
				seSetDefault();
			} else if(command.cmd == SE_PRINT_SETTINGS){
				sePrintSettings();
			} else if(command.cmd == SE_QUIT){
				printf("Exiting...\n");
				chGameDestroy(src);
				return NULL;
			} else if(command.cmd == SE_START){
				if(isLoaded)
					return src;
				else
					break;
			} else{
				printf("ERROR: invalid Command\n");
			}
		}
	}
	src = chGameCreate(gameMode, userColor, gameDifficulty, currentTurn);
	return src;
}





