/* File:            breadth_first.cpp
 * Description:     Source file for a breadth-first search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */

#include "breadth_first.hpp"
#include <queue>


///* Overloading < operator for Protein objects. */
//bool operator< (const Protein &protein1, const Protein &protein2) {
//    return protein1.score < protein2.score ||
//        (protein1.score == protein2.score && protein1.cur_len > protein2.cur_len);
//}
//
///* Overloading <= operator for Protein objects. */
//bool operator<= (const Protein &protein1, const Protein &protein2) {
//    return protein1.score <= protein2.score ||
//        (protein1.score == protein2.score && protein1.cur_len >= protein2.cur_len);
//}
//
///* Overloading > operator for Protein objects. */
//bool operator> (const Protein &protein1, const Protein &protein2) {
//    return protein1.score > protein2.score ||
//        (protein1.score == protein2.score && protein1.cur_len < protein2.cur_len);
//}
//
///* Overloading >= operator for Protein objects. */
//bool operator>= (const Protein &protein1, const Protein &protein2) {
//    return protein1.score >= protein2.score ||
//        (protein1.score == protein2.score && protein1.cur_len <= protein2.cur_len);
//}


/* A breadth-first search function for finding a minimum energy conformation. */
Protein* breadth_first(Protein* protein) {
    // TODO: Sort criteria: (energy, nodes_placed) -> equal e, shortest length first
    std::priority_queue<Protein, std::vector<Protein>, std::greater<Protein>> prioq;

//    Protein

    // Queue entry: <score, len, hash> --> sort by lower

}
