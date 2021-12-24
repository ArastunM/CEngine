#ifndef CHECKERS_ENGINE_MOVE_TREE_H
#define CHECKERS_ENGINE_MOVE_TREE_H

#include "simulation.h"
#include <string.h>


/**
 * move_tree: N-ary tree structure (where N is irregular)
 *            Contains the potential checkers move sequences
 *            Used by engine.h
 */
typedef struct node {
    move_option move;
    int eval; // game evaluation after move
    int col; // 0 - white, 1 - black
    struct node * first_child;
    struct node * next_sibling;
} move_tree;

/**
 * leaf_list: structure containing list of extracted move_tree leaves
 */
typedef struct {
    // NOTE if more than 10000; buffer overflow may occur
    move_option options[10000];
    int eval[10000];
    int num_of_options;
} leaf_list;


/**
 * initialises a new move tree branch
 * @param branch address of the branch to initialise
 */
void new_branch(move_tree ** branch, move_option move, int eval, int of_col) {
    // new memory is allocated to the empty branch
    *branch = (move_tree * ) malloc(sizeof(move_tree));
    // initialising branch parameters
    (*branch)->move = move;
    (*branch)->eval = eval;
    (*branch)->col = of_col;
    (*branch)->first_child = NULL;
    (*branch)->next_sibling = NULL;
}

/**
 * inserts branch details to the given branch or to
 * a sibling branch (when the given branch is full)
 * @param branch address of the branch to insert to
 */
void insert(move_tree ** branch, move_option move, int eval, int of_col) {
    // initialises the branch with given move, if uninitialised
    if ((*branch) == NULL) new_branch(branch, move, eval, of_col);
    // insert on current node, if empty
    else if ((*branch)->move.from_coord.x == (*branch)->move.to_coord.x) (*branch)->move = move;
    // insert on next sibling otherwise
    else insert(&(*branch)->next_sibling, move, eval, of_col);
}

/**
 * @param branch address of the root branch to consider
 * @param num number/index of the sibling to return
 * @return branch(->*num)next_sibling
 */
move_tree ** get_sibling(move_tree ** branch, int num) {
    if ((*branch) == NULL) return NULL;
    else if (num <= 0) return branch;
    else return get_sibling(&(*branch)->next_sibling, num - 1);
}


/**
 * adds given details to the leaf list
 */
void add_leaf(leaf_list * list, move_option move, int eval) {
    list->options[list->num_of_options] = move;
    list->eval[list->num_of_options] = eval;
    list->num_of_options++;
}

/**
 * constructs a new leaf list
 * @param branch move tree to construct leaves of
 * @param list list to store constructed leaf list at
 */
void construct_leaf_list(move_tree * branch, leaf_list * list) {
    if (branch == NULL) return;
    if (branch->first_child == NULL) add_leaf(list, branch->move, branch->eval);
    else construct_leaf_list(branch->first_child, list);
    if (branch->next_sibling != NULL) construct_leaf_list(branch->next_sibling, list);
}

/**
 * @param root root branch to consider
 * @return true if given root branch has path to a node with given parameters,
 *         false otherwise
 */
bool has_path(move_tree * root, move_option move, int eval) {
    if (root == NULL) return false;
    if (is_equal_coord(root->move.from_coord, move.from_coord) &&
        is_equal_coord(root->move.to_coord, move.to_coord) && root->eval == eval) {
        return true;
    }
    if (has_path(root->next_sibling, move, eval) ||
        has_path(root->first_child, move, eval)) {
        return true;
    } else return false;
}

/**
 * prints out the given branch's part in general path
 * @param branch move tree branch
 */
void print_path_of(move_tree * branch) {
    printf("{%s; %s -> %s}, ", branch->col == 0 ? "White" : "Black",
           get_coord_desc(branch->move.from_coord), get_coord_desc(branch->move.to_coord));
}

/**
 * prints out the path (if it exists) from a root to a node with given parameters
 */
void print_path(move_tree * root, move_option move, int eval) {
    if (has_path(root->first_child, move, eval)) {
        // print the child, only if the path continues or ends through it
        if (has_path(root->first_child->first_child, move, eval) ||
           (!has_path(root->first_child->next_sibling, move, eval))) {
            print_path_of(root->first_child);
        } print_path(root->first_child, move, eval);
    }
    else if (has_path(root->next_sibling, move, eval)) {
        // print the sibling, only if the path continues or ends through it
        if (has_path(root->next_sibling->first_child, move, eval) ||
           (!has_path(root->next_sibling->next_sibling, move, eval))) {
            print_path_of(root->next_sibling);
        } print_path(root->next_sibling, move, eval);
    }
}

/**
 * @return string move coordinates starting the path to given values (move, eval)
 */
char * get_path_start(move_tree * branch, move_option move, int eval) {
    if (has_path(branch->first_child, move, eval)) {
        char * output = (char*)malloc(10 * sizeof(char));
        sprintf(output, "%s -> %s", get_coord_desc(branch->move.from_coord),
                                            get_coord_desc(branch->move.to_coord));
        return output;
    } else get_path_start(branch->next_sibling, move, eval);
}

#endif //CHECKERS_ENGINE_MOVE_TREE_H
