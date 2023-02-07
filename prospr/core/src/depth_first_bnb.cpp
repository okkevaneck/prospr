/* File:            depth_first_bnb.cpp
 * Description:     Source file for a depth-first branch-and-bound search
 *                  function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "depth_first_bnb.hpp"

#include <math.h>

#include <algorithm>
#include <numeric>
#include <stack>
#include <vector>

/* All possible variables required by custom pruning. */
struct prune_vars {
  int max_length;
  int no_neighbors;
  std::vector<int> max_weights;
  int num_idxs;
  std::vector<int> h_idxs;
  std::vector<std::vector<int>> bond_dists;
};

/* Returns true if the branch cannot produce a better score. Possible bonds are
 * at all places surrounding the to be placed H-aminos.
 */
bool naive_prune(Protein *protein, int move, int best_score,
                 struct prune_vars *p_vars) {
  protein->place_amino(move, false);

  int cur_len = protein->get_cur_len();
  int cur_score = protein->get_score();

  /* Compute the sum of the remaining possible scoring connections. */
  int branch_score = p_vars->no_neighbors *
                     std::accumulate(p_vars->max_weights.begin() + cur_len,
                                     p_vars->max_weights.end(), 0);

  /* End of amino has 1 more possible way of connecting, update
   * branch_score accordingly.
   */
  if (cur_len != p_vars->max_length && p_vars->max_weights.back() != 0)
    branch_score += p_vars->max_weights.back();

  protein->remove_amino();

  return cur_score + branch_score >= best_score;
}

/* Returns true if the branch cannot produce a better score. Possible bonds are
 * all reachable H-aminos within the to be placed sequence.
 */
bool reach_prune(Protein *protein, int move, int best_score,
                 struct prune_vars *p_vars) {
  protein->place_amino(move, false);

  int cur_len = protein->get_cur_len();
  int cur_score = protein->get_score();

  /* Compute to be placed aminos possibly making bonds. */
  int future_aminos = 0;
  for (auto h_idx : p_vars->h_idxs) {
    if (h_idx >= cur_len) {
      future_aminos++;
    }
  }

  /* Compute branch score with the to be placed amino acids. */
  int branch_score = 0;
  for (int i = p_vars->num_idxs - future_aminos; i < p_vars->num_idxs; i++) {
    /* Check if bondable amino is last of protein. */
    if (p_vars->h_idxs[i] == p_vars->max_length - 1) {
      /* The last amino being bondable can create an additional bond. */
      branch_score += p_vars->max_weights[p_vars->h_idxs[i]] *
                      std::min((size_t)p_vars->no_neighbors + 1,
                               p_vars->bond_dists[i].size());
    } else {
      branch_score +=
          p_vars->max_weights[p_vars->h_idxs[i]] *
          std::min((size_t)p_vars->no_neighbors, p_vars->bond_dists[i].size());
    }
  }

  protein->remove_amino();

  return cur_score + branch_score >= best_score;
}

/* A depth-first branch-and-bound search function for finding a minimum
 * energy conformation.
 */
Protein *depth_first_bnb(Protein *protein, std::string prune_func) {
  int max_length = protein->get_sequence().length();
  int dim = protein->get_dim();
  int no_neighbors = (int)pow(2, (dim - 1));

  /* The first two amino acids are fixed to prevent y-axis symmetry. */
  if (max_length > 1)
    protein->place_amino(-1);
  if (max_length <= 2)
    return protein;

  /* Init default prune functions and arguments. */
  auto prune_branch = naive_prune;
  struct prune_vars p_vars = {};
  p_vars.max_length = max_length;
  p_vars.no_neighbors = no_neighbors;
  p_vars.max_weights = protein->get_max_weights();

  /* Determine if to use another prune criteria, and extend prune vars. */
  if (prune_func == "reach_prune") {
    prune_branch = reach_prune;
    std::vector<int> cur_dists = {};

    /* Create vector with distances between aminos that can create bonds. */
    for (int i = 0; i < max_length; i++) {
      /* Only include indexes that can create bonds. */
      if (p_vars.max_weights[i] != 0) {
        /* Create vector with distances to previous bondable aminos. */
        for (int idx : p_vars.h_idxs) {
          if (i - idx >= 3 && (i - idx) % 2 == 1) {
            cur_dists.push_back(i - idx);
          }
        }

        /* Add distance vector to set and clear for next iterations. */
        p_vars.bond_dists.push_back(cur_dists);
        cur_dists.clear();

        /* Add current index for next iterations. */
        p_vars.h_idxs.push_back(i);
      }
    }

    /* Store number of bondable idxs. */
    p_vars.num_idxs = p_vars.h_idxs.size();
  }

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
      if (protein->is_valid(move) &&
          !prune_branch(protein, move, best_score, &p_vars)) {
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
