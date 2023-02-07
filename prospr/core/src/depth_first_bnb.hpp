/* File:            depth_first_bnb.h
 * Description:     Header file for a depth-first branch-and-bound search
 *                  function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#ifndef DEPTH_FIRST_BNB_H
#define DEPTH_FIRST_BNB_H

#include "protein.hpp"

/* A depth-first branch-and-bound search function for finding a minimum energy
 * conformation.
 */
Protein *depth_first_bnb(Protein *protein, std::string prune_func = "");

#endif
