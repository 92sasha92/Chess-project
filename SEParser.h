/*
 * SEParser.h
 *
 *  Created on: Aug 26, 2017
 *      Author: sasha
 */

#ifndef SEPARSER_H_
#define SEPARSER_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//specify the maximum line length
#define MAX_LINE_LENGTH 1024

//a type used to represent a command
typedef enum {
	SE_GAME_MODE,
	SE_USER_COLOR,
	SE_DIFFICULTY,
	SE_LOAD,
	SE_DEFAULT,
	SE_PRINT_SETTINGS,
	SE_QUIT,
	SE_START,
	SE_INVALID_LINE
} SE_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SE_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
	char path[MAX_LINE_LENGTH];
} SECommand;

/**
 * Checks if a specified string represents a valid integer.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool seParserIsInt(const char* str);

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
SECommand seParserPraseLine(const char* str);

#endif /* SEPARSER_H_ */
