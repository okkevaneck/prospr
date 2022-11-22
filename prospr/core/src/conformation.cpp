/* File:            conformation.cpp
 * Description:     Source file for the conformation struct.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */
#include "conformation.hpp"
#include <numeric>
#include <algorithm>


/* Overload > operator for Conformation in the priority queue.
 * Bigger indicates more potential for leading towards the minimum energy conformation.
 */
bool operator>(const struct Conformation& conf1,
               const struct Conformation& conf2) {
    return conf1.score > conf2.score ||
        (conf1.score == conf2.score && conf1.length > conf2.length);
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
