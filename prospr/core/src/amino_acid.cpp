/* File:            amino_acid.cpp
 * Description:     Source file for an amino acid object within the HP-model.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "amino_acid.hpp"


/* Construct a new AminoAcid. */
AminoAcid::AminoAcid(char type, int index, int prev_move, int next_move) {
    this->type = type;
    this->index = index;
    this->prev_move = prev_move;
    this->next_move = next_move;
}

/* Returns the AminoAcid's type. */
char AminoAcid::get_type() {
    return type;
}

/* Returns the AminoAcid's index. */
int AminoAcid::get_index() {
    return index;
}

/* Returns the AminoAcid's previous move. */
int AminoAcid::get_prev_move() {
    return prev_move;
}

/* Returns the AminoAcid's next move. */
int AminoAcid::get_next_move() {
    return next_move;
}

/* Sets the AminoAcid's previous move. */
void AminoAcid::set_prev_move(int move) {
    prev_move = move;
}

/* Sets the AminoAcid's next move. */
void AminoAcid::set_next_move(int move) {
    next_move = move;
}
