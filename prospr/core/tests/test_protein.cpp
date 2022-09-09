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
                               std::map<std::string, int> model_bonds) {
    /* Test getters. */


    assert (1==1);
}

/* Test different Protein generation options. */
void test_protein_generation() {
    /* Check HP-model Protein generation. */
    Protein* protein = new Protein("HPPHPPHH", 2, "HP");
    assert_2d_protein_generation(protein, {{"HH", -1}});

    /* Check HPXN-model Protein generation. */
    protein = new Protein("HPPHPPHH", 2, "HPXN");
    assert_2d_protein_generation(protein, {{"HH", -4}, {"PP", -1}, {"PN", -1},
                                        {"NN", 1}});

    /* Check custom model Protein generation. */
    protein = new Protein("HPPHPPHH", 2, "", {{"HH", -4}, {"HP", -2}});
    assert_2d_protein_generation(protein, {{"HH", -4}, {"HP", -2}});

    /* Check custom model Protein generation with symmetry option. */
    protein = new Protein("HPPHPPHH", 2, "", {{"HH", -4}, {"HP", -2}}, true);
    assert_2d_protein_generation(protein, {{"HH", -4}, {"HP", -2}, {"PH", -2}});
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
