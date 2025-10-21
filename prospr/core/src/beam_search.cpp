/* File:            beam_search.h
 * Description:     Source file for a beam search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "beam_search.hpp"

#include <math.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

/* Type for storing info on bondable amino acids.  */
struct BondInfo {
  size_t max_length;
  size_t no_neighbors;
  std::vector<int> max_weights;
  size_t num_idxs;
  std::vector<size_t> h_idxs;
  std::vector<std::vector<size_t>> bond_dists;
};

/* Type for ordering proteins in a priority queue.  */
struct PrioProtein {
  Protein *protein;
  int score;

  /* Overload > operator for Conformation in the priority queue.
   * Bigger indicates more potential for leading towards the minimum energy
   * conformation.
   */
  bool operator>(const PrioProtein &rhs) const { return score > rhs.score; }
};

/* Overload << operator for printing PrioProteins. */
std::ostream &operator<<(std::ostream &os, PrioProtein &prot) {
  std::cout << "<" << prot.score << ", [";

  for (int i : prot.protein->hash_fold()) {
    std::cout << i << " ";
  }

  std::cout << "]>";
  return os;
}

/* Compute how amino acids can possibly form bonds. */
BondInfo _comp_bondable_aminos(Protein *protein) {
  /* Fetch protein specific information. */
  size_t max_length = protein->get_sequence().length();
  size_t no_neighbors = (size_t)pow(2, (protein->get_dim() - 1));
  std::vector<int> max_weights = protein->get_max_weights();
  std::vector<size_t> h_idxs = {};
  std::vector<std::vector<size_t>> bond_dists = {};

  /* Create vector with distances between aminos that can create bonds. */
  std::vector<size_t> cur_dists = {};

  for (size_t i = 0; i < max_length; i++) {
    /* Only include indexes that can create bonds. */
    if (max_weights[i] != 0) {
      /* Create vector with distances to previous bondable aminos. */
      for (size_t idx : h_idxs) {
        if (i - idx >= 3 && (i - idx) % 2 == 1) {
          cur_dists.push_back(i - idx);
        }
      }

      /* Add distance vector to set and clear for next iterations.  */
      bond_dists.push_back(cur_dists);
      cur_dists.clear();

      /* Add current index for next iterations. */
      h_idxs.push_back(i);
    }
  }

  /* Store info on possible bonds in BondInfo struct and return. */
  BondInfo binfo = {max_length,    no_neighbors, max_weights,
                    h_idxs.size(), h_idxs,       bond_dists};
  return binfo;
}

/* Compute heuristic score for protein to use in priority queue. */
int comp_score(Protein *protein, BondInfo *binfo) {
  /* Compute to be placed aminos possibly making bonds. */
  int future_aminos = 0;
  for (auto h_idx : binfo->h_idxs) {
    if (h_idx >= protein->get_cur_len()) {
      future_aminos++;
    }
  }

  /* Compute branch score with the to be placed amino acids. */
  int branch_score = 0;
  for (size_t i = binfo->num_idxs - future_aminos; i < binfo->num_idxs; i++) {
    /* Check if bondable amino is last of protein. */
    if (binfo->h_idxs[i] == binfo->max_length - 1) {
      /* The last amino being bondable can create an additional bond. */
      branch_score +=
          binfo->max_weights[binfo->h_idxs[i]] *
          (int)std::min(binfo->no_neighbors + 1, binfo->bond_dists[i].size());
    } else {
      branch_score +=
          binfo->max_weights[binfo->h_idxs[i]] *
          (int)std::min(binfo->no_neighbors, binfo->bond_dists[i].size());
    }
  }

  return protein->get_score() + branch_score;
}

/* A beam search function for finding a minimum energy conformation. */
void beam_search(Protein *protein, int beam_width) {
  protein->reset_conformation();
  size_t max_length = protein->get_sequence().length();
  int dim = protein->get_dim();

  /* The first two amino acids are fixed to prevent y-axis symmetry. */
  if (max_length > 1)
    protein->place_amino(-1);
  if (max_length <= 2)
    return;

  /* Return if given beam_width is not -1 or positive. */
  if (beam_width < 1 && beam_width != -1) {
    return;
  }

  /* Create vector for current proteins, and a priority queue to filter. */
  std::vector<PrioProtein> beam = {};
  std::priority_queue<PrioProtein, std::vector<PrioProtein>,
                      std::greater<PrioProtein>>
      cur_proteins = {};

  /* Create vector with all moves. */
  std::vector<int> all_moves(dim * 2 + 1);
  std::iota(all_moves.begin(), all_moves.end(), -dim);
  all_moves.erase(all_moves.begin() + dim);

  /* Make a copy of the initial protein. */
  Protein *cur_protein = new Protein(*protein);

  /* Compute future bondable connections for heuristic scoring. */
  BondInfo binfo = _comp_bondable_aminos(protein);

  /* Loop over proteins in beam until proteins are fully folded. */
  PrioProtein cur_prioprot = {cur_protein, comp_score(cur_protein, &binfo)};
  beam.push_back(cur_prioprot);
  Protein *cur_expansion = NULL;
  size_t num_elements;

  while (beam[0].protein->get_cur_len() != max_length) {
    /* Expand all proteins in the beam and add to priority queue. */
    for (PrioProtein prio_prot : beam) {
      cur_protein = prio_prot.protein;

      for (int m : all_moves) {
        if (cur_protein->is_valid(m)) {
          cur_expansion = new Protein(*cur_protein);
          cur_expansion->place_amino(m);
          cur_prioprot = {cur_expansion, comp_score(cur_expansion, &binfo)};
          cur_proteins.push(cur_prioprot);
        }
      }
    }

    /* Interpret beam_width of -1 as all elements.
     * Check for beam_width values below -1 or 0 is done above.
     */
    if (beam_width == -1) {
      num_elements = cur_proteins.size();
    } else {
      num_elements = std::min(cur_proteins.size(), (size_t)beam_width);
    }

    /* Clear beam for next iteration. */
    for (PrioProtein prioprot : beam) {
      delete prioprot.protein;
    }
    beam.clear();

    /* Update beam with highest ranked proteins. */
    for (size_t i = 0; i < num_elements; i++) {
      beam.push_back(cur_proteins.top());
      cur_proteins.pop();
    }

    /* Delete old left over priority queue items. */
    while (!cur_proteins.empty()) {
      delete cur_proteins.top().protein;
      cur_proteins.pop();
    }
  }

  /* First protein in priority queue will have highest score. */
  protein->set_hash(beam[0].protein->hash_fold(), true);

  /* Deallocate old left over allocated beam proteins. */
  for (PrioProtein prioprot : beam) {
    delete prioprot.protein;
  }
  beam.clear();
}
