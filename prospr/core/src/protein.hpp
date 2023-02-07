/* File:            protein.h
 * Description:     Header file for a protein object within the HP-model.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#ifndef PROTEIN_H
#define PROTEIN_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "amino_acid.hpp"

class Protein {
public:
  /* Zero argument constructor. */
  Protein();

  /* Copy constructor. */
  Protein(const Protein &other);

  /* Construct a new Protein. */
  Protein(std::string sequence, int dim = 2, std::string model = "HP",
          std::map<std::string, int> bond_values = {},
          bool bond_symmetry = true);

  /* Overload assignment operator for assignments. */
  Protein &operator=(const Protein &other);

  /* Returns the Protein's sequence. */
  std::string get_sequence();

  /* Returns the Protein's set maximum dimension. */
  int get_dim();

  /* Returns the Protein's set of bond links. */
  std::map<std::string, int> get_bond_values();

  /* Returns the Protein's current length. */
  int get_cur_len();

  /* Returns the last performed move. */
  int get_last_move();

  /* Returns the last position an amino acid was placed. */
  std::vector<int> get_last_pos();

  /* Returns the AminoAcid at the given position, or NULL if there is
   * none.
   */
  AminoAcid *get_amino(std::vector<int> position);

  /* Returns the Protein's current score. */
  int get_score();

  /* Returns the number of performed changes. */
  std::uint64_t get_solutions_checked();

  /* Returns the number of amino acids placed. */
  std::uint64_t get_aminos_placed();

  /* Returns if the amino acid at the given index is weighted. */
  bool is_weighted(int index);

  /* Returns the weight created between two amino acids. */
  int get_weight(std::string aminos);

  /* Returns the vector with maximum achievable weights per amino acid. */
  std::vector<int> get_max_weights();

  /* Reset all variables of a protein as if it was just initialized. */
  void reset();

  /* Reset only the conformation variables of a protein. */
  void reset_conformation();

  /* Returns true if a move is valid, returns false otherwise. */
  bool is_valid(int move);

  /* Place the next amino acid and update the conformation accordingly. */
  void place_amino(int move, bool track = true);

  /* Remove last placed amino acid and change score accordingly. */
  void remove_amino();

  /* Hash and return the fold of the current conformation. */
  std::vector<int> hash_fold();

  /* Set the conformation to the given hash. */
  void set_hash(std::vector<int> fold_hash, bool track = false);

  /* Get the pairs of amino acids indexes forming bonds. */
  std::vector<std::pair<int, int>> get_bonds();

private:
  std::string sequence;
  std::map<std::vector<int>, AminoAcid *> space;
  int cur_len;
  int dim;
  std::map<std::string, int> bond_values;
  std::string weighted_amino_acids;
  std::vector<int> max_weights;
  int last_move;
  std::vector<int> last_pos;
  int score;
  std::uint64_t aminos_placed;
  std::uint64_t solutions_checked;
  std::vector<AminoAcid *> amino_acids;

  /* Change score according to the already performed addition or removal
   * of the given move.
   */
  void _change_score(int move, bool placed);

  /* Return a vector with int pairs that represent the amino acids making
   * a bond.
   */
  std::vector<std::pair<int, int>>
  _append_bond_pairs(std::vector<std::pair<int, int>> pairs,
                     std::vector<int> pos, std::vector<int> moves);
};

/* Overload << operator for printing Proteins. */
std::ostream &operator<<(std::ostream &os, Protein &protein);

#endif
