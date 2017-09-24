/*
 * save&load.c
 *
 *  Created on: Sep 23, 2017
 *      Author: sasha
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "CHGame.h"

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

int load(char *path,CHGame *src,int *currentTurn,int *gameMode,int *gameDifficulty,int *userColor){
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
	fscanf(fp, "%d",currentTurn);
    fscanf(fp,"%15s",buf);
	if(strcmp(buf,"</current_turn>") != 0)
		return exitload(fp);
	fscanf(fp, "%*[^\n]");
	fscanf(fp,"%11s",buf);
	if(strcmp(buf,"<game_mode>") != 0)
		return exitload(fp);
	fscanf(fp, "%d",gameMode);
    fscanf(fp,"%12s",buf);
	if(strcmp(buf,"</game_mode>") != 0)
		return exitload(fp);
	fscanf(fp, "%*[^\n]");
	if(*gameMode != 2){
		fscanf(fp,"%12s",buf);
		if(strcmp(buf,"<difficulty>") != 0)
			return exitload(fp);
		fscanf(fp, "%d",gameDifficulty);
	    fscanf(fp,"%13s",buf);
		if(strcmp(buf,"</difficulty>") != 0)
			return exitload(fp);
		fscanf(fp, "%*[^\n]");
		fscanf(fp,"%12s",buf);
		if(strcmp(buf,"<user_color>") != 0)
			return exitload(fp);
		fscanf(fp, "%d",userColor);
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
	src->currentTurn = *currentTurn;
	src->gameMode = *gameMode;
	if(!chGameCreateMode1(src,*gameDifficulty,*userColor))
		return 0;
	return 1;
}

CH_GAME_MESSAGE chGameSave(CHGame* src,char *path){
	int i,j;
	FILE *fp;
	if(src == NULL)
		return CH_GAME_INVALID_ARGUMENT;
	fp = fopen(path,"w");
	if (!fp)
		return CH_GAME_FILE_PROBLEM;
	fprintf(fp,"%s","<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fp,"%s","<game>\n");
	fprintf(fp,"%s%d%s","\t<current_turn>",src->currentTurn,"</current_turn>\n");
	fprintf(fp,"%s%d%s","\t<game_mode>",src->gameMode,"</game_mode>\n");
	if(src->gameMode == 1){
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

