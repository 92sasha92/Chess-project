#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "CHGame.h"


CH_GAME_MESSAGE readBoard(FILE *fp,CHGame *src) {
	int i, j;
	char strMark[10];
	char buf[10];
	fscanf(fp,"%7s",buf);
	if (strcmp(buf,"<board>") != 0)
		return CH_GAME_INVALID_ARGUMENT;
	fscanf(fp, "%*[^\n]");
	for (i = CH_GAME_N_ROWS - 1; i >= 0; i--){
		sprintf(strMark,"<row_%d>",i + 1);
		fscanf(fp,"%7s",buf);
		if (strcmp(buf,strMark) != 0)
			return CH_GAME_INVALID_ARGUMENT;
		for (j = 0; j < CH_GAME_N_COLUMNS; j++){
			src->gameBoard[i][j] = fgetc(fp);
		}
		fscanf(fp,"%8s",buf);
		sprintf(strMark,"</row_%d>",i + 1);
		if (strcmp(buf,strMark) != 0)
			return CH_GAME_INVALID_ARGUMENT;
		fscanf(fp, "%*[^\n]");
	}
	fscanf(fp,"%8s",buf);
	if (strcmp(buf,"</board>") != 0)
		return CH_GAME_INVALID_ARGUMENT;
	fscanf(fp, "%*[^\n]");
	return CH_GAME_SUCCESS;
}


CH_GAME_MESSAGE exitload(FILE *fp){
	fclose(fp);
	return CH_GAME_INVALID_ARGUMENT;
}


CH_GAME_MESSAGE load(char *path, CHGame *src, int *currentTurn, int *gameMode, int *gameDifficulty, int *userColor) {
	char buf[20];
	FILE *fp = fopen(path,"r");
	if (fp == NULL) {
		printf("Error: File doesn't exist or cannot be opened\n");
		return CH_GAME_FILE_PROBLEM;
	}
	fscanf(fp, "%*[^\n]");
	fscanf(fp,"%6s",buf);
	if (strcmp(buf,"<game>") != 0)
		return exitload(fp);
	fscanf(fp, "%*[^\n]");
	fscanf(fp,"%14s",buf);
	if (strcmp(buf,"<current_turn>") != 0)
		return exitload(fp);
	fscanf(fp, "%d",currentTurn);
    fscanf(fp,"%15s",buf);
	if (strcmp(buf,"</current_turn>") != 0)
		return exitload(fp);
	fscanf(fp, "%*[^\n]");
	fscanf(fp,"%11s",buf);
	if(strcmp(buf,"<game_mode>") != 0)
		return exitload(fp);
	fscanf(fp, "%d",gameMode);
    fscanf(fp,"%12s",buf);
	if (strcmp(buf,"</game_mode>") != 0)
		return exitload(fp);
	fscanf(fp, "%*[^\n]");
	if (*gameMode != 2) {
		fscanf(fp,"%12s",buf);
		if (strcmp(buf,"<difficulty>") != 0)
			return exitload(fp);
		fscanf(fp, "%d",gameDifficulty);
	    fscanf(fp,"%13s",buf);
		if (strcmp(buf,"</difficulty>") != 0)
			return exitload(fp);
		fscanf(fp, "%*[^\n]");
		fscanf(fp,"%12s",buf);
		if (strcmp(buf,"<user_color>") != 0)
			return exitload(fp);
		fscanf(fp, "%d",userColor);
	    fscanf(fp,"%13s",buf);
		if (strcmp(buf,"</user_color>") != 0)
			return exitload(fp);
		fscanf(fp, "%*[^\n]");
	}
	if (!readBoard(fp,src))
		return exitload(fp);
	fscanf(fp,"%7s",buf);
	if (strcmp(buf,"</game>") != 0)
		return exitload(fp);
	fclose(fp);
	src->currentTurn = *currentTurn;
	src->gameMode = *gameMode;
	if (!chGameCreateMode1(src,*gameDifficulty,*userColor))
		return 0;
	return CH_GAME_SUCCESS;
}


CH_GAME_MESSAGE chGameSave(CHGame* src,char *path) {
	int i, j;
	FILE *fp;
	if (src == NULL)
		return CH_GAME_INVALID_ARGUMENT;
	fp = fopen(path,"w");
	if (!fp)
		return CH_GAME_FILE_PROBLEM;
	fprintf(fp,"%s","<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fp,"%s","<game>\n");
	fprintf(fp,"%s%d%s","\t<current_turn>",src->currentTurn,"</current_turn>\n");
	fprintf(fp,"%s%d%s","\t<game_mode>",src->gameMode,"</game_mode>\n");
	if (src->gameMode == 1) {
		fprintf(fp,"%s%d%s","\t<difficulty>",src->difficulty,"</difficulty>\n");
		fprintf(fp,"%s%d%s","\t<user_color>",src->userColor,"</user_color>\n");
	}
	fprintf(fp,"%s","\t<board>\n");
	for (i = CH_GAME_N_ROWS - 1; i >=0; i--) {
		fprintf(fp,"%s%d%s","\t\t<row_",i + 1,">");
		for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
			fprintf(fp,"%c",src->gameBoard[i][j]);
		}
		fprintf(fp,"%s%d%s\n","</row_",i + 1,">");
	}
	fprintf(fp,"%s","\t</board>\n");
	fprintf(fp,"%s","</game>\n");
	fclose(fp);
	return CH_GAME_SUCCESS;
}


char *slotPath(int slotNum) {
	switch(slotNum){
	case 1:
		return "./savedGames/gameSlot1.xml";
	case 2:
		return "./savedGames/gameSlot2.xml";
	case 3:
		return "./savedGames/gameSlot3.xml";
	case 4:
		return "./savedGames/gameSlot4.xml";
	case 5:
		return "./savedGames/gameSlot5.xml";
	}
	return "";
}


CH_GAME_MESSAGE chGuiSave(CHGame* src) {
	int i;
	FILE *fp;
	CH_GAME_MESSAGE mes;
	int numOfSaves = 0;
	int currentTurn = 1;
	int gameMode = 2;
	int gameDifficulty = 2;
	int userColor = 1;
	if(src == NULL)
		return CH_GAME_INVALID_ARGUMENT;
	CHGame* tmp = NULL;
	fp = fopen("./savedGames/readMeForLoad.txt","r");
	if (!fp)
		return CH_GAME_FILE_PROBLEM;
	if (fscanf(fp,"%d",&numOfSaves) != 1) {
		fclose(fp);
		return CH_GAME_FILE_PROBLEM;
	}
	fclose(fp);
	if (numOfSaves == 5) {
		i = numOfSaves;
	} else {
		i = numOfSaves + 1;
		numOfSaves++;
	}
	for (; i > 1; i--) {
		tmp = chGameCreate(gameMode,userColor,gameDifficulty,currentTurn);
		mes = load(slotPath(i - 1),tmp,&currentTurn,&gameMode,&gameDifficulty,&userColor);
		if (mes != CH_GAME_SUCCESS) {
			chGameDestroy(tmp);
			return mes;
		}
		mes = chGameSave(tmp,slotPath(i));
		if (mes != CH_GAME_SUCCESS) {
			chGameDestroy(tmp);
			return mes;
		}
		chGameDestroy(tmp);
	}
	chGameSave(src,slotPath(i));
	fp = fopen("./savedGames/readMeForLoad.txt","w");
	if (!fp){
		return CH_GAME_FILE_PROBLEM;
	}
	fprintf(fp,"%d",numOfSaves);
	fclose(fp);
	return CH_GAME_SUCCESS;
}

CH_GAME_MESSAGE chGuiLoad(CHGame* src,int slot){
	int erCheck = 0;
	if (!src){
		printf("Error: malloc has failed\n");
		return CH_GAME_MEMORY_PROBLEM;
	}
	int currentTurn = 1;
	int gameMode = 2;
	int gameDifficulty = 2;
	int userColor = 1;
	switch (slot) {
	case 1:
		erCheck = load("./savedGames/gameSlot1.xml",src,&currentTurn,&gameMode,&gameDifficulty,&userColor);
		break;
	case 2:
		erCheck = load("./savedGames/gameSlot2.xml",src,&currentTurn,&gameMode,&gameDifficulty,&userColor);
		break;
	case 3:
		erCheck = load("./savedGames/gameSlot3.xml",src,&currentTurn,&gameMode,&gameDifficulty,&userColor);
		break;
	case 4:
		erCheck = load("./savedGames/gameSlot4.xml",src,&currentTurn,&gameMode,&gameDifficulty,&userColor);
		break;
	case 5:
		erCheck = load("./savedGames/gameSlot5.xml",src,&currentTurn,&gameMode,&gameDifficulty,&userColor);
		break;
	}
	if(erCheck == -1){
		return CH_GAME_FILE_PROBLEM;
	}else{
		return CH_GAME_SUCCESS;
	}
}


