/* File:            amino_acid.h
 * Description:     Header file for an amino acid object within the HP-model.
 */

#ifndef AMINO_ACID_H
#define AMINO_ACID_H

#include <string.h>


class AminoAcid {
    public:
        AminoAcid(std::string type, int bond_value, int index, int prev_move,
                    int next_move);

    private:
        std::string type;
        int bond_value;
        int index;
        int prev_move;
        int next_move;
};

#endif
