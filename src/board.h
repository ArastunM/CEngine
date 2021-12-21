#ifndef CHECKERS_ENGINE_BOARD_H
#define CHECKERS_ENGINE_BOARD_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


/**
 * coord: x and y coordinates of board tiles
 */
typedef struct {
    // x: x coordinate on board
    int x;
    // y: y coordinate on board
    int y;
} coord;

/**
 * piece: represents a checkers piece
 */
typedef struct {
    // color: 0 -> white, 1 -> black
    int color;
    // value: 10 -> pawn, 100 -> king
    int value;
} piece;

/**
 * tile: represents a checkers board tile
 */
typedef struct {
    // is_empty: whether the tile is occupied
    bool is_empty;
    // piece: piece placed on this tile
    piece piece;
} tile;

/**
 * board: represents a checkers board
 */
typedef struct {
    // max number of pieces on a checkers board -> 32
    // row size -> board size / 2 (only used tiles considered)
    tile tiles[8][4];
} board;

// board_size: size of board dimensions (default: 8X8)
int board_size = 8;


/**
 * @return true if the 2 coordinates are equal, false otherwise
 */
bool is_equal_coord(coord coord1, coord coord2) {
    return coord1.x == coord2.x && coord1.y == coord2.y;
}

/**
 * @param color color of the piece
 * @param value piece type (based on value)
 * @return initialised new piece
 */
piece new_piece(int color, int value) {
    piece new_piece = { .color = color, .value = value };
    return new_piece;
}

/**
 * @param code checkers notation code
 * @return initialised new piece
 */
piece new_pdn_piece(char code) {
    switch (code) {
        case 'X': return new_piece(0, 10);
        case 'K': return new_piece(0, 100);
        case 'x': return new_piece(1, 10);
        case 'k': return new_piece(1, 100);
        default: return new_piece(-1, -1);
    }
}

/**
 * @param code checkers notation code
 * @return initialised new checkers board tile
 */
tile new_pdn_tile(char code) {
    tile new_tile = { .is_empty = true };
    char notation_format[4] = "xkXK";

    // piece is initialised and set on the tile based on notation code
    if (strchr(notation_format, code) != NULL && code != '\0') {
        new_tile.is_empty = false; // tile is set as filled
        new_tile.piece = new_pdn_piece(code);
    } return new_tile;
}

/**
 * @param PDN portable draughts (checkers) notation used for board setup
 * @return initialised new checkers board
 */
board new_board(char * PDN) {
    board new_board;
    char decoded_notations[board_size][board_size / 2];
    // delim: '/' represents a new row
    char * token = strtok(PDN, "/");

    // initialising all board tiles based on extracted PDN content
    for (int row = 0; row < board_size; row++) {
        strcpy(decoded_notations[row], token);
        token = strtok(NULL, "/");
        for (int col = 0; col < board_size / 2; col++) {
            new_board.tiles[row][col] = new_pdn_tile(decoded_notations[row][col]);
        }
    } return new_board;
}

/**
 * @param coord_to_show coordinate to get description of
 * @return string description of given coordinate
 */
char * get_coord_desc(coord coord_to_show) {
    char * description = malloc(30);
    sprintf(description, "[%d][%d]", coord_to_show.x, coord_to_show.y);
    return description;
}

/**
 * @param piece_to_show piece to get description of
 * @return string description of given piece
 */
char * get_piece_desc(piece piece_to_show) {
    char * description = malloc(1);
    strcpy(description, piece_to_show.color == 0 ? "w" : "b");
    strcpy(description, piece_to_show.value == 10 ? description : strupr(description));
    return description;
}

/**
 * prints out the given checkers board
 * @param board_to_see board to see
 */
void see_board(board board_to_see) {
    printf("\t\tGAME BOARD\n");
    for (int row = 0; row < board_size; row++) {
        printf("--------------------------------\n");

        for (int col = 0; col < board_size; col++) {
            if (row % 2 != col % 2) {
                if (!board_to_see.tiles[row][col / 2].is_empty)
                    printf("%s", get_piece_desc
                            (board_to_see.tiles[row][col / 2].piece));
                else printf(" ");
            }

            else printf(" ");
            printf(" | ");
        } printf("\n");
    }
}

#endif //CHECKERS_ENGINE_BOARD_H
