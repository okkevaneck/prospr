/* File:            amino_acid.h
 * Description:     Header file for an amino acid object within the HP-model.
 */

#ifndef AMINO_ACID_H
#define AMINO_ACID_H

#include <string.h>


class AminoAcid {
    public:
        /* Construct a new AminoAcid. */
        AminoAcid(std::string type, int index, int prev_move, int next_move);

        /* Returns the AminoAcid's type. */
        std::string get_type();

        /* Returns the AminoAcid's index. */
        int get_index();

        /* Returns the AminoAcid's previous move. */
        int get_prev_move();

        /* Returns the AminoAcid's next move. */
        int get_next_move();

    private:
        std::string type;
        int index;
        int prev_move;
        int next_move;
};

#endif
