/* File:            test_amino_acid.cpp
 * Description:     Source file for testing the functionality of amino_acid.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include <assert.h>

#include <iostream>

#include "../src/amino_acid.hpp"

int main(void) {
  /* Test creation of AminoAcid. */
  AminoAcid *new_aa = new AminoAcid('H', 0, 0, 0);
  std::cout << "\tAminoAcid creation successful.\n";

  /* Test getters. */
  char type = new_aa->get_type();
  assert(type == 'H');

  int idx = new_aa->get_index();
  assert(idx == 0);

  int prev_move = new_aa->get_prev_move();
  assert(prev_move == 0);

  int next_move = new_aa->get_next_move();
  assert(next_move == 0);
  std::cout << "\tAminoAcid getters check successful.\n";

  /* Test setters. */
  new_aa->set_prev_move(1);
  prev_move = new_aa->get_prev_move();
  assert(prev_move == 1);

  new_aa->set_next_move(1);
  next_move = new_aa->get_next_move();
  assert(next_move == 1);
  std::cout << "\tAminoAcid setters check successful.\n";

  delete new_aa;
  return 0;
}
