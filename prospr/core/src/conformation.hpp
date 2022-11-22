/* File:            conformation.hpp
 * Description:     Header file for the conformation struct.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */
#ifndef CONFORMATION_h
#define CONFORMATION_h

#include "protein.hpp"
#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>


/* Definition for a partial fold in the priority queue. */
struct Conformation {
    int score;
    int length;
    std::vector<int> hash;

    /* Conformation constructor. */
    Conformation(int score, int length, std::vector<int> hash) {
        this->score = score;
        this->length = length;
        this->hash = hash;
    }

    /* Create set of moves depending on current hash. */
    std::vector<int> _create_move_set(int dim) {
        /* Construct list of moves, limit first moves to one quadrant. */
        std::vector<int> one_dim (this->hash.size(), -1);

        /* Limit directions to -1 and -2 until a bend at -2 has been made. */
        auto goes_down = std::find(this->hash.begin(), this->hash.end(), -2);
        if (this->hash == one_dim || goes_down == this->hash.end()) {
            std::vector<int> neg_moves(dim);
            std::iota(neg_moves.begin(), neg_moves.end(), -dim);
            return neg_moves;
        } else {
            /* Else, create list of all possible moves. */
            std::vector<int> all_moves(dim * 2 + 1);
            std::iota(all_moves.begin(), all_moves.end(), -dim);
            all_moves.erase(all_moves.begin() + dim);
            return all_moves;
        }
    }

    /* Create and return vector of children Conformations. */
    std::vector<Conformation> create_children(Protein* protein) {
        /* Set hash to check for move validity later on. */
        std::vector<Conformation> children;
        protein->set_hash(this->hash);

        /* Construct list of possible moves. Restrict to negative quadrant for
         * first placements to prevent symmetry.
         */
        std::vector<int> moves = this->_create_move_set(protein->get_dim());

        /* Only add valid conformations to list of children. */
        for (int m : moves) {
            if (protein->is_valid(m)) {
                protein->place_amino(m);

                Conformation conf = Conformation(protein->get_score(),
                                                 protein->get_cur_len(),
                                                 protein->hash_fold());
                children.push_back(conf);

                protein->remove_amino();
            }
        }

        return children;
    }
};

/* Overload > operator for Conformation in the priority queue.
 * Bigger indicates more potential for leading towards the minimum energy
 * conformation.
 */
bool operator>(const struct Conformation& conf1,
               const struct Conformation& conf2);

/* Overload << operator for printing Conformations. */
std::ostream &operator<<(std::ostream &os, const Conformation& conf);

#endif
