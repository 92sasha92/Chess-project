#include "CHMiniMax.h"

int get_piece_score(char piece, CHGame* src, int maximizer){
    int piece_score = 0;
            switch (piece) {
                case CH_BLACK_PAWN:
                case CH_WHITE_PAWN:piece_score = PAWN_SCORE;
                    break;
                case CH_BLACK_ROOK:
                case CH_WHITE_ROOK:piece_score = ROOK_SCORE;
                    break;
                case CH_BLACK_KNIGHT:
                case CH_WHITE_KNIGHT:piece_score = KNIGHT_SCORE;
                    break;
                case CH_BLACK_BISHOP:
                case CH_WHITE_BISHOP:piece_score = BISHOP_SCORE;
                    break;
                case CH_BLACK_KING:
                case CH_WHITE_KING:piece_score = KING_SCORE;
                    break;
                case CH_BLACK_QUEEN:
                case CH_WHITE_QUEEN:piece_score = QUEEN_SCORE;
                    break;
                default:
                    return 0;
    }
    if ((isABlackPiece(piece) && (maximizer == 0)) || (isAWhitePiece(piece) && (maximizer == 1))) {
        return piece_score;
    }
    return (piece_score * (-1));
}

int get_board_score(int maximizer, CHGame* src){
    int total_score = 0;
    int winner = chIsCheckmateOrTie(src);
    if ((winner == CH_GAME_WHITE_WINS) || (winner == CH_GAME_BLACK_WINS)){
        if(maximizer != src->currentTurn){
            return WIN_SCORE;
        }
        return (WIN_SCORE * (-1));
    }

    if (winner == CH_GAME_TIE){
        return TIE_SCORE;
    }

    for (int i = 0; i < CH_GAME_N_ROWS; i++) {
        for (int j = 0; j < CH_GAME_N_COLUMNS; j++) {
            total_score += get_piece_score(src->gameBoard[i][j], src, maximizer);
        }
    }
    return total_score;
}

CHMoveNode* set_cur_best_move(CHGame* src, CHMoveNode* best_move, int i, int j, CHMovesList* cur_piece_moves_list){
    best_move->from_row = i;
    best_move->from_col= j;
    best_move->to_row = cur_piece_moves_list->row;
    best_move->to_col= cur_piece_moves_list->col;
    best_move->current_piece = src->gameBoard[i][j];
    return best_move;
}

BestMove *rec_alphabeta(CHGame* src, int depth, int a, int b, int maximizer){
    if(src == NULL){
        printf("there is no game to check alphabeta function\n");
        return NULL;
    }
    CH_GAME_MESSAGE mes;
    BestMove best, score, new_score;
    CHMovesList* cur_piece_moves_list;
    int i, j;
    int winner = chIsCheckmateOrTie(src);
    if ((depth == 0) || (winner != CH_GAME_NO_WIN_OR_TIE)){
        best.best_depth = depth;
        best.best_score = get_board_score(maximizer, src);
        return &best;
    }
    score.best_depth = -1;
    if(src->currentTurn == maximizer){
        score.best_score = INT32_MIN;
        for (i = 0; i < CH_GAME_N_ROWS; i++) {
            for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
                cur_piece_moves_list = createMoveList(src->gameBoard,src->gameBoard[i][j], i, j, src->currentTurn);
                if (cur_piece_moves_list == NULL){
                    return NULL;
                }

                if ((cur_piece_moves_list->isValid)){
                    while (cur_piece_moves_list != NULL){
                        mes = chGameSetMove(src, i, j, cur_piece_moves_list->row, cur_piece_moves_list->col, true);
                        if (mes == CH_GAME_SUCCESS){
                            if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
                                src->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
                            } else {
                                src->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
                            }
                            new_score = *rec_alphabeta(src, depth - 1, a, b, maximizer);
                            mes = chGameUndoPrevMove(src);
                            if (mes == CH_GAME_SUCCESS){
                                if ((score.best_score < new_score.best_score) || ((score.best_score == new_score.best_score) && (score.best_depth < new_score.best_depth))){
                                    score =  new_score;
                                    a = MAX(a, score.best_score);
                                    if (b <= a)
                                        break;
                                }
                            } else {
                                printf("problem occurred when trying to undo move\n ");
                            }
                        }
                        cur_piece_moves_list = cur_piece_moves_list->next;
                    }
                }
            }
        }
    }

    else{
        score.best_score = INT32_MAX;
        for (i = 0; i < CH_GAME_N_ROWS; i++) {
            for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
                cur_piece_moves_list = createMoveList(src->gameBoard, src->gameBoard[i][j], i, j, src->currentTurn);
                if (cur_piece_moves_list == NULL){
                    return NULL;
                }

                if ((cur_piece_moves_list->isValid)){
                    while (cur_piece_moves_list != NULL){
                        mes = chGameSetMove(src, i, j, cur_piece_moves_list->row,cur_piece_moves_list->col, true);
                        if (mes == CH_GAME_SUCCESS){
                            if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
                                src->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
                            } else {
                                src->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
                            }
                            new_score = *rec_alphabeta(src, depth - 1, a, b, maximizer);
                            mes = chGameUndoPrevMove(src);
                            if (mes == CH_GAME_SUCCESS){
                                if ((score.best_score > new_score.best_score) || ((score.best_score == new_score.best_score) && (score.best_depth < new_score.best_depth))){
                                    score = new_score;
                                    b = MIN(b, score.best_score);
                                    if (b <= a)
                                        break;
                                }
                            } else {
                                printf("problem occurred when trying to undo move\n");
                            }
                        }
                        cur_piece_moves_list = cur_piece_moves_list->next;
                    }
                }
            }
        }
    }
    destroyMoveList(cur_piece_moves_list);
    return &score;
}

CHMoveNode* alphabeta(CHGame* src, int depth, int maximizer, CHMoveNode* best_move ){
    if(src == NULL){
        printf("there is no game to check alphabeta function\n");
        return NULL;
    }
    spArrayListDestroy(src->list);
    src->list = spArrayListCreate(depth);
    CH_GAME_MESSAGE mes;
    CHMovesList* cur_piece_moves_list = NULL;
    BestMove score, new_score;
    int i, j, a = INT32_MIN, b = INT32_MAX ;
    int winner = chIsCheckmateOrTie(src);
    if ((depth == 0) || (winner != CH_GAME_NO_WIN_OR_TIE)){
        printf("cant check for best move\n");
        return NULL;
    }

    if(src->currentTurn == maximizer){
        score.best_score = INT32_MIN;
        score.best_depth = -1;
        for (i = 0; i < CH_GAME_N_ROWS; i++) {
            for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
                cur_piece_moves_list = createMoveList(src->gameBoard,src->gameBoard[i][j], i, j, src->currentTurn);
                if (cur_piece_moves_list == NULL){
                    return NULL;
                }

                if ((cur_piece_moves_list->isValid)){
                    while (cur_piece_moves_list != NULL) {
                        mes = chGameSetMove(src, i, j, cur_piece_moves_list->row,cur_piece_moves_list->col, true);
                        if (mes == CH_GAME_SUCCESS){
                            if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
                                src->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
                            } else {
                                src->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
                            }
                            new_score = *rec_alphabeta(src, depth - 1, a, b, maximizer);
                            mes = chGameUndoPrevMove(src);
                            if (mes == CH_GAME_SUCCESS){
                                if ((score.best_score < new_score.best_score) || ((score.best_score == new_score.best_score) && (score.best_depth < new_score.best_depth))){
                                    score = new_score;
                                    set_cur_best_move(src, best_move, i, j, cur_piece_moves_list);
                                }
                                a = MAX(a, score.best_score);
                                if (b <= a)
                                    break;
                            } else {
                                printf("problem occurred when trying to undo move\n ");
                            }
                        }
                        cur_piece_moves_list = cur_piece_moves_list->next;
                    }
                }
            }
        }
    }

    else{
        printf("hii");
//        score = INT32_MAX;
//        for (i = 0; i < CH_GAME_N_ROWS; i++) {
//            for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
//                CHMovesList* cur_piece_moves_list = createMoveList(src->gameBoard,src->gameBoard[i][j], i, j, src->currentTurn);
//                if (cur_piece_moves_list == NULL){
//                    return NULL;
//                }
//
//                if ((cur_piece_moves_list->isValid)){
//                    while (cur_piece_moves_list != NULL){
//                        mes = chGameSetMove(src, i, j, cur_piece_moves_list->row,cur_piece_moves_list->col, true);
//                        if (mes == CH_GAME_SUCCESS){
//                            if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
//                                src->currentTurn = CH_GAME_BLACK_PLAYER_SYMBOL;
//                            } else {
//                                src->currentTurn = CH_GAME_WHITE_PLAYER_SYMBOL;
//                            }
//                            new_score = rec_alphabeta(src, depth - 1, a, b, maximizer);
//                            mes = chGameUndoPrevMove(src);
//                            if (mes == CH_GAME_SUCCESS){
//                                if (score >= new_score){
//                                    score = new_score;
//                                    set_cur_best_move(src, best_move, i, j, cur_piece_moves_list);
//                                }
//                                b = MIN(b, score);
//                                if (b < a)
//                                    break;
//                            } else {
//                                printf("problem occurred when trying to undo move\n ");
//                            }
//                        }
//                        cur_piece_moves_list = cur_piece_moves_list->next;
//                    }
//                }
//            }
//        }
    }
    destroyMoveList(cur_piece_moves_list);
    return best_move;
}
