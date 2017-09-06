/*
 * CHParser.h
 *
 *  Created on: Aug 29, 2017
 *      Author: sasha
 */

#ifndef CHPARSER_H_
#define CHPARSER_H_

//specify the maximum line length
#define MAX_LINE_LENGTH 1024
#include "SEParser.h"
//a type used to represent a command
typedef enum ch{
	CH_MOVE,
	CH_GET_MOVES,
	CH_SAVE,
	CH_UNDO,
	CH_RESET,
	CH_QUIT,
	CH_INVALID_LINE
} CH_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_ch {
	CH_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int fRow;
	int fColomn;
	int toRow;
	int toColomn;
	char path[MAX_LINE_LENGTH];
} CHCommand;


/**
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is game_mode/difficulty/user_color and the integer argument
 *              is valid or the command is load and there is a string after the load command
 *   arg      - the integer argument in case validArg is set to true and the command is not load
 *   path - the string argument in case validArg is set to true and the command is load
 */
CHCommand chParserPraseLine(const char* str);

#endif /* CHPARSER_H_ */
