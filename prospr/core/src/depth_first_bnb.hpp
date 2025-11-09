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
void depth_first_bnb(Protein *protein, std::string prune_func = "", bool is_pre_folded = false);

/* A depth-first branch-and-bound search function for finding a minimum energy
 * conformation using OpenMP to explore multiple subtrees in parallel.
 */
void depth_first_bnb_parallel(Protein *protein, std::string prune_func = "", float work_ratio = 3);

#endif
