//
// Created by Guy Druker on 09/09/2017.
//

#ifndef CHESS_PROJECT_CHMINIMAX_H
#define CHESS_PROJECT_CHMINIMAX_H

#include "SPArrayList.h"
#include "CHGame.h"
#include "CHMoves.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define PAWN_SCORE 1
#define KNIGHT_SCORE 3
#define BISHOP_SCORE 3
#define ROOK_SCORE 5
#define QUEEN_SCORE 9
#define KING_SCORE 100

#define WIN_SCORE INT32_MAX
#define LOOSE_SCORE INT32_MIN
#define TIE_SCORE -1000

typedef struct Move {
  int best_score;
  int best_depth;
  char peice;
} BestMove;

/** return the the score of that specific piece.
 * @param piece - a specific piece name
 * @param maximizer - the player hwo call the function.
 * @param src - the game struct.
 * @return
 * the piece score in accordance to the maximizer.
 */
int get_piece_score(char piece, CHGame* src, int maximizer);

/** return the the score of the board.
 * @param maximizer - the player hwo call the function.
 * @param src - the game struct.
 * @return
 * the sum of the score of all the pieces on board.
 */
int get_board_score(int maximizer, CHGame* src);

/** updated best_move struct.
 * @param src - the game struct.
 * @param best_move - struct of movement.
 * @param i - row moved from.
 * @param j - column moved from.
 * @param cur_piece_moves_list - the best option to move the piece .
 * @return
 * updated best_move struct.
 */
CHMoveNode* set_cur_best_move(char peice, CHMoveNode* best_move, int i, int j, CHMovesList* cur_piece_moves_list);

/** recursive function to compute the best option to move, use from depth 2.
 * @param src - the game struct.
 * @param depth - the maximum depth to calculate the scoring function.
 * @param a - alpha value, used to cut off if there is no need to keep calculate the scoring of the sub-tree. use in maximum level.
 * @param b - beta value, used to cut off if there is no need to keep calculate the scoring of the sub-tree. use in minimum level.
 * @param maximizer - the player hwo call the function.
 * @return
 * the optimal score for this sub tree in respect to the minimum or maximum level.
 */
BestMove rec_alphabeta(CHGame* src, int depth, int a, int b, int maximizer);

/** compute the best option to move.
 * @param src - the game struct.
 * @param depth - the maximum depth to calculate the scoring function.
 * @param best_move - struct of the best move to do to be set.
 * @param maximizer - the player hwo call the function.
 * @return
 * struct of the the optimal piece to move and where to move it. (the data will set in the best_move struct)
 */
CH_GAME_MESSAGE alphabeta(CHGame* src, int depth, int maximizer, CHMoveNode* best_move );

BestMove pawn_promotion_rec_alphabeta(CHGame* src, int depth , int a, int b, int maximizer , int toRow, int toCol);

#endif //CHESS_PROJECT_CHMINIMAX_H
