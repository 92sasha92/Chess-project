#ifndef SAVE_LOAD_H_
#define SAVE_LOAD_H_

/**
 * parse the read the game-board from the file and set it to the game.
 *
 * @param fp - The xml file that save the game-board.
 * @param src - the game instant.
 * @return
 * CH_GAME_INVALID_ARGUMENT - If src is NULL.
 * CH_GAME_FILE_PROBLEM - If the file cannot be created or modified.
 * CH_GAME_SUCCESS - otherwise.
 */
CH_GAME_MESSAGE readBoard(FILE *fp,CHGame *src);

/**
* exit when load fail.
*
* @param fp - The xml file that save the game-board.
* @return
* CH_GAME_INVALID_ARGUMENT.
*/
CH_GAME_MESSAGE exitload(FILE *fp);

/**
 * load the game from the file and set it to src (the game instant).
 *
 * @param path - path to the xml file.
 * @param src - the game instant.
 * @param currentTurn - the player to start play after load.
 * @param gameMode - the game mode.
 * @param gameDifficulty - the game difficulty if it is mode 1.
 * @param userColor - the user color if it is mode 1.
 * @return
 * CH_GAME_INVALID_ARGUMENT or CH_GAME_MEMORY_PROBLEM or CH_GAME_SUCCESS. accordingly to what chGameCreateMode1 return.
 */
CH_GAME_MESSAGE load(char *path, CHGame *src, int *currentTurn, int *gameMode, int *gameDifficulty, int *userColor);

/**
 * Save the current game state to the specified path file.
 *
 * @param src - The target game.
 * @param path - represent the file relative or full path.
 * @return
 * CH_GAME_INVALID_ARGUMENT - If src is NULL.
 * CH_GAME_FILE_PROBLEM - If the file cannot be created or modified.
 * CH_GAME_SUCCESS - otherwise.
 */
CH_GAME_MESSAGE chGameSave (CHGame* src,char *path);

/**
 * return the specific slot path.
 *
 * @param slotNum - The slot number.
 * @return
 * the path of the slot.
 */
char *slotPath(int slotNum);

/**
 * Save the current game state.
 *
 * @param src - The target game.
 * @return
 * CH_GAME_INVALID_ARGUMENT - If src is NULL.
 * CH_GAME_FILE_PROBLEM - If the file cannot be created or modified.
 * CH_GAME_SUCCESS - otherwise.
 */
CH_GAME_MESSAGE chGuiSave(CHGame* src);

/**
 * load the current game from the chosen slot.
 *
 * @param src - The target game.
 * @param slot - The slot number to load from.
 * @return
 * CH_GAME_INVALID_ARGUMENT - If src is NULL.
 * CH_GAME_FILE_PROBLEM - If the file cannot be created or modified.
 * mes - otherwise (the output of "load" function).
 */
CH_GAME_MESSAGE chGuiLoad(CHGame* src, int slot);

#endif /* SAVE_LOAD_H_ */

