#ifndef CHECKERS_ENGINE_SIMULATION_H
#define CHECKERS_ENGINE_SIMULATION_H

#include "evaluation.h"


bool is_capture_at(board on_board, int for_col, coord from_coord, coord to_coord);
bool is_capture(board on_board, int for_col);

/**
 * move_option: represents a legal game move option
 */
typedef struct {
    // from_coord: coordinates to make the move from
    coord from_coord;
    // to_coord: coordinates to make the move to
    coord to_coord;
} move_option;

/**
 * move_options: list of all possible move options in current game position
 */
typedef struct {
    // options: list of all legal move options from current position
    move_option options[100];
    // num_of_options: number of move options
    int num_of_options;
} move_options;


/**
 * makes a checkers move based on given parameters
 * @param on_board checkers board to refer to
 * @param move move to make
 *
 * @return move status: 0 -> move not possible
 *                      1 -> move completed
 */
int make_move(board * on_board, move_option move) {
    if (!move_possible(*on_board, move.from_coord, move.to_coord)) return 0;

    piece piece_to_move = on_board->tiles[move.from_coord.x][move.from_coord.y].piece;
    // moving the piece
    clear_diagonal(on_board, move.from_coord, move.to_coord);
    on_board->tiles[move.from_coord.x][move.from_coord.y].is_empty = true;
    on_board->tiles[move.to_coord.x][move.to_coord.y].is_empty = false;
    on_board->tiles[move.to_coord.x][move.to_coord.y].piece = piece_to_move;
    return 1;
}

/**
 * returns the evaluation of the board after given move option
 * @param on_board checkers board to refer to
 * @param eval_method evaluation method to use
 * @param move move option to consider
 * @return calculated evaluation
 */
int get_evaluation_after(board on_board, int (eval_method)(board), move_option move) {
    make_move(&on_board, move);
    return (eval_method)(on_board);
}

/**
 * prints out the move option
 * @param option move option to print
 */
void see_move_option(move_option option) {
    printf("%s -> %s", get_coord_desc(option.from_coord),
           get_coord_desc(option.to_coord));
}

/**
 * prints out whether move options with given parameters would be possible
 * @param on_board checkers board to refer to
 * @param from_coord from coordinate [row][col]
 * @param to_coord  to coordinate [row][col]
 */
void see_move_options(board on_board, move_options options) {
    printf("\nMOVE OPTIONS [%d]\n", options.num_of_options);
    for (int i = 0; i < options.num_of_options; i++) {
        printf("option[%d]: ", i);
        see_move_option(options.options[i]);
        printf(" ME: %d\n", get_evaluation_after
        (on_board, &get_material_evaluation, options.options[i]));
    }
}

/**
 * simulates possible game move options from given coordinate
 * @param on_board checkers board to refer to
 * @param at_move game move to save simulations at
 * @param from_coord from coordinate [row][col]
 */
void simulate_moves_from(board on_board, move_options * at_move, const coord from_coord, bool has_capture) {
    coord to_coord;
    // simulates all possible moves from given coordinate in given board
    for (int row = 0; row < board_size; row++) {
        for (int col = 0; col < board_size / 2; col++) {
            to_coord.x = row; to_coord.y = col;
            if (move_possible(on_board, from_coord, to_coord) &&
               (is_capture_at(on_board, col, from_coord, to_coord) || !has_capture)) {
                at_move->options[at_move->num_of_options].from_coord = from_coord;
                at_move->options[at_move->num_of_options].to_coord = to_coord;
                at_move->num_of_options++;
            }
        }
    }
}

/**
 * simulates moves for pieces of given color
 * @param on_board checkers board to refer to
 * @param color piece color to consider
 * @param has_capture whether captures would exist in simulated moves
 */
move_options simulate_moves(board on_board, int color, bool has_capture) {
    move_options move = { .num_of_options = 0 };
    coord from_coord;
    // checking for moves from all tiles in given board
    for (int row = 0; row < board_size; row++) {
        for (int col = 0; col < board_size / 2; col++) {
            // tile must contain a piece and that piece should be of given color
            if (!on_board.tiles[row][col].is_empty && on_board.tiles[row][col].piece.color == color) {
                from_coord.x = row; from_coord.y = col;
                simulate_moves_from(on_board, &move, from_coord, has_capture);
            }
        }
    }
    return move;
}

/**
 * simulates all moves for checkers pieces
 * @param on_board checkers board to refer to
 * @param color piece color to consider
 */
move_options simulate_all(board on_board, int color) {
    return simulate_moves(on_board, color, is_capture(on_board, color));
}

/**
 * @return true if a capture at given coordinates exists, false otherwise
 */
bool is_capture_at(board on_board, int for_col, const coord from_coord, const coord to_coord) {
    return get_diagonal_status(on_board, for_col,  from_coord, to_coord) == 1;
}
/**
 * @param on_board checkers board to refer tp
 * @param for_col piece color to consider
 * @return true if a capture exists, false otherwise
 */
bool is_capture(board on_board, int for_col) {
    move_options moves = simulate_moves(on_board, for_col, false);
    for (int i = 0; i < moves.num_of_options; i++) {
        if (is_capture_at(on_board, for_col, moves.options[i].from_coord, moves.options[i].to_coord))
            return true;
    } return false;
}

#endif //CHECKERS_ENGINE_SIMULATION_H
