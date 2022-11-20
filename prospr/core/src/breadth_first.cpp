/* File:            breadth_first.cpp
 * Description:     Source file for a breadth-first search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */

#include "breadth_first.hpp"
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
};


/* Overload > operator for Conformation in the priority queue. */
bool operator>(const struct Conformation& conf1, const struct Conformation& conf2) {
    return conf1.score > conf2.score || (conf1.score == conf2.score && conf1.length < conf2.length);
}


/* Overload << operator for printing Conformations. */
std::ostream &operator<<(std::ostream &os, const Conformation& conf) {
    std::cout << "<" << conf.score << ", " << conf.length << ", [";

    for (int i: conf.hash)
        std::cout << i << ", ";

    std::cout << "]>";
    return os;
}


/* A breadth-first search function for finding a minimum energy conformation. */
Protein* breadth_first(Protein* protein) {
    std::priority_queue<Conformation, std::vector<Conformation>, std::greater<Conformation>> prioq;

    prioq.push(Conformation(1, 2, std::vector<int>{1,1,2,2,2}));
    prioq.push(Conformation(1, 3, std::vector<int>{1,1,-1,2,2}));
    prioq.push(Conformation(1, 1, std::vector<int>{1,2,2,2,2}));
    prioq.push(Conformation(2, 2, std::vector<int>{1,-1,2,2,2}));
    prioq.push(Conformation(1, 3, std::vector<int>{1,1,-1,2,2}));

    while (! prioq.empty() ) {
        std::cout << prioq.top() << "\n";
        prioq.pop();
    }

    return protein;
}
