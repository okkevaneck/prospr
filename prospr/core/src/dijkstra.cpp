/* File:            dijkstra.cpp
 * Description:     Source file for Dijkstra's search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */

#include "dijkstra.hpp"
#include <queue>
#include <iostream>


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

    /* Create and return vector of children Conformations. */
    std::vector<Conformation> create_children(Protein* protein) {
        std::vector<Conformation> children;
        std::vector<int> moves = {-2, -1, 1, 2};
        protein->set_hash(this->hash);

        for (int m : moves) {
            /* Only add valid conformations to list of children. */
            if (protein->is_valid(m)) {
                protein->place_amino(m);

                Conformation conf = Conformation(protein->get_score(), protein->get_cur_len(), protein->hash_fold());
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
    /* Make priority queue and set initial conformation as only node. */
    std::priority_queue<Conformation, std::vector<Conformation>, std::greater<Conformation>> prioq;
    Conformation conf = Conformation(0, 1, std::vector<int>{});
    prioq.push(conf);

    /* Fetch shortest path while the queue is not empty. */
    while (!prioq.empty()) {
        conf = prioq.top();
        prioq.pop();

        /* Create children of current conformation and add to queue. */
        std::vector<Conformation> children = conf.create_children(protein);
        for (Conformation c : children) {
            std::cout << c << "\n";
        }
    }



//    prioq.push(Conformation(-1, 2, std::vector<int>{1,1,2,2,2}));
//    prioq.push(Conformation(-1, 3, std::vector<int>{1,1,-1,2,2}));
//    prioq.push(Conformation(-1, 1, std::vector<int>{1,2,2,2,2}));
//    prioq.push(Conformation(-2, 2, std::vector<int>{1,-1,2,2,2}));
//    prioq.push(Conformation(-1, 3, std::vector<int>{1,1,-1,2,2}));

//    while (! prioq.empty() ) {
//        std::cout << prioq.top() << "\n";
//        prioq.pop();
//    }

    return protein;
}
