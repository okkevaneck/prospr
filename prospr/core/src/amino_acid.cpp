/* File:            amino_acid.cpp
 * Description:     Source file for an amino acid object within the HP-model.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "amino_acid.hpp"

#include <iostream>

/* Copy constructor. */
AminoAcid::AminoAcid(const AminoAcid &other) {
  this->type = other.type;
  this->index = other.index;
  this->prev_move = other.prev_move;
  this->next_move = other.next_move;
}

/* Construct a new AminoAcid. */
AminoAcid::AminoAcid(char type, int index, int prev_move, int next_move) {
  this->type = type;
  this->index = index;
  this->prev_move = prev_move;
  this->next_move = next_move;
}

/* Overload assignment operator for copy-assignments. */
AminoAcid &AminoAcid::operator=(const AminoAcid &other) {
  this->type = other.type;
  this->index = other.index;
  this->prev_move = other.prev_move;
  this->next_move = other.next_move;

  return *this;
}

/* Returns the AminoAcid's type. */
char AminoAcid::get_type() { return type; }

/* Returns the AminoAcid's index. */
int AminoAcid::get_index() { return index; }

/* Returns the AminoAcid's previous move. */
int AminoAcid::get_prev_move() { return prev_move; }

/* Returns the AminoAcid's next move. */
int AminoAcid::get_next_move() { return next_move; }

/* Sets the AminoAcid's previous move. */
void AminoAcid::set_prev_move(int move) { prev_move = move; }

/* Sets the AminoAcid's next move. */
void AminoAcid::set_next_move(int move) { next_move = move; }

/* Overload << operator for printing AminoAcids. */
std::ostream &operator<<(std::ostream &os, AminoAcid &amino_acid) {
  std::cout << "<AminoAcid '" << amino_acid.get_type()
            << "' idx=" << amino_acid.get_index() << " moves=["
            << amino_acid.get_prev_move() << "," << amino_acid.get_next_move()
            << "]>" << std::flush;
  return os;
}
