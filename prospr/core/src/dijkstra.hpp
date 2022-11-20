/* File:            dijsktra.hpp
 * Description:     Header file for Dijkstra's search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "protein.hpp"


/* Dijkstra's search function for finding a minimum energy conformation. */
Protein* dijkstra(Protein* protein);

#endif
