/*
 * SettingsMode.c
 *
 *  Created on: Aug 26, 2017
 *      Author: sasha
 */
#include "SEMode.h"
int gameMode = 1;
int gameDifficulty = 2;
int userColor = 1;
int currentTurn = 1;




void setGameMode(int modeNum){
	if (modeNum != 1 &&  modeNum != 2){
		printf("Wrong difficulty level. The value should be between 1 to 5\n");
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


int readBoard(FILE *fp,CHGame *src){
	int i,j;
	char strMark[10];
	char buf[10];
	fscanf(fp,"%7s",buf);
	if(strcmp(buf,"<board>") != 0)
		return 0;
	fscanf(fp, "%*[^\n]");
	for(i = CH_GAME_N_ROWS - 1;i >= 0;i--){
		sprintf(strMark,"<row_%d>",i + 1);
		fscanf(fp,"%7s",buf);
		if(strcmp(buf,strMark) != 0)
			return 0;
		for(j = 0;j < CH_GAME_N_COLUMNS;j++){
			src->gameBoard[i][j] = fgetc(fp);
		}
		fscanf(fp,"%8s",buf);
		sprintf(strMark,"</row_%d>",i + 1);
		if(strcmp(buf,strMark) != 0)
			return 0;
		fscanf(fp, "%*[^\n]");
	}
	fscanf(fp,"%8s",buf);
	if(strcmp(buf,"</board>") != 0)
		return 0;
	fscanf(fp, "%*[^\n]");
	return 1;
}

int exitload(FILE *fp){
	fclose(fp);
	return 0;
}

int load(char *path,CHGame *src){
	char buf[20];
	FILE *fp = fopen(path,"r");
	if(fp == NULL){
		printf("Error: File doesn’t exist or cannot be opened\n");
		return 0;
	}
	fscanf(fp, "%*[^\n]");
	fscanf(fp,"%6s",buf);
	if(strcmp(buf,"<game>") != 0)
		return exitload(fp);
	fscanf(fp, "%*[^\n]");
	fscanf(fp,"%14s",buf);
	if(strcmp(buf,"<current_turn>") != 0)
		return exitload(fp);
	fscanf(fp, "%d",&currentTurn);
	fscanf(fp,"%15s",buf);
	if(strcmp(buf,"</current_turn>") != 0)
		return exitload(fp);
	fscanf(fp, "%*[^\n]");
	fscanf(fp,"%11s",buf);
	if(strcmp(buf,"<game_mode>") != 0)
		return exitload(fp);
	fscanf(fp, "%d",&gameMode);
	fscanf(fp,"%12s",buf);
	if(strcmp(buf,"</game_mode>") != 0)
		return exitload(fp);
	fscanf(fp, "%*[^\n]");
	if(gameMode != 2){
		fscanf(fp,"%12s",buf);
		if(strcmp(buf,"<difficulty>") != 0)
			return exitload(fp);
		fscanf(fp, "%d",&gameDifficulty);
		fscanf(fp,"%13s",buf);
		if(strcmp(buf,"</difficulty>") != 0)
			return exitload(fp);
		fscanf(fp, "%*[^\n]");
		fscanf(fp,"%12s",buf);
		if(strcmp(buf,"<user_color>") != 0)
			return exitload(fp);
		fscanf(fp, "%d",&userColor);
		fscanf(fp,"%13s",buf);
		if(strcmp(buf,"</user_color>") != 0)
			return exitload(fp);
		fscanf(fp, "%*[^\n]");
	}
	if(!readBoard(fp,src))
		return exitload(fp);
	fscanf(fp,"%7s",buf);
	if(strcmp(buf,"</game>") != 0)
		return exitload(fp);
	fclose(fp);
	src->currentTurn = currentTurn;
	src->gameMode = gameMode;
	if(!chGameCreateMode1(src,gameDifficulty,userColor))
		return 0;
	return 1;
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



CHGame* startSettingsMode(){
	char strCommand[MAX_LINE_SIZE];
	SECommand command;
	CHGame* src;
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n" );
	while(true){
		fgets(strCommand,MAX_LINE_SIZE,stdin);
		command = seParserPraseLine(strCommand);
		if(command.cmd == SE_GAME_MODE){
			if(command.validArg)
				setGameMode(command.arg);
			else
				printf("ERROR: invalid argument\n");
		}
		else if(command.cmd == SE_DIFFICULTY){
			if(gameMode == 1){
				if(command.validArg)
					setDifficulty(command.arg);
				else
					printf("Wrong difficulty level. The value should be between 1 to 5\n");
			}
			else{
				printf("ERROR: invalid Command\n");
			}
		}
		else if(command.cmd == SE_USER_COLOR){
			if(gameMode == 1){
				if(command.validArg)
					setUserColor(command.arg);
//				else
//					printf("ERROR: invalid argument\n");
			}
			else{
				printf("ERROR: invalid Command\n");
			}
		}
		else if(command.cmd == SE_LOAD){
			if(command.validArg){
				src = (CHGame*) malloc(sizeof(CHGame)); /* allocate place in memory */
				if (!src){
					printf("Error: malloc has failed\n");
					return NULL;
				}
				if(!load(command.path,src)){
					setDefault();
					free(src);/////////////////////////change to function
				}
				else{
					return src;
				}
			}
			else
				printf("ERROR: invalid argument\n");
		}
		else if(command.cmd == SE_DEFAULT){
			setDefault();
		}
		else if(command.cmd == SE_PRINT_SETTINGS){
			printSettings();
		}
		else if(command.cmd == SE_QUIT){
			printf("Exiting...\n");
			return NULL;
		}
		else if(command.cmd == SE_START){
			break;
		}
		else{
			printf("ERROR: invalid Command\n");
		}
	}
	src = chGameCreate(gameMode,userColor,gameDifficulty,currentTurn);
	return src;
}