#ifndef CHECKERS_ENGINE_EVALUATION_H
#define CHECKERS_ENGINE_EVALUATION_H

#include "board.h"


/**
 * material evaluation is a strict position evaluation purely based on existing pieces
 * >0 white has an advantage, =0 equal position, <0 black has an advantage
 *
 * @param on_board checkers board to refer to
 * @return calculated material evaluation
 */
int get_material_evaluation(board on_board) {
    int material_dist = 0;
    for (int row = 0; row < board_size; row++) {
        for (int col = 0; col < board_size / 2; col++) {
            // each non-empty board tile affects the calculated material distribution
            if (!on_board.tiles[row][col].is_empty) {
                material_dist += on_board.tiles[row][col].piece.color == 0 ?
                                 on_board.tiles[row][col].piece.value : -(on_board.tiles[row][col].piece.value);
            }
        }
    } return material_dist;
}

#endif //CHECKERS_ENGINE_EVALUATION_H
