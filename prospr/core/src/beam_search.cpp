/* File:            beam_search.h
 * Description:     Source file for a beam search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "beam_search.hpp"
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
#include <numeric>
#include <cstring>


/* Type for ordering proteins in a priority queue.  */
struct PrioProtein {
    Protein protein;
    int score;
};

/* Overload > operator for Conformation in the priority queue.
 * Bigger indicates more potential for leading towards the minimum energy
 * conformation.
 */
bool operator<(const struct PrioProtein& prot1,
               const struct PrioProtein& prot2) {
    return prot1.score < prot2.score;
}

/* Overload << operator for printing PrioProteins. */
std::ostream &operator<<(std::ostream &os, struct PrioProtein& prot) {
    std::cout << "<" << prot.score << ", [";

    for (int i: prot.protein.hash_fold()) {
        std::cout << i << " ";
    }

    std::cout << "]>";
    return os;
}

/* Compute heuristic score for protein to use in priority queue. */
int comp_score(Protein* protein) {
    return 0;
}

/* A beam search function for finding a minimum energy conformation. */
Protein* beam_search(Protein* protein, int beam_width) {
    int max_length = protein->get_sequence().length();
    int dim = protein->get_dim();

    /* The first two amino acids are fixed to prevent y-axis symmetry. */
    if (max_length > 1)
        protein->place_amino(-1);
    if (max_length <= 2)
        return protein;

    /* Create vector for current proteins, and a priority queue to filter. */
    std::vector<PrioProtein> beam;
    std::priority_queue<PrioProtein,
                        std::vector<PrioProtein>,
                        std::less<PrioProtein>> cur_proteins;

    /* Create vector with all moves. */
    std::vector<int> all_moves(dim * 2 + 1);
    std::iota(all_moves.begin(), all_moves.end(), -dim);
    all_moves.erase(all_moves.begin() + dim);

    /* Loop over proteins in beam until proteins are fully folded. */
    int num_elements = beam_width;
    PrioProtein cur_prioprot = {*protein, comp_score(protein)};
    beam.push_back(cur_prioprot);
    Protein cur_protein, cur_expansion;

    while (beam[0].protein.get_cur_len() != max_length) {
        /* Expand all proteins in the beam and add to priority queue. */
        for (PrioProtein prio_prot : beam) {
            cur_protein = prio_prot.protein;

            for (int m : all_moves) {
                if (cur_protein.is_valid(m)) {
                    cur_expansion = *protein;
//                    std::memcpy(&cur_expansion, protein, sizeof(Protein));
                    cur_expansion.place_amino(m);
                    cur_prioprot = {cur_expansion, comp_score(&cur_expansion)};
                    cur_proteins.push(cur_prioprot);
                }
            }
        }

        /* Interpret beam_width of -1 as all elements. */
        if (beam_width == -1) {
            num_elements = cur_proteins.size();
            std::cout << "Num elements " << num_elements << "\n";
        }

        /* Update beam with highest ranked proteins and clear priority queue. */
        beam.clear();
        for (int i = 0; i < num_elements; i++) {
            beam.push_back(cur_proteins.top());
            cur_proteins.pop();
        }
        cur_proteins.empty();
    }

    std::memcpy(protein, &beam[0].protein, sizeof(Protein));
    std::cout << "Best score: " << protein->get_score() << "\n";

    /* First protein in priority queue will have highest score. */
    return protein;
}
