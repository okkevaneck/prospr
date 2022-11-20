/* File:            a_star.hpp
 * Description:     Header file for an a_star search function.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2022). See the LICENSE file for the
 *                  specifics.
 */
#ifndef A_STAR_H
#define A_STAR_H

#include "protein.hpp"


/* An a_star search function for finding a minimum energy conformation. */
Protein* a_star(Protein* protein);

#endif
