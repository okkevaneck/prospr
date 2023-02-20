/* File:            test_protein.cpp
 * Description:     Source file for testing the functionality of protein.
 *                  Expects that the following modules work:
 *                      - amino_acid.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include <assert.h>
#include <string.h>

#include <iostream>

#include "../src/beam_search.hpp"
#include "../src/depth_first.hpp"
#include "../src/depth_first_bnb.hpp"
#include "../src/protein.hpp"

/* Test functionality of depth_first. */
void test_depth_first() {
  /* Check if 2D solutions are found correctly. */
  Protein *protein = new Protein("PHPHPHPPH", 2, "HP");
  protein = depth_first(protein);
  assert(protein->get_score() == -3);
  delete protein;
  std::cout << "\t2D Protein solution scores matches.\n";

  /* Check if 3D solutions are found correctly. */
  protein = new Protein("HPPHPHPHPH", 3, "HP");
  protein = depth_first(protein);
  assert(protein->get_score() == -4);
  delete protein;
  std::cout << "\t3D Protein solution scores matches.\n";
}

/* Test functionality of depth_first_bnb. */
void test_depth_first_bnb() {
  /* Check if 2D solutions are found correctly. */
  Protein *protein = new Protein("PHPHPHPPH", 2, "HP");
  protein = depth_first_bnb(protein);
  assert(protein->get_score() == -3);
  delete protein;
  std::cout << "\t2D Protein solution scores matches with naive_prune.\n";

  /* Check if 2D solutions are found with reach_prune criteria. */
  protein = new Protein("PHPHPHPPH", 2, "HP");
  protein = depth_first_bnb(protein, "reach_prune");
  assert(protein->get_score() == -3);
  delete protein;
  std::cout << "\t2D Protein solution scores matches with reach_prune.\n";

  /* Check if 3D solutions are found correctly. */
  protein = new Protein("HPPHPHPHPH", 3, "HP");
  protein = depth_first_bnb(protein);
  assert(protein->get_score() == -4);
  delete protein;
  std::cout << "\t3D Protein solution scores matches.\n";
}

/* Test functionality of depth_first_bnb. */
void test_beam_search() {
  /* Check if 2D solutions are found correctly with a beam width of -1. */
  Protein *protein = new Protein("PHPHPHPPH", 2, "HP");
  protein = beam_search(protein, -1);
  assert(protein->get_score() == -3);
  delete protein;
  std::cout << "\t2D Protein solution scores matches with beam_width = -1.\n";

  /* Check if 2D solutions are found correctly with a beam width of 99. */
  protein = new Protein("PHPHPHPPH", 2, "HP");
  protein = beam_search(protein, 99);
  assert(protein->get_score() == -3);
  delete protein;
  std::cout << "\t2D Protein solution scores matches with beam_width = 99.\n ";

  /* Check if 2D solutions are found correctly with a beam width of 40. */
  protein = new Protein("PHPHPHPPH", 2, "HP");
  protein = beam_search(protein, 40);
  assert(protein->get_score() == -2);
  delete protein;
  std::cout << "\t2D Protein solution scores matches with beam_width = 40.\n ";

  /* Check if 3D solutions are found correctly with a beam width of 99. */
  protein = new Protein("HPPHPHPHPH", 3, "HP");
  protein = beam_search(protein, 99);
  assert(protein->get_score() == -4);
  delete protein;
  std::cout << "\t3D Protein solution scores matches with beam_width = 99.\n ";

  /* Check if 3D solutions are found correctly with a beam width of 10. */
  protein = new Protein("HPPHPHPHPH", 3, "HP");
  protein = beam_search(protein, 10);
  assert(protein->get_score() == -4);
  delete protein;
  std::cout << "\t3D Protein solution scores matches with beam_width = 10.\n ";

  /* Check if 3D solutions are found correctly with a beam width of 5. */
  protein = new Protein("HPPHPHPHPH", 3, "HP");
  protein = beam_search(protein, 5);
  assert(protein->get_score() == -3);
  delete protein;
  std::cout << "\t3D Protein solution scores matches with beam_width = 5.\n ";
}

/* Test functionality of all algorithms. */
void run_all() {
  test_depth_first();
  test_depth_first_bnb();
  test_beam_search();
}

int main(int argc, char *argv[]) {
  /* Determine what algorithms to test. */
  if (argc == 1) {
    /* No extra arguments, so testing all algorithms. */
    std::cout << "\tTesting all algorithms..\n";
    run_all();
  } else if (strcmp(argv[1], "depth_first") == 0) {
    test_depth_first();
  } else if (strcmp(argv[1], "depth_first_bnb") == 0) {
    test_depth_first_bnb();
  } else if (strcmp(argv[1], "beam_search") == 0) {
    test_beam_search();
  } else {
    std::cout << "\tSpecific algorithm not detected..\n";
  }

  return 0;
}
