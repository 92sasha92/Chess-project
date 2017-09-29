#include "SEParser.h"

bool seParserIsInt(const char* str) {
	unsigned int j = 0;
	if(strlen(str) >= 1 && str[0]=='-'){
		j = 1;
	}
	while (j < strlen(str)) {
		if (str[j] > '9' || str[j] < '0') {
			return false;
		}
		j++;
	}
	return true;
}


SECommand seParserPraseLine(const char* str) {
	int num;
	char *cur;
	SECommand command;
	char delimiter[8] = " \t\r\n";
	char strCopy[MAX_LINE_LENGTH];
	command.cmd = SE_INVALID_LINE;
	command.validArg = false;
	strcpy(strCopy,str);
	cur = strtok(strCopy, delimiter);
	if (strcmp(cur, "game_mode") == 0 || strcmp(cur, "difficulty") == 0 || strcmp(cur, "user_color") == 0) {
		if(strcmp(cur, "game_mode") == 0){
			command.cmd = SE_GAME_MODE;
		}
		else if(strcmp(cur, "difficulty") == 0){
			command.cmd = SE_DIFFICULTY;
		}
		else{
			command.cmd = SE_USER_COLOR;
		}
		if((cur = strtok(NULL,delimiter)) != NULL){
			if (seParserIsInt(cur)) {
				sscanf(cur, "%d",&num);
				command.arg = num;
				command.validArg = true;
			}
		}
	} else if (strcmp(cur, "load") == 0) {
		if((cur = strtok(NULL,delimiter)) != NULL){
			command.cmd = SE_LOAD;
			sscanf(cur,"%s",command.path);
			command.validArg = true;
		}
	} else if (strcmp(cur, "default") == 0) {
		command.cmd = SE_DEFAULT;
	} else if (strcmp(cur, "quit") == 0) {
		command.cmd = SE_QUIT;
	} else if (strcmp(cur, "print_setting") == 0) {
		command.cmd = SE_PRINT_SETTINGS;
	} else if (strcmp(cur, "start") == 0) {
		command.cmd = SE_START;
	}
	if(strtok(NULL, delimiter) != NULL){
		command.cmd = SE_INVALID_LINE;
		command.validArg = false;
	}
	return command;
}
