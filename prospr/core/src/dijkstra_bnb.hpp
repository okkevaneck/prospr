/* File:            dijsktra_bnb.hpp
 * Description:     Header file for Dijkstra's search function with
 *                  branch-and-bound.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */
#ifndef DIJKSTRA_BNB_H
#define DIJKSTRA_BNB_H

#include "protein.hpp"


/* Dijkstra's search function with branch-and-bound for finding a minimum
 * energy conformation.
 */
Protein* dijkstra_bnb(Protein* protein);

#endif
