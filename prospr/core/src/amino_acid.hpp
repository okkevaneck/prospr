/* File:            amino_acid.h
 * Description:     Header file for an amino acid object within the HP-model.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#ifndef AMINO_ACID_H
#define AMINO_ACID_H

#include <string>

class AminoAcid {
public:
  /* Copy constructor. */
  AminoAcid(const AminoAcid &other);

  /* Construct a new AminoAcid. */
  AminoAcid(char type, int index, int prev_move = 0, int next_move = 0);

  /* Overload assignment operator for copy-assignments. */
  AminoAcid &operator=(const AminoAcid &other);

  /* Returns the AminoAcid's type. */
  char get_type();

  /* Returns the AminoAcid's index. */
  int get_index();

  /* Returns the AminoAcid's previous move. */
  int get_prev_move();

  /* Returns the AminoAcid's next move. */
  int get_next_move();

  /* Sets the AminoAcid's previous move. */
  void set_prev_move(int move);

  /* Sets the AminoAcid's next move. */
  void set_next_move(int move);

private:
  char type;
  int index;
  int prev_move;
  int next_move;
};

/* Overload << operator for printing AminoAcids. */
std::ostream &operator<<(std::ostream &os, AminoAcid &amino_acid);

#endif
