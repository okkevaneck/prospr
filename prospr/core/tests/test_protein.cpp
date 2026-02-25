/* File:            test_protein.cpp
 * Description:     Source file for testing the functionality of protein.
 *                  Expects that the following modules work:
 *                      - amino_acid.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include <assert.h>

#include <iostream>

#include "../src/protein.hpp"

/* Perform assertion checks for newly generated Protein. */
void assert_2d_protein_generation(Protein *protein,
                                  std::map<std::string, int> model_bonds,
                                  std::vector<int> max_weights) {
  /* Test getters without arguments. */
  std::string sequence = protein->get_sequence();
  assert(sequence.compare("HPPHPPHH") == 0);

  int dim = protein->get_dim();
  assert(dim == 2);

  std::map<std::string, int> bond_values = protein->get_bond_values();
  assert(bond_values == model_bonds);

  assert(protein->get_cur_len() == 1);

  assert(protein->get_last_move() == 0);

  assert(protein->get_last_pos() == std::vector<int>(dim, 0));

  assert(protein->get_score() == 0);

  assert(protein->get_solutions_checked() == 0);

  assert(protein->get_max_weights() == max_weights);

  /* Test getters with arguments. */
  bool weighted = protein->is_weighted(0);
  assert(weighted == true);

  /* Only test for non-weighted P value in case of HP-model tests. */
  std::map<std::string, int> hp_model_bonds = {{"HH", -1}};
  if (bond_values == hp_model_bonds) {
    weighted = protein->is_weighted(1);
    assert(weighted == false);
  }

  for (const auto &element : model_bonds) {
    assert(protein->get_weight(element.first) == element.second);
  }
}

/* Test different Protein generation options. */
void test_protein_generation() {
  /* Check HP-model Protein generation. */
  Protein *protein = new Protein("HPPHPPHH", 2, "HP");
  assert_2d_protein_generation(protein, {{"HH", -1}},
                               std::vector<int>{-1, 0, 0, -1, 0, 0, -1, -1});
  delete protein;
  std::cout << "\tHP-model generation successful.\n";

  /* Check HPXN-model Protein generation. */
  protein = new Protein("HPPHPPHH", 2, "HPXN");
  assert_2d_protein_generation(
      protein, {{"HH", -4}, {"PP", -1}, {"PN", -1}, {"NN", 1}},
      std::vector<int>{-4, -1, -1, -4, -1, -1, -4, -4});
  delete protein;
  std::cout << "\tHPXN-model generation successful.\n";

  /* Check custom model Protein generation. */
  protein = new Protein("HPPHPPHH", 2, "", {{"HH", -4}, {"HP", -2}});
  assert_2d_protein_generation(
      protein, {{"HH", -4}, {"HP", -2}, {"PH", -2}},
      std::vector<int>{-4, -2, -2, -4, -2, -2, -4, -4});
  delete protein;
  std::cout << "\tCustom model generation successful.\n";

  /* Check custom model Protein generation with symmetry option. */
  protein = new Protein("HPPHPPHH", 2, "", {{"HH", -4}, {"HP", -2}}, false);
  assert_2d_protein_generation(
      protein, {{"HH", -4}, {"HP", -2}},
      std::vector<int>{-4, -2, -2, -4, -2, -2, -4, -4});
  delete protein;
  std::cout << "\tCustom non-symmetry model generation successful.\n";
}

/* Test Protein functionality in 2D space. */
void test_2d_protein() {
  Protein *protein = new Protein("HPPHPPHH", 2, "HP");
  std::vector<int> moves = std::vector<int>{1, 2, -1, -1, -2};
  std::vector<int> scores = std::vector<int>{0, 0, -1, -1, -1};
  std::vector<int> performed_moves;

  /* Perform predetermined moves and assert correctness of the Protein. */
  for (unsigned int i = 0; i < moves.size(); i++) {
    protein->place_amino(moves[i]);
    performed_moves = std::vector<int>(moves.begin(), moves.begin() + i + 1);

    assert(protein->hash_fold() == performed_moves);
    assert(protein->get_cur_len() == performed_moves.size() + 1);
    assert(protein->get_last_move() == moves[i]);
    assert(protein->get_score() == scores[i]);
  }

  /* Make sure an amino acid was placed. */
  std::vector<int> amino_pos = std::vector<int>(protein->get_dim() - 1, 0);
  amino_pos.push_back(1);
  AminoAcid *amino = protein->get_amino(amino_pos);
  assert(amino != NULL);

  /* Test removing amino acids. */
  for (size_t i = 0; i < moves.size(); i++) {
    protein->remove_amino();
    assert(protein->get_cur_len() == (moves.size() - i));
  }

  assert(protein->get_score() == 0);

  delete protein;
  std::cout << "\t2D Protein movements work.\n";
}

/* Test Protein functionality in 3D space. */
void test_3d_protein() {
  Protein *protein = new Protein("HPPHPPHH", 3, "HP");
  std::vector<int> moves = std::vector<int>{1, 2, -1, 3, -2, -1, -3};
  std::vector<int> scores = std::vector<int>{0, 0, -1, -1, -1, -1, -2};
  std::vector<int> performed_moves;

  /* Perform predetermined moves and assert correctness of the Protein. */
  for (size_t i = 0; i < moves.size(); i++) {
    protein->place_amino(moves[i]);
    performed_moves = std::vector<int>(moves.begin(), moves.begin() + i + 1);

    assert(protein->hash_fold() == performed_moves);
    assert(protein->get_cur_len() == performed_moves.size() + 1);
    assert(protein->get_last_move() == moves[i]);
    assert(protein->get_score() == scores[i]);
  }

  /* Make sure an amino was placed. */
  std::vector<int> amino_pos = std::vector<int>(protein->get_dim() - 1, 0);
  amino_pos.push_back(1);
  AminoAcid *amino = protein->get_amino(amino_pos);
  assert(amino != NULL);

  /* Test removing amino acids. */
  for (size_t i = 0; i < moves.size(); i++) {
    protein->remove_amino();
    assert(protein->get_cur_len() == (moves.size() - i));
  }

  assert(protein->get_score() == 0);

  delete protein;
  std::cout << "\t3D Protein movements work.\n";
}

/* Test if a 2D protein correctly computes indexes of formed bonds. */
void test_get_bonds() {
  Protein *protein = new Protein("HPPHPPHH", 2, "HP");
  std::vector<int> moves = std::vector<int>{1, 2, -1, -1, -1, -2, 1};

  for (int m : moves) {
    protein->place_amino(m);
  }

  std::vector<std::pair<int, int>> bonds = protein->get_bonds();
  std::vector<std::pair<int, int>> check_bonds = {
      std::pair<int, int>{0, 7}, std::pair<int, int>{0, 3},
      std::pair<int, int>{3, 0}, std::pair<int, int>{7, 0}};
  assert(bonds == check_bonds);

  delete protein;
  std::cout << "\t2D get_bonds() work.\n";
}

void test_get_contact_order() {
  /* Setup 2D protein. */
  Protein *protein_2d = new Protein("HPPHPPHH", 2, "HP");
  std::vector<int> moves = std::vector<int>{1, 2, -1, -1, -1, -2, 1};
  for (int m : moves) {
    protein_2d->place_amino(m);
  }

  /* Test for 2D protein contact order and clean up. */
  float contact_order = protein_2d->get_contact_order();
  assert(contact_order == 0.5);
  delete protein_2d;

  /* Setup 3D protein. */
  Protein *protein_3d = new Protein("HPPHPPHH", 3, "HP");
  std::vector<int> moves = std::vector<int>{1, 2, -1, 3, -2, -1, -3};
  for (int m : moves) {
    protein_3d->place_amino(m);
  }

  /* Test for 3D protein contact order and clean up. */
  float contact_order = protein_3d->get_contact_order();
  assert(contact_order == 0.625);
  delete protein_3d;

  std::cout << "\t2D and 3D get_contact_order() works.\n";
}

int main(void) {
  /* Test different Protein generation options. */
  test_protein_generation();

  /* Test Protein in a 2D space. */
  test_2d_protein();

  /* Test Protein in a 3D space. */
  test_3d_protein();

  /* Test get_bonds() function. */
  test_get_bonds();

  /* Test get_contact_order() function. */
  test_get_contact_order();

  return 0;
}
