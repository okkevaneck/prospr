/* File:            test_amino_acid.cpp
 * Description:     Source file for testing the functionality of amino_acid.
 */

#include <iostream>
#include <assert.h>
#include "../src/amino_acid.hpp"


int main(int argc, char* argv[]) {
    /* Test creation of AminoAcid. */
    AminoAcid* new_aa = new AminoAcid('H', 0, 0, 0);

    /* Test getters. */
    char type = new_aa->get_type();
    assert (type == 'H');

    int idx = new_aa->get_index();
    assert (idx == 0);

    int prev_move = new_aa->get_prev_move();
    assert (prev_move == 0);

    int next_move = new_aa->get_next_move();
    assert (next_move == 0);

    /* Test setters. */
    new_aa->set_prev_move(1);
    prev_move = new_aa->get_prev_move();
    assert (prev_move == 1);

    new_aa->set_next_move(1);
    next_move = new_aa->get_next_move();
    assert (next_move == 1);

    return 0;
}
