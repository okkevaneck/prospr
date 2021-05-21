/* File:            amino_acid.cpp
 * Description:     Source file for an amino acid object within the HP-model.
 */

#include "amino_acid.hpp"


AminoAcid::AminoAcid(std::string type, int bond_value, int index, int prev_move,
                     int next_move) {
    this->type = type;
    this->bond_value = bond_value;
    this->index = index;
    this->prev_move = prev_move;
    this->next_move = next_move;
}
