#include <stdio.h>
#include <jni.h>

#include "bots_CEngineAccess.h"

#include "board.h"
#include "move_logics.h"
#include "evaluation.h"
#include "simulation.h"
#include "engine.h"


/**
 * input - PDN, color (player's turn), depth (depth of analysis)
 * output - best move (first move of the best sequence)
 */
char * get_best(char * PDN, int depth, int for_col) {
    if (depth == 0 || for_col > 1 || for_col < 0) return NULL;
    // initialising the board
    board test_board = new_board(PDN);

    // constructing a move sequences tree and corresponding leaf list
    move_tree move_sqs = build_tree(test_board, get_material_evaluation, for_col, depth);
    leaf_list list = { .num_of_options = 0 };
    construct_leaf_list(&move_sqs, &list);

    // obtaining the best move sequence index
    int best_index = get_best_index(list, for_col, NULL, 0);
    return get_path_start(&(*move_sqs.first_child), list.options[best_index], list.eval[best_index]);
}

/**
 * obtaining the request details from Java (CheckersApp), returning the best move in given position
 */
JNIEXPORT jstring JNICALL Java_bots_CEngineAccess_request(JNIEnv *env, jobject javaobj,
                                           jstring pdn_raw, jint depth, jint col)
{
    char * pdn = (*env)->GetStringUTFChars(env,pdn_raw, NULL);
    char * best_move = get_best(pdn, depth, col);
    jstring result = (*env)->NewStringUTF(env, best_move);
    return result;
}
