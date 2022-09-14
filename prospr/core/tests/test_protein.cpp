/* File:            test_protein.cpp
 * Description:     Source file for testing the functionality of protein.
 *                  Expects that the following modules work:
 *                      - amino_acid.
 */

#include <iostream>
#include <assert.h>
#include "../src/protein.hpp"


/* Perform assertion checks for newly generated Protein. */
void assert_2d_protein_generation(Protein* protein,
                                  std::map<std::string, int> model_bonds,
                                  std::vector<int> max_weights) {
    /* Test getters without arguments. */
    std::string sequence = protein->get_sequence();
    assert (sequence.compare("HPPHPPHH") == 0);

    int dim = protein->get_dim();
    assert (dim == 2);

    std::map<std::string, int> bond_values = protein->get_bond_values();
    assert (bond_values == model_bonds);

    int length = protein->get_cur_len();
    assert (length == 1);

    int last_move = protein->get_last_move();
    assert (last_move == 0);

    std::vector<int> last_pos = protein->get_last_pos();
    assert (last_pos == std::vector<int>(dim, 0));

    int score = protein->get_score();
    assert (score == 0);

    int changes = protein->get_changes();
    assert (changes == 0);

    std::vector<int> mweights = protein->get_max_weights();
    assert (mweights == max_weights);




//    /* Returns the AminoAcid at the given position, or NULL if there is
//     * none.
//     */
//    AminoAcid* get_amino(std::vector<int> position);
//
//    /* Returns if the amino acid at the given index is weighted. */
//    bool is_weighted(int index);
//
//    /* Returns the weight created between two amino acids. */
//    int get_weight(std::string aminos);
}

/* Test different Protein generation options. */
void test_protein_generation() {
    /* Check HP-model Protein generation. */
    Protein* protein = new Protein("HPPHPPHH", 2, "HP");
    assert_2d_protein_generation(protein, {{"HH", -1}},
                                 std::vector<int>{-1, 0, 0, -1, 0, 0, -1, -1});
    std::cout << "\tHP-model generation successful.\n";

    /* Check HPXN-model Protein generation. */
    protein = new Protein("HPPHPPHH", 2, "HPXN");
    assert_2d_protein_generation(protein,
                                 {{"HH", -4}, {"PP", -1}, {"PN", -1},
                                  {"NN", 1}},
                                 std::vector<int>{-4, -1, -1, -4, -1, -1, -4,
                                                  -4});
    std::cout << "\tHPXN-model generation successful.\n";

    /* Check custom model Protein generation. */
    protein = new Protein("HPPHPPHH", 2, "", {{"HH", -4}, {"HP", -2}});
    assert_2d_protein_generation(protein,
                                 {{"HH", -4}, {"HP", -2}, {"PH", -2}},
                                 std::vector<int>{-4, -2, -2, -4, -2, -2, -4,
                                                  -4});
    std::cout << "\tCustom model generation successful.\n";

    /* Check custom model Protein generation with symmetry option. */
    protein = new Protein("HPPHPPHH", 2, "", {{"HH", -4}, {"HP", -2}}, false);
    assert_2d_protein_generation(protein, {{"HH", -4}, {"HP", -2}},
                                 std::vector<int>{-4, -2, -2, -4, -2, -2, -4,
                                                  -4});
    std::cout << "\tCustom non-symmetry model generation successful.\n";
}

/* Test Protein functionality in 2D space. */
void test_2d_protein() {
    Protein* protein = new Protein("HPPHPPHH", 2, "HP");
}

/* Test Protein functionality in 3D space. */
void test_3d_protein() {
    Protein* protein = new Protein("HPPHPPHH", 3, "HP");
}

int main(int argc, char* argv[]) {
    /* Test different Protein generation options. */
    test_protein_generation();

    /* Test Protein in a 2D space. */
    test_2d_protein();

    /* Test Protein in a 3D space. */
    test_3d_protein();

    return 0;
}
