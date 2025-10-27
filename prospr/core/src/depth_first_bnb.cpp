/* File:            depth_first_bnb.cpp
 * Description:     Source file for a depth-first branch-and-bound search
 *                  function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "depth_first_bnb.hpp"
#include "utils.hpp"

#include <math.h>

#include <algorithm>
#include <atomic>
#include <csignal>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stack>
#include <vector>

/* All possible variables required by custom pruning. */
struct prune_vars {
  size_t max_length;
  size_t no_neighbors;
  std::vector<int> max_weights;
  size_t num_idxs;
  std::vector<size_t> h_idxs;
  std::vector<std::vector<size_t>> bond_dists;
};

/* Returns true if the branch cannot produce a better score. Possible bonds are
 * at all places surrounding the to be placed H-aminos.
 */
bool naive_prune(Protein *protein, int move, int best_score,
                 struct prune_vars *p_vars) {
  protein->place_amino(move, false);

  size_t cur_len = protein->get_cur_len();
  int cur_score = protein->get_score();

  /* Compute the sum of the remaining possible scoring connections. */
  int branch_score = (int)p_vars->no_neighbors *
                     std::accumulate(p_vars->max_weights.begin() + (int)cur_len,
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

  size_t cur_len = protein->get_cur_len();
  int cur_score = protein->get_score();

  /* Compute to be placed aminos possibly making bonds. */
  int remaining_Hs = 0;
  for (auto h_idx : p_vars->h_idxs) {
    if (h_idx >= cur_len) {
      remaining_Hs++;
    }
  }

  /* Compute branch score with the to be placed amino acids. */
  int branch_score = 0;
  for (size_t i = p_vars->num_idxs - remaining_Hs; i < p_vars->num_idxs; i++) {
    /* Check if bondable amino is last of protein. */
    if (p_vars->h_idxs[i] == p_vars->max_length - 1) {
      /* The last amino being bondable can create an additional bond. */
      branch_score +=
          p_vars->max_weights[p_vars->h_idxs[i]] *
          (int)std::min(p_vars->no_neighbors + 1, p_vars->bond_dists[i].size());
    } else {
      branch_score +=
          p_vars->max_weights[p_vars->h_idxs[i]] *
          (int)std::min(p_vars->no_neighbors, p_vars->bond_dists[i].size());
    }
  }

  protein->remove_amino();

  return cur_score + branch_score >= best_score;
}

/* If a checkpoint location is provided through the environment, attemt to store the checkpoint. */
void try_store_checkpoint(const Protein& protein,
                        const std::stack<int>& dfs_stack,
                        int move,
                        bool placed_amino,
                        int best_score,
                        int score,
                        const std::vector<int>& best_hash,
                        int iterations)
{
    auto cache_dir = get_cache_dir("depth_first_bnb", true);
    if (!cache_dir) {
#ifdef PROSPR_DEBUG_STEPS
        std::cout << "[Debug depth_first_bnb] No cache directory to save checkpoint to." << std::endl;
#endif
      return;
    }
    std::string filename = (*cache_dir / protein.get_sequence().append(".checkpoint")).string();
#ifdef PROSPR_DEBUG_STEPS
      std::cout << "[Debug depth_first_bnb] Writing to checkpoint: " << filename << std::endl;
#endif
    std::ofstream ofs(filename);
    if (!ofs) throw std::runtime_error("Cannot open checkpoint file for writing.");

    ofs << "; prospr checkpoint for sequence " << protein.get_sequence() << "\n";
    ofs << "; Protein state:\n";
    dump_protein_state(protein, ofs);
    ofs << "\n; Algorithm state:\n";
    ofs << "algorithm=depth_first_bnb\n";
    // Serialize stack as comma-separated list
    std::stack<int> temp = dfs_stack;
    std::vector<int> stack_data;
    while (!temp.empty()) {
        stack_data.push_back(temp.top());
        temp.pop();
    }
    std::reverse(stack_data.begin(), stack_data.end());
    ofs << "dfs_stack=";
    for (size_t i = 0; i < stack_data.size(); ++i) {
        if (i != 0) ofs << ",";
        ofs << stack_data[i];
    }
    ofs << "\n";

    ofs << "move=" << move << "\n";
    ofs << "placed_amino=" << placed_amino << "\n";
    ofs << "best_score=" << best_score << "\n";
    ofs << "score=" << score << "\n";

    ofs << "best_hash=";
    for (size_t i = 0; i < best_hash.size(); ++i) {
        if (i != 0) ofs << ",";
        ofs << best_hash[i];
    }
    ofs << "\n";
    ofs << "iterations=" << iterations << "\n";
}

/* If a checkpoint location is provided through the environment, attemt to load the checkpoint. */
void try_load_checkpoint(Protein& protein,
                        std::stack<int>& dfs_stack,
                        int& move,
                        bool& placed_amino,
                        int& best_score,
                        int& score,
                        std::vector<int>& best_hash,
                        int& iterations)
{
    auto cache_dir = get_cache_dir("depth_first_bnb");
    if (!cache_dir) {
#ifdef PROSPR_DEBUG_STEPS
          std::cout << "[Debug depth_first_bnb] No cache directory to load checkpoint from." << std::endl;
#endif
      return;
    }
    std::string filename = (*cache_dir / protein.get_sequence().append(".checkpoint")).string();
    std::filesystem::path path(filename);
    if (!std::filesystem::exists(path)) {
#ifdef PROSPR_DEBUG_STEPS
          std::cout << "[Debug depth_first_bnb] No checkpoint to load:" << filename << std::endl;
#endif
      return;
    }
#ifdef PROSPR_DEBUG_STEPS
      std::cout << "[Debug depth_first_bnb] Reading from checkpoint: " << filename << std::endl;
#endif
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("Cannot open checkpoint file for reading.");

    // Load the protein state
    load_protein_state(protein, ifs);

    // Read the file again for loading the algorithm state
    ifs.clear();
    ifs.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(ifs, line)) {
        std::string key;
        std::string value;
        if (!parse_ini_line(line, key, value)) continue;

        if (key == "dfs_stack") {
            dfs_stack = std::stack<int>(); // clear
            std::vector<int> stack_data;
            std::stringstream ss(value);
            std::string token;
            while (std::getline(ss, token, ',')) {
                stack_data.push_back(std::stoi(token));
            }
            // rebuild stack
            for (int v : stack_data) dfs_stack.push(v);
        } 
        else if (key == "algorithm" && value != "depth_first_bnb") {
#ifdef PROSPR_DEBUG_STEPS
        std::cerr << "[Debug depth_first_bnb] Unexpected value for checkpoint algorithm: " << value << std::endl;
#endif
        }
        else if (key == "move") move = std::stoi(value);
        else if (key == "placed_amino") placed_amino = std::stoi(value);
        else if (key == "best_score") best_score = std::stoi(value);
        else if (key == "score") score = std::stoi(value);
        else if (key == "best_hash") {
            best_hash.clear();
            std::stringstream ss(value);
            std::string token;
            while (std::getline(ss, token, ',')) {
                best_hash.push_back(std::stoi(token));
            }
        }
        else if (key == "iterations") iterations = std::stoi(value);
    }
}


std::atomic<int> caught_signal{0};

/* Function to catch signals (SIGTERM, SIGINT) and store them for delayed handling. */
void signal_handler(int signal) {
    caught_signal.store(signal, std::memory_order_relaxed);
}

/* A depth-first branch-and-bound search function for finding a minimum
 * energy conformation.
 */
void depth_first_bnb(Protein *protein, std::string prune_func) {
  /* Override signal handlers */
  sighandler_t signal_handler_sigint = std::signal(SIGINT, signal_handler);
  sighandler_t signal_handler_sigterm = std::signal(SIGTERM, signal_handler);

  protein->reset_conformation();
  size_t max_length = protein->get_sequence().length();
  int dim = protein->get_dim();
  size_t no_neighbors = (size_t)pow(2, (dim - 1));

  /* The first two amino acids are fixed to prevent y-axis symmetry. */
  if (max_length > 1)
    protein->place_amino(-1);
  if (max_length <= 2)
    return;

  /* Init default prune functions and arguments. */
  auto prune_branch = naive_prune;
  struct prune_vars p_vars = {};
  p_vars.max_length = max_length;
  p_vars.no_neighbors = no_neighbors;
  p_vars.max_weights = protein->get_max_weights();

  /* Determine if to use another prune criteria, and extend prune vars. */
  if (prune_func == "reach_prune") {
    prune_branch = reach_prune;
    std::vector<size_t> cur_dists = {};

    /* Create vector with distances between aminos that can create bonds. */
    for (size_t i = 0; i < max_length; i++) {
      /* Only include indexes that can create bonds. */
      if (p_vars.max_weights[i] != 0) {
        /* Create vector with distances to previous bondable aminos. */
        for (size_t idx : p_vars.h_idxs) {
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

  int signal = 0;
  int iterations = 0;

  try_load_checkpoint(*protein, dfs_stack, move, placed_amino, best_score, score, best_hash, iterations);
#ifdef PROSPR_DEBUG_STEPS
  std::cout << "[Debug depth_first_bnb] Algorithm starting from iteration " << iterations << "." << std::endl;
#endif

  do {
    signal = caught_signal.exchange(0);
    if (signal) break;

    iterations++;
#ifdef PROSPR_DEBUG_STEPS
    std::cout << "[Debug depth_first_bnb] Paused before iteration " << iterations << ". (Press enter to continue!) " << std::flush;
    std::cin.get();
#endif

    placed_amino = false;

    /* Try to place the current amino acid. */
    while (!placed_amino && move != -dim - 1) {
      if (protein->is_valid(move) &&
          !prune_branch(protein, move, best_score, &p_vars)) {
        protein->place_amino(move);
        placed_amino = true;

        /* Push next possible move on stack for backtracking.
         * Skip 0 as it is invalid.
         */
        if (move == 1) {
          dfs_stack.push(-1);
        } else {
          /* Pushes "-dim - 1" as backstop if no possible moves are left. */
          dfs_stack.push(move - 1);
        }
      } else {
        /* Try next move, but skip 0 as it is invalid. */
        if (move == 1) {
          move = -1;
        } else {
          /* Sets "-dim - 1" as backstop if no possible moves are left. */
          move -= 1;
        }
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

  try_store_checkpoint(*protein, dfs_stack, move, placed_amino, best_score, score, best_hash, iterations);
  if (!signal) {
    /* Set best found conformation. */
    protein->set_hash(best_hash);
  }

  /* Restore signal handlers */
  std::signal(SIGINT, signal_handler_sigint);
  std::signal(SIGTERM, signal_handler_sigterm);
  if (signal) std::raise(signal);
}
