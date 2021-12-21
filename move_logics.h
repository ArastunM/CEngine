#ifndef CHECKERS_ENGINE_MOVE_LOGICS_H
#define CHECKERS_ENGINE_MOVE_LOGICS_H

#include "board.h"


/**
 * @param coord1 first pair of coordinates [row][col]
 * @param coord2 second pair of coordinate [row][col]
 * @return true if given pair of coordinates are diagonally aligned, false otherwise
 */
bool is_diagonal(const coord coord1, const coord coord2) {
    // in board rows, only used tiles (see board.h board.tiles) are
    // considered hence a difference between odd and even rows should be made
    int fixed_col1 = coord1.x % 2 == 0 ? coord1.y * 2 + 1 : coord1.y * 2;
    int fixed_col2 = coord2.x % 2 == 0 ? coord2.y * 2 + 1 : coord2.y * 2;
    return abs(coord1.x - coord2.x) == abs(fixed_col1 - fixed_col2);
}

/**
 * @param coord_to_check coordinates to check [row][col]
 * @param coord1 first pair of coordinates [row][col]
 * @param coord2 second pair of coordinates [row][col]
 * @return true if coord_to_check is between coord1 and coord2 diagonally, false otherwise
 */
bool is_between(const coord coord_to_check, const coord coord1, const coord coord2) {
    return (coord1.x > coord_to_check.x && coord2.x < coord_to_check.x
    || coord1.x < coord_to_check.x && coord2.x > coord_to_check.x)
    && is_diagonal(coord_to_check, coord1) && is_diagonal(coord_to_check, coord2);
}

/**
 * @param on_board checkers board to refer to
 * @param for_col game turn (current color) to consider
 * @param from_coord from coordinate [row][col]
 * @param to_coord to coordinate [row][col]
 * NOTE: from_coord and to_coord are not considered as part of the diagonal
 *
 * @return 0 -> diagonal is completely empty
 *         1 -> diagonal only contains opponent's pieces
 *         2 -> diagonal is full
 */
int get_diagonal_status(board on_board, int for_col, const coord from_coord, const coord to_coord) {
    int status_code = 0;
    coord coord_to_check;
    for (int row = 0; row < board_size; row++) {
        coord_to_check.x = row;

        for (int col = 0; col < board_size / 2; col++) {
            coord_to_check.y = col;
            if (is_between(coord_to_check, from_coord, to_coord)
            && !on_board.tiles[row][col].is_empty) {
                if (on_board.tiles[row][col].piece.color == for_col) return 2;
                else status_code = 1;
            }
        }
    } return status_code;
}

/**
 * clears the given diagonal at given checkers board
 * @param on_board checkers board to refer to
 * @param from_coord from coordinate [row][col]
 * @param to_coord to coordinate [row][col]
 */
void clear_diagonal(board * on_board, const coord from_coord, const coord to_coord) {
    coord coord_to_check;
    for (int row = 0; row < board_size; row++) {
        coord_to_check.x = row;

        for (int col = 0; col < board_size / 2; col++) {
            coord_to_check.y = col;
            if (is_between(coord_to_check, from_coord, to_coord)
            && !on_board->tiles[row][col].is_empty) {
                on_board->tiles[row][col].is_empty = true;
            }
        }
    }
}

/**
 * @param on_board checkers board to refer to
 * @param from_coord from coordinate [row][col]
 * @param to_coord to coordinate [row][col]
 * @return true if a legal move would be possible, false otherwise
 */
bool pawn_move_possible(board on_board, const coord from_coord, const coord to_coord) {
    piece piece_to_move = on_board.tiles[from_coord.x][from_coord.y].piece;

    int row_difference =  piece_to_move.color == 0 ? 1 : -1;
    return from_coord.x == to_coord.x + row_difference && is_diagonal(from_coord, to_coord);
}

/**
 * @param on_board checkers board to refer to
 * @param from_coord from coordinate [row][col]
 * @param to_coord to coordinate [row][col]
 * @return true if a legal capture would be possible, false otherwise
 */
bool pawn_capture_possible(board on_board, const coord from_coord, const coord to_coord) {
    piece piece_to_move = on_board.tiles[from_coord.x][from_coord.y].piece;
    return abs(from_coord.x - to_coord.x) == 2
           && is_diagonal(from_coord, to_coord)
           && get_diagonal_status(on_board, piece_to_move.color, from_coord, to_coord) == 1;
}

/**
 * @param on_board checkers board to refer to
 * @param from_coord from coordinate[row][col]
 * @param to_coord to coordinate [row][col]
 * @return true if a legal move or capture would be possible, false otherwise
 */
bool king_move_possible(board on_board, const coord from_coord, const coord to_coord) {
    piece piece_to_move = on_board.tiles[from_coord.x][from_coord.y].piece;
    return get_diagonal_status(on_board, piece_to_move.color, from_coord, to_coord) != 2
           && is_diagonal(from_coord, to_coord);
}

/**
 * determines whether a checkers move would be possible
 * @param on_board checkers board to refer to
 * @param from_coord from coordinate [row][col]
 * @param to_coord  to coordinate [row][col]
 * @return true if a legal move would be possible, false otherwise
 */
bool move_possible(board on_board, const coord from_coord, const coord to_coord) {
    // from_coord should be occupied and to_coord should be empty
    if (on_board.tiles[from_coord.x][from_coord.y].is_empty
    || !on_board.tiles[to_coord.x][to_coord.y].is_empty) return false;

    piece piece_to_move = on_board.tiles[from_coord.x][from_coord.y].piece;
    // move possibility with men pieces
    if (piece_to_move.value == 10) return pawn_move_possible(on_board, from_coord, to_coord)
                                          || pawn_capture_possible(on_board, from_coord, to_coord);
    // move possibility with king pieces
    else if (piece_to_move.value == 100) return king_move_possible(on_board, from_coord, to_coord);
    return false;
}

#endif //CHECKERS_ENGINE_MOVE_LOGICS_H
