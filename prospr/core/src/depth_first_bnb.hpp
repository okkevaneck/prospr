/* File:            depth_first_bnb.h
 * Description:     Header file for a depth-first branch-and-bound search
 *                      function.
 */

#ifndef DEPTH_FIRST_BNB_H
#define DEPTH_FIRST_BNB_H

#include "protein.hpp"


/* A depth-first branch-and-bound search function for finding a minimum energy
 * conformation.
 */
Protein* depth_first_bnb(Protein* protein);

#endif
