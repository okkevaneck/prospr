/* File:            depth_first.cpp
 * Description:     Source file for a depth-first search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "depth_first.hpp"

#include <algorithm>
#include <stack>
#include <vector>

/* A depth-first search function for finding a minimum energy conformation. */
Protein *depth_first(Protein *protein) {
  int max_length = protein->get_sequence().length();
  int dim = protein->get_dim();

  /* The first two amino acids are fixed to prevent y-axis symmetry. */
  if (max_length > 1)
    protein->place_amino(-1);
  if (max_length <= 2)
    return protein;

  /* Create a stack that tracks possible next moves. */
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
      if (protein->is_valid(move)) {
        protein->place_amino(move);
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
    if (placed_amino && protein->get_cur_len() == max_length) {
      score = protein->get_score();

      if (score < best_score) {
        best_score = score;
        best_hash = protein->hash_fold();
      }
    }

    /* Reset move if new amino acid is placed. Pop old move when
     * backtracking. Algorithm stops if stack filled with -dim - 1 moves.
     */
    if (placed_amino && protein->get_cur_len() != max_length) {
      move = dim;
    } else {
      move = -dim - 1;

      while (move == -dim - 1 && !dfs_stack.empty()) {
        protein->remove_amino();
        move = dfs_stack.top();
        dfs_stack.pop();
      }
    }
  } while (move != -dim - 1 || !dfs_stack.empty());

  /* Set best found conformation and return protein. */
  protein->set_hash(best_hash);

  return protein;
}
