#ifndef CHECKERS_ENGINE_ENGINE_H
#define CHECKERS_ENGINE_ENGINE_H

#include "simulation.h"
#include "move_tree.h"


/**
 * @return true if given list contains the index (integer), false otherwise
 */
bool contains_index(int index, const int * list, int size) {
    for (int i = 0; i < size; i++) {
        if (list[i] == index) return true;
    } return false;
}

/**
 * @param list list to check
 * @param for_col checkers game player turn
 * @return returns the list index with best evaluation
 */
int get_best_index(leaf_list list, int for_col, int accum_list[], int size) {
    int bestIndex = 0;
    for (int i = 0; i < list.num_of_options; i++) {
        if (((list.eval[bestIndex] < list.eval[i] && for_col == 0) ||
           (list.eval[bestIndex] > list.eval[i] && for_col == 1)) &&
            !contains_index(i, accum_list, size)) {
            bestIndex = i;
        }
    }
    return bestIndex;
}

/**
 * prints out the top {count} best move sequences
 */
void print_top(move_tree * root, leaf_list list, int for_col, int count) {
    int topIndexes[count];
    if (count > list.num_of_options) count = list.num_of_options;

    for (int i = 0; i < count; i++) {
        topIndexes[i] = get_best_index(list, for_col, topIndexes, count);
        printf("%d: ", i + 1);
        print_path(root, list.options[topIndexes[i]], list.eval[topIndexes[i]]);
        printf("EVAL: %d\n", list.eval[topIndexes[i]]);
    }
}

/**
 * builds a move tree from given branch
 * @param branch current move tree branch
 * @param on_board current board position
 * @param eval_method evaluation method to consider
 * @param for_col checkers game player turn
 * @param depth number of depth's left
 */
void build_tree_from(move_tree ** branch, board on_board, int (eval_method)(board), int for_col, int depth) {
    int next_col = 1 - for_col;
    if (depth == 0) return;
    if (is_capture(on_board, for_col)) next_col = 1 - next_col;
    // obtaining all possible move options from current position
    move_options existing_options = simulate_all(on_board, for_col);
    board temp_board = on_board;

    // building sibling branches based on existing_options
    for (int i = 0; i < existing_options.num_of_options; i++) {
        // preparing new board positions
        temp_board = on_board;
        make_move(&temp_board, existing_options.options[i]);

        // inserting sibling, building children of inserted sibling
        insert(branch, existing_options.options[i], eval_method(temp_board), for_col);
        build_tree_from(&(*get_sibling(branch, i))->first_child, temp_board, eval_method,
                        next_col, depth - 1);
    }
}

/**
 * @param on_board initial board position
 * @param eval_method evaluation method to consider
 * @param for_col checkers game player turn to build the tree for
 * @param depth depth of the analysis (depth 2 = 2 moves for that player)
 * @return potential checkers game move sequences (move tree)
 */
move_tree build_tree(board on_board, int (eval_method)(board), int for_col, int depth) {
    move_tree * root_branch = (move_tree * ) malloc(sizeof(move_tree));
    if (depth == 0) return *root_branch;
    move_option root_move = { .from_coord = {0, 0}, .to_coord = {-1, -1}};
    new_branch(&root_branch, root_move, -999, 1 - for_col);

    build_tree_from(&root_branch->first_child, on_board, eval_method, for_col, depth);
    return *root_branch;
}

#endif //CHECKERS_ENGINE_ENGINE_H
