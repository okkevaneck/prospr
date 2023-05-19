/* File:            beam_search.h
 * Description:     Header file for a beam search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#ifndef BEAM_SEARCH_H
#define BEAM_SEARCH_H

#include "protein.hpp"

/* A beam search function for finding a minimum energy conformation. */
Protein *beam_search(Protein *protein, int beam_width = -1);

#endif
