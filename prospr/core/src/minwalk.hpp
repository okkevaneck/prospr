/* File:            minwalk.hpp
 * Description:     Header file for the MINWALK algorithm.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#ifndef MINWALK_H
#define MINWALK_H

#include "protein.hpp"


/* MINWALK algorithm for finding a minimum energy conformation. */
Protein* minwalk(Protein* protein);

#endif
