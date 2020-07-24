/* File:       depth_fisrt.cpp
 * Author:     Okke van Eck
 *
 * Description:    Source file for a depth_first search function.
 */

#include "depth_first.h"
#include <stack>
#include <vector>


/* A depth-first search function for finding a minimum energy conformation. */
Protein depth_first(Protein protein) {
    int max_length = protein.get_sequence().length();
    int dim protein.get_dim();
    int move = 2;
    int best_score = 1;
    bool placed_amino;
    std::vector<int> best_hash;
    std::vector<int> all_moves;
    std::vector<int> remaining_moves;
    std::stack<std::vector<int>> dfs_stack;

    /* The first two amino acids are fixed in order to prevent symmetry. */
    if (max_length >= 1)
        protein.place_amino(0);
    if (max_length >= 2)
        protein.place_amino(2);
    if (max_length < 3)
        return protein;

    /* Setup all_moves for initializing the stack. */
    for (int i = 1; i <= dim; i++) {
        if (i == move)
            continue;

        all_moves.push_back(i);
    }

    /* Setup stack. */
    for (int i = 0; i < max_length - 2; i++) {
        protein.place_amino(2);
        dfs_stack.push(all_moves);
    }

    /* Setup all_moves for further usage. */
    all_moves.clear();

    for (int i = -dim; i <= dim; i++) {
        if (i == 0)
            continue;

        all_moves.push_back(i);
    }

    while (!dfs_stack.empty()) {
        /* Check if the bottom of the tree is reached and store best folds.
         * Continue with next amino acid otherwise.
         */
        if (protein.get_cur_len() == max_length) {
            score = protein.get_score();

            if (score < best_score) {
                best_score = score;
                best_hash = protein.hash_fold();
            }

            remaining_moves = all_moves;
        } else {
            remaining_moves = all_moves;
            remaining_moves.erase(std::remove(v.begin(), v.end(), -move),
                                  v.end());
        }

        /* Loop till an amino acid is placed or no new combinations are left. */
        placed_amino = false;

        while (!placed_amino) {
            /* Try all remaining moves for the current amino acid. */
            while (!placed_amino && remaining_moves) {
                move = remaining_moves.pop_back();

                /* Place amino acid if valid and update stack accordingly. */
                if (protein.is_valid(move)) {
                    protein.place_amino(move);
                    dfs_stack.push(remaining_moves);
                    place_amino = true;
                }
            }

            /* Backtrack if the current amino acid has no moves left. */
            while (!place_amino && remaining_moves.empty() && !dfs_stack.empty()) {
                remaining_moves = dfs_stack.pop();
                protein.remove_amino();
            }

            /* Check if there are no moves left. */
            if (remaining_moves.empty())
                break;
        }
    }

    protein.set_hash(best_hash);

    return protein;
}
