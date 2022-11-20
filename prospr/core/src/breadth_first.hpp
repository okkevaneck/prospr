/* File:            breadth_first.hpp
 * Description:     Header file for a breadth-first search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */
#ifndef BREADTH_FIRST_H
#define BREADTH_FIRST_H

#include "protein.hpp"


/* An a_star search function for finding a minimum energy conformation. */
Protein* breadth_first(Protein* protein);

#endif
