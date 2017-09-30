
#ifndef SAVE_LOAD_H_
#define SAVE_LOAD_H_

CH_GAME_MESSAGE load(char *path, CHGame *src, int *currentTurn, int *gameMode, int *gameDifficulty, int *userColor);
/**
 * Save the current game state to the specified file.
 *
 * @param src - The target game.
 * @param path - represent the file relative or full path.
 * @return
 * CH_GAME_INVALID_ARGUMENT - If src is NULL.
 * CH_GAME_FILE_PROBLEM - If the file cannot be created or modified.
 * CH_GAME_SUCCESS - otherwise.
 */
CH_GAME_MESSAGE chGameSave(CHGame* src, char *path);
CH_GAME_MESSAGE chGuiSave(CHGame* game);
CH_GAME_MESSAGE chGuiLoad(CHGame* game,int slot);
#endif /* SAVE_LOAD_H_ */
