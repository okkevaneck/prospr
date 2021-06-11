/* File:            amino_acid.cpp
 * Description:     Source file for an amino acid object within the HP-model.
 */

#include "amino_acid.hpp"


/* Construct a new AminoAcid. */
AminoAcid::AminoAcid(std::string type, int index, int prev_move,
        int next_move) {
    this->type = type;
    this->index = index;
    this->prev_move = prev_move;
    this->next_move = next_move;
}

/* Returns the AminoAcid's type. */
std::string AminoAcid::get_type() {
    return this->type;
}

/* Returns the AminoAcid's index. */
int AminoAcid::get_index() {
    return this->index;
}

/* Returns the AminoAcid's previous move. */
int AminoAcid::get_prev_move() {
    return this->prev_move;
}

/* Returns the AminoAcid's next move. */
int AminoAcid::get_next_move() {
    return this->next_move;
}
