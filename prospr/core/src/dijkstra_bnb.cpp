/* File:            dijkstra_bnb.cpp
 * Description:     Source file for Dijkstra's search function with
 *                  branch-and-bound.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */

#include "dijkstra_bnb.hpp"
#include "conformation.hpp"
#include <queue>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <math.h>


/* Returns true if the branch cannot produce a better score. */
bool dijkstra_prune_branch(Protein* protein, std::vector<int> hash,
        int max_length, int no_neighbors, int best_score) {
    int cur_len = hash.size() + 1;
    std::vector<int> max_weights = protein->get_max_weights();

    /* Compute the sum of the remaining possible scoring connections. */
    int branch_score = no_neighbors * \
        std::accumulate(max_weights.begin() + cur_len, max_weights.end(), 0);

    /* End of amino has 1 more possible way of connecting, update
     * branch_score accordingly.
     */
    if (cur_len != max_length && max_weights.back() != 0)
        branch_score += max_weights.back();

    return protein->get_score() + branch_score >= best_score;
}


/* Dijkstra's search function with branch-and-bound for finding a minimum
 * energy conformation.
 */
Protein* dijkstra_bnb(Protein* protein) {
    size_t max_length = protein->get_sequence().length();
    size_t no_neighbors = (int)pow(2, (protein->get_dim() - 1));

    /* A Protein with 3 or less amino acids cannot make a bond, so return. */
    if (max_length <= 3) { // TODO: Extend with sub cases.
        protein->place_amino(-1);
        protein->place_amino(-1);
        return protein;
    }

    /* Make priority queue, sorting on lowest energy, then on length. */
    std::priority_queue<Conformation,
                        std::vector<Conformation>,
                        std::greater<Conformation>> prioq;
    std::vector<Conformation> children;

    /* Add initial partial conformation as only node in priority queue. */
    protein->place_amino(-1);
    Conformation conf = Conformation(0, 2, std::vector<int>{-1});
    prioq.push(conf);

    /* Init best solution on impossible scenario. */
    Conformation best_conf = Conformation(1, max_length,
            std::vector<int>(max_length, -protein->get_dim()));
    int best_score = 1;

    /* Fetch shortest path while the queue is not empty. */
    while (!prioq.empty()) {
        conf = prioq.top();
        prioq.pop();

        /* Create children of current conformation and loop over them. */
        children = conf.create_children(protein);

        for (Conformation conf : children) {
            /* If child is complete conformation, check for new best. */
            if (conf.hash.size() == max_length - 1) {
                protein->set_hash(conf.hash);

                if (protein->get_score() < best_score) {
                    best_score = protein->get_score();
                    best_conf = conf;
                }
            } else {
                /* Add child to priority queue if not complete conformation,
                 * and it has the potential of being the new best.
                 */
                if (!dijkstra_prune_branch(protein, conf.hash, max_length,
                                           no_neighbors, best_score)) {
                    prioq.push(conf);
                }
            }
        }
    }

    protein->set_hash(best_conf.hash);

    return protein;
}
