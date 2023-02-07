/* File:            depth_first.h
 * Description:     Header file for a depth-first search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#ifndef DEPTH_FIRST_H
#define DEPTH_FIRST_H

#include "protein.hpp"

/* A depth-first search function for finding a minimum energy conformation. */
Protein *depth_first(Protein *protein);

#endif
