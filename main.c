#include <stdio.h>
#include "board.h"
#include "move_logics.h"
#include "evaluation.h"
#include "simulation.h"
#include "engine.h"


/**
 * runs the CEngine
 * @param PDN initial board position
 * @param depth depth of the analysis
 * @param for_col checkers game player turn
 * @param num_eval number of move sequences to display
 */
void run(char * PDN, int depth, int for_col, int num_eval) {
    if (depth == 0 || for_col > 1 || for_col < 0 || num_eval < 1) return;
    // initialising and displaying the board (and initial evaluation)
    board test_board = new_board(PDN);
    see_board(test_board);
    printf("\nMaterial Evaluation: %d\n", get_material_evaluation(test_board));

    // constructing a move sequences tree and corresponding leaf list
    move_tree move_sqs = build_tree(test_board, get_material_evaluation, for_col, depth);
    leaf_list list = { .num_of_options = 0 };
    construct_leaf_list(&move_sqs, &list);

    // displaying extracted top move sequences
    printf("\nAT DEPTH %d, MOVE SEQUENCES: %d\n", depth, list.num_of_options);
    print_top(&move_sqs, list, for_col, num_eval);
}

/**
 * accommodates the command line user input requests
 */
void cli_request() {
    char PDN[50]; int depth; int color; int num_eval;
    printf("   ___    ___    __                _              \n"
           "  / __\\  / __\\  /__\\ _ __    __ _ (_) _ __    ___ \n"
           " / /    / /    /_\\  | '_ \\  / _` || || '_ \\  / _ \\\n"
           "/ /___ / /___ //__  | | | || (_| || || | | ||  __/\n"
           "\\____/ \\____/ \\__/  |_| |_| \\__, ||_||_| |_| \\___|\n"
           "                            |___/                 \n");

    printf("Enter Board Position (PDN):");
    fgets(PDN, 50, stdin);
    printf("Enter Analysis Depth (1-7):");
    scanf("%d", &depth);
    printf("Enter Player's Turn (0 - White, 1 - Black):");
    scanf("%d", &color);
    printf("Enter Number of Evaluations:");
    scanf("%d", &num_eval);
    // running the CEngine with given input
    run(PDN, depth, color, num_eval);
}

/**
 * runs the app
 */
int main() { cli_request(); getchar(); getchar(); }
