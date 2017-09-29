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




void setGameMode(int modeNum){
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

void setDifficulty(int difficultyNum){
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

void setDefault(){
	gameMode = 1;
	gameDifficulty = 2;
	userColor = 1;
	currentTurn = 1;
}

void setUserColor(int userColorNum){
	if(userColorNum == 0 || userColorNum == 1){
		userColor = userColorNum;
	}
}

void printSettings(){
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

CHGame *seGuiActiveHandle(){
	SPWindow* window;
	CHGame* src;
	int slot;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return NULL;
	}
	window = createSimpleStartWindow();
	if (window == NULL ) {
		SDL_Quit();
		return NULL;
	}
	SPSimpleWindow *simpleWindow =(SPSimpleWindow *)window->data;
	SDL_Event event;
	int flag = 1;
	while (1) {
		SDL_WaitEvent(&event);
		if(event.type == SDL_QUIT){
			destroyWindow(window);
			SDL_Quit();
			return NULL;
		}else if(event.type == SDL_USEREVENT){
			if(event.user.code == EVENT_SHOW_GAME_MODE_SCREEN){
				destroyWindow(window);
				window = createSimpleModeWindow();
				simpleWindow =(SPSimpleWindow *)window->data;
				pressModeChange(window,gameMode);
				SDL_WaitEvent(&event);
			}else if(event.user.code == EVENT_LOAD_WINDOW){
				if(simpleWindow->type == CH_WINDOW_START){
					destroyWindow(window);
					window = createLoadWindow();
				}else if(simpleWindow->type == CH_WINDOW_LOAD){
					slot = getSlotPressed(window);
					destroyWindow(window);
					if(slot > 0){
						src = (CHGame*) malloc(sizeof(CHGame)); /* allocate place in memory */
						if(chGuiLoad(src,slot) != CH_GAME_SUCCESS){
							return NULL;
						}else{
							return src;
						}
					}
				}
				simpleWindow =(SPSimpleWindow *)window->data;
				SDL_WaitEvent(&event);
			}else if(event.user.code == EVENT_UPDATE_TO_ONE_PLAYER){
				setGameMode(1);
			}else if(event.user.code == EVENT_UPDATE_TO_TWO_PLAYERS){
				setGameMode(2);
			}else if(event.user.code == EVENT_START_GAME){
				destroyWindow(window);
				break;
			}else if(event.user.code == EVENT_NEXT){
				if(simpleWindow->type == CH_WINDOW_MODE){
					destroyWindow(window);
					window = createSimpleDifficultyWindow();
					pressDifficultyChange(window,gameDifficulty);
					simpleWindow =(SPSimpleWindow *)window->data;
					SDL_WaitEvent(&event);
				}else if(simpleWindow->type == CH_WINDOW_DIFFICULTY){
					destroyWindow(window);
					window = createSimpleColorWindow();
					pressColorChange(window,userColor);
					simpleWindow =(SPSimpleWindow *)window->data;
					SDL_WaitEvent(&event);
				}else if(simpleWindow->type == CH_WINDOW_COLOR){
					destroyWindow(window);
					break;
				}
			}else if(event.user.code == EVENT_BACK){
				if(simpleWindow->type == CH_WINDOW_MODE || simpleWindow->type == CH_WINDOW_LOAD){
					destroyWindow(window);
					window = createSimpleStartWindow();
					simpleWindow =(SPSimpleWindow *)window->data;
					SDL_WaitEvent(&event);
				}else if(simpleWindow->type == CH_WINDOW_DIFFICULTY){
					destroyWindow(window);
					window = createSimpleModeWindow();
					simpleWindow =(SPSimpleWindow *)window->data;
					SDL_WaitEvent(&event);
				}else if(simpleWindow->type == CH_WINDOW_COLOR){
					destroyWindow(window);
					window = createSimpleDifficultyWindow();
					pressDifficultyChange(window,gameDifficulty);
					simpleWindow =(SPSimpleWindow *)window->data;
					SDL_WaitEvent(&event);
				}
			}else if(event.user.code == EVENT_DIFFICULTY_EASY){
				setDifficulty(1);
				pressDifficultyChange(window,1);
			}else if(event.user.code == EVENT_DIFFICULTY_NOOB){
				setDifficulty(2);
				pressDifficultyChange(window,2);
			}else if(event.user.code == EVENT_DIFFICULTY_MODERATE){
				setDifficulty(3);
				pressDifficultyChange(window,3);
			}else if(event.user.code == EVENT_DIFFICULTY_HARD){
				setDifficulty(4);
				pressDifficultyChange(window,4);
			}else if(event.user.code == EVENT_SET_COLOR_WHITE){
				setUserColor(1);
				pressColorChange(window,1);
			}else if(event.user.code == EVENT_SET_COLOR_BLACK){
				setUserColor(0);
				pressColorChange(window,0);
			}else if(event.user.code == EVENT_SET_SLOT_1){
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
			flag = 1;
		}
		window->handleEventWindow(window,&event);
		if(flag == 1){
			window->drawWindow(window);
			flag = 0;
		}
	}
	src = chGameCreate(gameMode,userColor,gameDifficulty,currentTurn);
	return src;
}

CHGame* loadCommandHandle(bool *isLoaded, SECommand command){
	CHGame* src;
	if(command.validArg){
		src = (CHGame*) malloc(sizeof(CHGame)); /* allocate place in memory */
		if (!src){
			printf("Error: malloc has failed\n");
			return NULL;
		}
		if(load(command.path,src,&currentTurn,&gameMode,&gameDifficulty,&userColor)!= CH_GAME_SUCCESS){
			setDefault();
			free(src);
			return NULL;
		}
		else{
			*isLoaded = 1;
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
	CHGame* src;
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
				   setGameMode(command.arg);
				else
				   printf("ERROR: invalid argument\n");
			} else if(command.cmd == SE_DIFFICULTY) {
				if(gameMode == 1)
					setDifficulty(command.arg);
				else
					printf("ERROR: invalid Command\n");
			} else if(command.cmd == SE_USER_COLOR){
				if(gameMode == 1){
					if(command.validArg)
						setUserColor(command.arg);
	//				else
	//					printf("ERROR: invalid argument\n");
				}
				else
					printf("ERROR: invalid Command\n");
			} else if(command.cmd == SE_LOAD){
				src = loadCommandHandle(&isLoaded, command);
			} else if(command.cmd == SE_DEFAULT){
				setDefault();
			} else if(command.cmd == SE_PRINT_SETTINGS){
				printSettings();
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





