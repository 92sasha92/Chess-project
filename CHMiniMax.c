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

CHMoveNode* set_cur_best_move(char peice, CHMoveNode* best_move, int i, int j, CHMovesList* cur_piece_moves_list){
    best_move->from_row = i;
    best_move->from_col= j;
    best_move->to_row = cur_piece_moves_list->row;
    best_move->to_col= cur_piece_moves_list->col;
    best_move->current_piece = peice;
    return best_move;
}

BestMove rec_alphabeta(CHGame* src, int depth, int a, int b, int maximizer){
    BestMove score, new_score, error_move;
    error_move.best_depth = CH_ERROR_MOVE_DEPTH;
    if(src == NULL){
        return error_move;
    }
    CH_GAME_MESSAGE mes;
    CHMovesList* cur_piece_moves_list;
    CHMovesList* node;
    int i, j;
    int winner = chIsCheckmateOrTie(src);
    if ((depth == 0) || (winner != CH_GAME_NO_WIN_OR_TIE)){
        score.best_depth = depth;
        score.best_score = get_board_score(maximizer, src);
        return score;
    }
    score.best_depth = -1;
    if(src->currentTurn == maximizer){
        score.best_score = INT32_MIN;
        for (i = 0; i < CH_GAME_N_ROWS; i++) {
            for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
                cur_piece_moves_list = createMoveList(src->gameBoard,src->gameBoard[i][j], i, j, src->currentTurn);
                node = cur_piece_moves_list;
                if (cur_piece_moves_list == NULL){
                    return error_move;
                }

                if ((node->isValid)){
                    while (node != NULL){
                        mes = chGameSetMove(src, src->gameBoard[i][j], i, j, node->row, node->col, true);
                        if (mes == CH_GAME_SUCCESS){
                            if (((src->gameBoard[node->row][node->col] == CH_BLACK_PAWN) && (node->row == 0)) || ((src->gameBoard[node->row][node->col] == CH_WHITE_PAWN) && (node->row == 7))) {
                                new_score = pawn_promotion_rec_alphabeta(src, depth , a, b, maximizer , node->row, node->col);
                                src->gameBoard[node->row][node->col] = new_score.peice;
                                src->currentTurn = !(src->currentTurn);
                            } else {
                                src->currentTurn = !(src->currentTurn);
                                new_score = rec_alphabeta(src, depth - 1, a, b, maximizer);
                            }

                            if (new_score.best_depth == CH_ERROR_MOVE_DEPTH){
                                destroyMoveList(cur_piece_moves_list);
                                return error_move;
                            }
                            mes = chGameUndoPrevMove(src);
                            if (mes == CH_GAME_SUCCESS){
                                if ((score.best_score < new_score.best_score) || ((score.best_score == new_score.best_score) && (score.best_depth < new_score.best_depth))){
                                    score =  new_score;
                                    a = MAX(a, score.best_score);
                                    if (b < a)
                                        break;
                                }
                            } else {
                                printf("problem occurred when trying to undo move\n ");
                            }
                        }
                        node = node->next;
                    }
                }
                destroyMoveList(cur_piece_moves_list);
            }
        }
    }

    else{
        score.best_score = INT32_MAX;
        for (i = 0; i < CH_GAME_N_ROWS; i++) {
            for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
                cur_piece_moves_list = createMoveList(src->gameBoard, src->gameBoard[i][j], i, j, src->currentTurn);
                node = cur_piece_moves_list;
                if (cur_piece_moves_list == NULL){
                    return error_move;
                }

                if ((node->isValid)){
                    while (node != NULL){
                        mes = chGameSetMove(src, src->gameBoard[i][j], i, j, node->row,node->col, true);
                        if (mes == CH_GAME_SUCCESS){

                            if (((src->gameBoard[node->row][node->col] == CH_BLACK_PAWN) && (node->row == 0)) || ((src->gameBoard[node->row][node->col] == CH_WHITE_PAWN) && (node->row == 7))) {
                                new_score = pawn_promotion_rec_alphabeta(src, depth , a, b, maximizer , node->row, node->col);
                                src->gameBoard[node->row][node->col] = new_score.peice;
                                src->currentTurn = !(src->currentTurn);
                            } else {
                                src->currentTurn = !(src->currentTurn);
                                new_score = rec_alphabeta(src, depth - 1, a, b, maximizer);
                            }

                            if (new_score.best_depth == CH_ERROR_MOVE_DEPTH){
                                destroyMoveList(cur_piece_moves_list);
                                return error_move;
                            }
                            mes = chGameUndoPrevMove(src);
                            if (mes == CH_GAME_SUCCESS){
                                if ((score.best_score > new_score.best_score) || ((score.best_score == new_score.best_score) && (score.best_depth < new_score.best_depth))){
                                    score = new_score;
                                    b = MIN(b, score.best_score);
                                    if (b < a)
                                        break;
                                }
                            } else {
                                printf("problem occurred to undo move\n");
                            }
                        }
                        node = node->next;
                    }
                }
                destroyMoveList(cur_piece_moves_list);
            }
        }
    }
    return score;
}

void setMaxPawnPromotion(char peice, BestMove *best_move, CHGame* src, int depth , int a, int b, int maximizer , int toRow, int toCol){
    BestMove cur_move;
    src->gameBoard[toRow][toCol] = peice;
    cur_move = rec_alphabeta(src, depth - 1, a, b, maximizer);
    if (cur_move.best_score > best_move->best_score) {
        best_move->best_score = cur_move.best_score;
        best_move->peice = peice;
    }
}

void setMinPawnPromotion(char peice, BestMove *best_move, CHGame* src, int depth , int a, int b, int maximizer , int toRow, int toCol){
    BestMove cur_move;
    src->gameBoard[toRow][toCol] = peice;
    cur_move = rec_alphabeta(src, depth - 1, a, b, maximizer);
    if (cur_move.best_score < best_move->best_score) {
        best_move->best_score = cur_move.best_score;
        best_move->peice = peice;
    }
}

BestMove pawn_promotion_rec_alphabeta(CHGame* src, int depth , int a, int b, int maximizer , int toRow, int toCol) {
    BestMove best_move;
    best_move.peice = '\0';
    if (src->currentTurn == maximizer) {
        best_move.best_score = INT32_MIN;
        if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
            src->currentTurn = !(src->currentTurn);

            setMaxPawnPromotion(CH_WHITE_QUEEN, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMaxPawnPromotion(CH_WHITE_ROOK, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMaxPawnPromotion(CH_WHITE_BISHOP, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMaxPawnPromotion(CH_WHITE_KNIGHT, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMaxPawnPromotion(CH_WHITE_PAWN, &best_move, src,depth ,a, b, maximizer, toRow, toCol);

        } else {
            src->currentTurn = !(src->currentTurn);

            setMaxPawnPromotion(CH_BLACK_QUEEN, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMaxPawnPromotion(CH_BLACK_ROOK, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMaxPawnPromotion(CH_BLACK_BISHOP, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMaxPawnPromotion(CH_BLACK_KNIGHT, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMaxPawnPromotion(CH_BLACK_PAWN, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
        }
    } else {
        best_move.best_score = INT32_MAX;
        if (src->currentTurn == CH_GAME_WHITE_PLAYER_SYMBOL) {
            src->currentTurn = !(src->currentTurn);

            setMinPawnPromotion(CH_WHITE_QUEEN, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMinPawnPromotion(CH_WHITE_ROOK, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMinPawnPromotion(CH_WHITE_BISHOP, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMinPawnPromotion(CH_WHITE_KNIGHT, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMinPawnPromotion(CH_WHITE_PAWN, &best_move, src,depth ,a, b, maximizer, toRow, toCol);

        } else {
            src->currentTurn = !(src->currentTurn);

            setMinPawnPromotion(CH_BLACK_QUEEN, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMinPawnPromotion(CH_BLACK_ROOK, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMinPawnPromotion(CH_BLACK_BISHOP, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMinPawnPromotion(CH_BLACK_KNIGHT, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
            setMinPawnPromotion(CH_BLACK_PAWN, &best_move, src,depth ,a, b, maximizer, toRow, toCol);
        }
    }
    return best_move;
}

CH_GAME_MESSAGE alphabeta(CHGame* src, int depth, int maximizer, CHMoveNode* best_move ){
    if(src == NULL){
        return CH_GAME_INVALID_ARGUMENT;
    }
    spArrayListDestroy(src->list);
    src->list = spArrayListCreate(depth);
    if (!(src->list)) {
        chGameDestroy(src);
        return CH_GAME_MEMORY_PROBLEM;
    }
    CH_GAME_MESSAGE mes;
    CHMovesList* cur_piece_moves_list = NULL;
    CHMovesList* node = NULL;
    BestMove score, new_score;
    int i, j, a = INT32_MIN, b = INT32_MAX ;
    int winner = chIsCheckmateOrTie(src);
    if ((depth == 0) || (winner != CH_GAME_NO_WIN_OR_TIE)){
        chGameDestroy(src);
        return CH_GAME_INVALID_ARGUMENT;
    }
    score.best_score = INT32_MIN;
    score.best_depth = -1;
    for (i = 0; i < CH_GAME_N_ROWS; i++) {
        for (j = 0; j < CH_GAME_N_COLUMNS; j++) {
            cur_piece_moves_list = createMoveList(src->gameBoard,src->gameBoard[i][j], i, j, src->currentTurn);
            node = cur_piece_moves_list;
            if (cur_piece_moves_list == NULL){
                chGameDestroy(src);
                return CH_GAME_MEMORY_PROBLEM;
            }

            if ((cur_piece_moves_list->isValid)){
                while (node != NULL) {
                    mes = chGameSetMove(src, src->gameBoard[i][j], i, j, node->row,node->col, true);
                    if (mes == CH_GAME_SUCCESS){

                        if (((src->gameBoard[node->row][node->col] == CH_BLACK_PAWN) && (node->row == 0)) || ((src->gameBoard[node->row][node->col] == CH_WHITE_PAWN) && (node->row == 7))) {
                            new_score = pawn_promotion_rec_alphabeta(src, depth , a, b, maximizer , node->row, node->col);
                            src->gameBoard[node->row][node->col] = new_score.peice;
                            src->currentTurn = !(src->currentTurn);
                        } else {
                            src->currentTurn = !(src->currentTurn);
                            new_score = rec_alphabeta(src, depth - 1, a, b, maximizer);
                            new_score.peice = src->gameBoard[node->row][node->col];
                        }

                        if (new_score.best_depth == CH_ERROR_MOVE_DEPTH){
                            chGameDestroy(src);
                            destroyMoveList(cur_piece_moves_list);
                            return CH_GAME_MEMORY_PROBLEM;
                        }
                        mes = chGameUndoPrevMove(src);
                        if (mes == CH_GAME_SUCCESS){
                            if ((score.best_score < new_score.best_score) || ((score.best_score == new_score.best_score) && (score.best_depth < new_score.best_depth))){
                                score = new_score;
                                set_cur_best_move(score.peice, best_move, i, j, node);
                            }
                            a = MAX(a, score.best_score);
                            if (b < a)
                                break;
                        } else {
                            printf("problem occurred when trying to undo move\n ");
                        }
                    }
                    node = node->next;
                }
            }
            destroyMoveList(cur_piece_moves_list);
        }
    }
    chGameDestroy(src);
    return CH_GAME_SUCCESS;
}
