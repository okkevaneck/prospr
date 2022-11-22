/* File:            dijkstra.cpp
 * Description:     Source file for Dijkstra's search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */

#include "dijkstra.hpp"
#include <queue>
#include <iostream>
#include <numeric>
#include <algorithm>


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
        /* Construct list of only positive moves. */
        std::vector<int> pos_moves(dim);
        std::iota(pos_moves.begin(), pos_moves.end(), 1);

        /* Check if there are any positive moves in current hash. */
        const auto result = std::find_first_of(this->hash.begin(),
                                       this->hash.end(),
                                       pos_moves.begin(),
                                       pos_moves.end());

        /* Use only negative moves if protein has not left quadrant axes. */
        if (result == this->hash.end()) {
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

        for (int m : moves) {
            /* Only add valid conformations to list of children. */
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
 * Bigger indicates more potential for leading towards the minimum energy conformation.
 */
bool operator>(const struct Conformation& conf1, const struct Conformation& conf2) {
    return conf1.score > conf2.score || (conf1.score == conf2.score && conf1.length > conf2.length);
}

/* Overload << operator for printing Conformations. */
std::ostream &operator<<(std::ostream &os, const Conformation& conf) {
    std::cout << "<" << conf.score << ", " << conf.length << ", [";

    for (int i: conf.hash) {
        std::cout << i << ", ";
    }

    std::cout << "]>";
    return os;
}

/* A breadth-first search function for finding a minimum energy conformation. */
Protein* dijkstra(Protein* protein) {
    size_t max_length = protein->get_sequence().length();

    /* A Protein with 3 or less amino acids cannot make a bond, so return. */
    if (max_length <= 3) {
        protein->place_amino(2);
        protein->place_amino(2);
        return protein;
    }

    /* Make priority queue sorting first on lowest energy, then on smallest length. */
    std::priority_queue<Conformation, std::vector<Conformation>, std::greater<Conformation>> prioq;
    std::vector<Conformation> children;

    /* Add initial partial conformation as only node in priority queue. */
    Conformation conf = Conformation(0, 1, std::vector<int>{});
    prioq.push(conf);

    /* Init best score on impossible value. */
    int best_score = 1;
    Conformation best_conf = Conformation(1, max_length, std::vector<int>(max_length, -protein->get_dim()));

    /* Fetch shortest path while the queue is not empty. */
    while (!prioq.empty()) {
        conf = prioq.top();
        prioq.pop();

        /* Create children of current conformation and loop over them. */
        children = conf.create_children(protein);

        for (Conformation conf : children) {
            std::cout << conf << "\n";

            /* If child is complete conformation, check stability for new best. */
            if (conf.hash.size() == max_length) {
                protein->set_hash(conf.hash);

                if (protein->get_score() < best_score) {
                    best_score = protein->get_score();
                    best_conf = conf;
                }
            } else {
                /* Add child to priority queue if not complete conformation. */
                prioq.push(conf);
            }
        }
    }

    std::cout << "\n\nBest found conformation:\n" << best_conf << "\n";
    protein->set_hash(best_conf.hash);

    return protein;
}
