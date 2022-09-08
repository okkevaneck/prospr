/* File:            depth_first_bnb.cpp
 * Description:     Source file for a depth-first branch-and-bound search
 *                      function.
 */

#include "depth_first_bnb.hpp"
#include <stack>
#include <vector>
#include <algorithm>
#include <math.h>


namespace dfs_bnb {
    // TODO: Change to use this->max_weights variables correctly instead of h_idxs!
    /* Returns true if the branch cannot produce a better score. */
    bool prune_branch(Protein protein, int max_length, int no_neighbors,
            int move, int best_score) {
        protein.place_amino(move);

        int cur_len = protein.get_cur_len();
        int cur_score = protein.get_score();
        std::vector<int> h_idxs = protein.get_h_idxs();

        /* Compute the number of possible scoring connections for branch. */
        std::vector<int>::iterator next_h_idx = std::lower_bound(h_idxs.begin(),
                                                                  h_idxs.end(),
                                                                  cur_len - 1);
        int h_left = h_idxs.size() - (next_h_idx - h_idxs.begin());
        int branch_score = -no_neighbors * h_left;

        /* End of amino has 1 more possible way of connecting, update
         * branch_score accordingly.
         */
        if (cur_len != max_length && h_idxs.back() == max_length - 1)
            branch_score--;

        protein.remove_amino();

        return cur_score + branch_score >= best_score;
    }

    /* A depth-first branch-and-bound search function for finding a minimum
     * energy conformation.
     */
    Protein depth_first_bnb(Protein protein) {
        int max_length = protein.get_sequence().length();
        int dim = protein.get_dim();
        int no_neighbors = (int)pow(2, (dim - 1));

        /* The first two amino acids are fixed to prevent y-axis symmetry. */
        if (max_length > 1)
            protein.place_amino(-1);
        if (max_length <= 2)
            return protein;

        /* Create a stack that tracks possible next moves, and a move variable
         * that contains the next possible move to try.
         */
        std::stack<int> dfs_stack;

        /* Create a move variable that contains the next possible move to try.
         * The third amino acid can only move in negative direction to prevent
         * x-axis symmetry.
         */
        int move = -1;

        /* Declare and set variables for the depth-first search. */
        bool placed_amino = false;
        int best_score = 1;
        int score;
        std::vector<int> best_hash;

        do {
            placed_amino = false;

            /* Try to place the current amino acid. */
            while (!placed_amino && move != -dim - 1) {
                if (protein.is_valid(move) && !prune_branch(protein, max_length,
                        no_neighbors, move, best_score)) {
                    protein.place_amino(move);
                    placed_amino = true;

                    /* Push next possible move if any exists. */
                    if (move != -dim) {
                        /* Skip 0 as potential next move. */
                        if (move == 1)
                            dfs_stack.push(-1);
                        else
                            dfs_stack.push(move - 1);
                    } else {
                        /* Push -dim - 1 if no possible next moves left. */
                        dfs_stack.push(move - 1);
                    }
                } else {
                    /* Skip 0 as potential next move. */
                    if (move == 1)
                        move = -1;
                    else
                        move -= 1;
                }
            }

            /* Check if a more optimal conformation was found. */
            if (placed_amino && protein.get_cur_len() == max_length) {
                score = protein.get_score();

                if (score < best_score) {
                    best_score = score;
                    best_hash = protein.hash_fold();
                }
            }

            /* Reset move if new amino acid is placed. Pop old move when
             * backtracking. Algorithm stops if stack filled with -dim - 1 moves.
             */
            if (placed_amino && protein.get_cur_len() != max_length) {
                move = dim;
            } else {
                move = -dim - 1;

                while (move == -dim - 1 && !dfs_stack.empty()) {
                    protein.remove_amino();
                    move = dfs_stack.top();
                    dfs_stack.pop();
                }
            }
        } while (move != -dim - 1 || !dfs_stack.empty());

        /* Set best found conformation and return protein. */
        protein.set_hash(best_hash);

        return protein;
    }
}
