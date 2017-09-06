/*
 * CHParser.c
 *
 *  Created on: Aug 29, 2017
 *      Author: sasha
 */


#include "CHParser.h"


CHCommand parseMove(char * cur,char delimiter[8]){
	int i;
	char x,y;
	CHCommand command;
	command.cmd = CH_INVALID_LINE;
	command.validArg = false;
	if((cur = strtok(NULL,delimiter)) == NULL){
		return command;
	}
	for(i = 0;i < 2;i++){
		if(cur[0]=='<'){
			cur++;
			x = cur[0];
			cur++;
			if(x > '0' && x < '9' ){
				if(cur[0] != ',')
					return command;
				cur++;
				y = cur[0];
				cur++;
				if(y >= 'A' && y <= 'H'){
					if(i == 0){
						command.fRow = x - 49;
						command.fColomn = y - 65;
					}
					else{
						command.toRow = x - 49;
						command.toColomn = y - 65;
					}
					if(i == 0 || command.validArg)
						command.validArg = true;
				}
				else
					command.validArg = false;
				if(cur[0] != '>')
					return command;
				cur++;
			}
			else
				command.validArg = false;
			if(i == 0 &&(cur = strtok(NULL,delimiter)) == NULL)
				return command;
		}
		if(i == 0){
			if(strcmp(cur, "to") != 0)
				return command;
			if((cur = strtok(NULL,delimiter)) == NULL)
				return command;
		}
	}
	command.cmd = CH_MOVE;
	return command;
}

CHCommand parseGetMoves(char *cur,char delimiter[8]){
	char x,y;
	CHCommand command;
	command.cmd = CH_INVALID_LINE;
	command.validArg = false;
	if((cur = strtok(NULL,delimiter)) == NULL){
		return command;
	}
	if(cur[0]=='<'){
		cur++;
		x = cur[0];
		cur++;
		if(x > '0' && x < '9' ){
			if(cur[0] != ',')
				return command;
			cur++;
			y = cur[0];
			cur++;
			if(y >= 'A' && y <= 'H'){
			    command.fRow = x - 49;
			    command.fColomn = y - 65;
				command.validArg = true;
			}
			if(cur[0] != '>')
				return command;
		}
	}
	command.cmd = CH_GET_MOVES;
	return command;
}

CHCommand chParserPraseLine(const char* str) {
	char *cur;
	CHCommand command;
	char delimiter[8] = " \t\r\n";
	char strCopy[MAX_LINE_LENGTH];
	command.cmd = CH_INVALID_LINE;
	command.validArg = false;
	strcpy(strCopy,str);
	cur = strtok(strCopy, delimiter);
	if (strcmp(cur, "move") == 0) {
		return parseMove(cur,delimiter);
	} else if (strcmp(cur, "get_moves") == 0) {
		return parseGetMoves(cur,delimiter);
	} else if (strcmp(cur, "save") == 0) {
		if((cur = strtok(NULL,delimiter)) != NULL){
			command.cmd = CH_SAVE;
			sscanf(cur,"%s",command.path);
			command.validArg = true;
		}
	} else if (strcmp(cur, "undo") == 0) {
		command.cmd = CH_UNDO;
	} else if (strcmp(cur, "reset") == 0) {
		command.cmd = CH_RESET;
	} else if (strcmp(cur, "quit") == 0) {
		command.cmd = CH_QUIT;
	}
	if(strtok(NULL, delimiter) != NULL){
		command.cmd = SE_INVALID_LINE;
		command.validArg = false;
	}
	return command;
}
