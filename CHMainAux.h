#ifndef CHMAINAUX_H_
#define CHMAINAUX_H_

#include "SEMode.h"
#include "CHGame.h"
#include "CHParser.h"
#include "graphics/gameBoard.h"
#include "CHMiniMax.h"
#include "saveAndLoad.h"

/** set the game mode. console or gui
 * @param argc - number of arguments.
 * @param argv - the input for the program
 * @param isGuiMode - set to true if its gui mode and false otherwise
 * @return
 */
void setGuiMode(int argc, char** argv, bool *isGuiMode);

/** return the name that the piece represent
 * @param piece - the piece character.
 * @return
 * the name of the piece
 */
char *getPieceName(char piece);

/** show pawn promotion message
 * @param best_move - the move that done.
 * @return
 */
void showComputerPawnPromotionMsg(CHMoveNode* best_move);

/** return the player color
 * @param player - the player color number
 * @return
 * the player color name. black or white
 */
char *getPlayerName(int player);

/** change the current turn of the game
 * @param game - the game instant
 * @param isTurnChanged - flag to know if the turn has change.
 * @return
 * the player color name. black or white
 */
void change_turn(CHGame* game, bool *isTurnChanged);

/** set of actions that done after every turn
 * @param game - the game instant
 * @param best_move - the last move.
 * @param isTurnChanged - flag to know if the turn has change.
 * @param isGuiMode - flag to know if gui is use
 * @return
 * 1 - if the function success (-1) otherwise
 */
int end_of_move(CHGame* game, CHMoveNode* best_move, bool *isTurnChanged,bool isGuiMode);

/** print message to the plyer to insert his turn
 * @param src - the game instant
 * @return
 */
void printTurn(CHGame *src);

/** undo the last move in gui mode
 * @param game - the game instant
 * @param best_move - the last move.
 * @param window - the game window widget.
 * @return
 * message of the failure or CH_GAME_SUCCESS other wise
 */
CH_GAME_MESSAGE undoGuiHandle(CHGame* game, SPWindow* window, CHMoveNode *best_move);

/** undo the last move
 * @param game - the game instant
 * @param best_move - the last move.
 * @param window - the game window widget.
 * @param isTurnChanged - flag to know if the turn has change.
 * @param isGuiMode - flag to know if gui is use.
 * @return
 * message of the failure or CH_GAME_SUCCESS otherwise
 */
CH_GAME_MESSAGE undoHandle(CHGame* game, SPWindow* window, CHMoveNode *best_move, bool *isTurnChanged, bool isGuiMode);

/** return the piece to it last place
 * @param prevCellData - the prev cell the piece was in it.
 * @param widgetGameBoard - the game window widget.
 * @param isGuiMode - flag to know if gui is use.
 * @return
 */
void returnPieceToPlace(BoardCell *prevCellData, Widget *widgetGameBoard, SPWindow *window, bool isGuiMode);

/** handle move command
 * @param game - the game instant
 * @param best_move - the last move.
 * @param window - the game window widget.
 * @param isTurnChanged - flag to know if the turn has change.
 * @param isSaved - flag to see if the game save.
 * @param command - fthe command the player wrote.
 * @param isGuiMode - flag to know if gui is use.
 * @return
 */
void handleMoveCommand(CHGame* game, SPWindow* window, CHMoveNode *best_move, CHCommand command, bool *isTurnChanged, bool *isSaved,bool isGuiMode);

/** get moves list if player asked to
 * @param game - the game instant
 * @param window - the game window widget.
 * @param command - fthe command the player wrote.
 * @param isGuiMode - flag to know if gui is use.
 * @return
 */
void handleGetMovesCommand(CHGame* game, SPWindow* window, CHCommand command,bool isGuiMode);

/** handle the case that slot has pressed
 * @param draw - the game instant
 * @param window - the game window widget.
 * @param slot - the slot number
 * @return
 */
void handleSlotEvent(SPWindow** window, int *draw, int slot);

/** show a message in case the player save th game
 * @param game - the game instant
 * @param draw - flag to .
 * @param window - the game window widget.
 * @param isSaved - flag to see if the game save.
 * @param command - fthe command the player wrote.
 * @return
 */
void showSaveMessage(CHGame** game, SPWindow** window, CHCommand *command, bool *isSaved, int *draw);

/** handle restart event
 * @param game - the game instant
 * @param window - the game window widget.
 * @param isSaved - flag to see if the game save.
 * @return
 * message of the failure or CH_GAME_SUCCESS otherwise
 */
CH_GAME_MESSAGE handleRestartEvent(CHGame** game, SPWindow** window, bool *isSaved);

/** set from the command the coordinates
 * @param command - fthe command the player wrote.
 * @param window - the game window widget.
 * @return
 */
void setFromCommandCoordinates(SPWindow** window, CHCommand *command);

/** handle load event
 * @param game - the game instant
 * @param window - the game window widget.
 * @param draw - flag to .
 * @return
 * message of the failure or CH_GAME_SUCCESS otherwise
 */
CH_GAME_MESSAGE handleLoadEvents(CHGame** game, SPWindow** window, int *draw);

/** handle back event
 * @param game - the game instant
 * @param window - the game window widget.
 * @param @param draw - flag to .
 * @return
 * message of the failure or CH_GAME_SUCCESS otherwise
 */
CH_GAME_MESSAGE handleBackEvent(CHGame** game, SPWindow** window, int *draw);

/** handle back event
 * @param game - the game instant
 * @param window - the game window widget.
 * @param @param draw - flag to .
 * @return
 */
void handleMoveEvent(CHCommand *command, SPWindow** window);

/** handle all the events and send them to the write functions
 * @param game - the game instant
 * @param window - the game window widget.
 * @param draw - flag to .
 * @param best_move - the last move.
 * @param event - the event occured.
 * @return
 */
void handleMainEvents(CHGame** game, SPWindow** window, SDL_Event *event, CHCommand *command, bool *isSaved, int *draw, CHMoveNode *best_move);

/** playes the computer turn, use scoring function and alphabeta function in order to calculate the best move to play
 * according to the game difficulty.
 * @param game - the game instant
 * @param window - the game window widget.
 * @param draw - flag to .
 * @param best_move - the last move.
 * @param isTurnChanged - flag to know if the turn has change.
 * @param isSaved - flag to see if the game save.
 * @param isGuiMode - flag to know if gui is use.
 * @return
 */
void computerTurn(CHGame* game, SPWindow* window, CHMoveNode *best_move, bool *isSaved, bool *isTurnChanged, bool isGuiMode);

/** handle reset command. destroy the last game and create new one.
 * @param game - the game instant
 * @param window - the game window widget.
 * @param draw - flag to.
 * @param event - the event occured.
 * @param best_move - the last move.
 * @param isTurnChanged - flag to know if the turn has change.
 * @param isGuiMode - flag to know if gui is use.
 * @return
 */
void handleResetCommand(CHGame **game, SPWindow** window, CHMoveNode *best_move, SDL_Event *event, bool *isTurnChanged,bool isGuiMode);

/** handle all the commands the player set, send the command to the right command handler function.
 * @param game - the game instant
 * @param window - the game window widget.
 * @param draw - flag to.
 * @param event - the event occured.
 * @param best_move - the last move.
 * @param isTurnChanged - flag to know if the turn has change.
 * @param isSaved - flag to see if the game save.
 * @param isGuiMode - flag to know if gui is use.
 * @return
 */
void handleAllCommands(CHCommand command, CHGame **game, SPWindow** window, bool *isSaved, bool *isTurnChanged, CHMoveNode *best_move, SDL_Event *event,bool isGuiMode);

/** handle errors that occured in the memory or other errors that force exit from the game, free all allocate memory and exit.
 * @param game - the game instant
 * @param window - the game window widget.
 * @param best_move - the last move.
 * @param isGuiMode - flag to know if gui is use.
 * @return
 */
void errorHandling( CHGame *game, SPWindow* window, CHMoveNode *best_move, bool isGuiMode);

#endif /* CHMAINAUX_H_ */
