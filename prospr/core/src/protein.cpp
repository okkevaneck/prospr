/* File:            protein.cpp
 * Description:     Source file for a protein object within the HP-model.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "protein.hpp"

#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>

/* Zero argument constructor. */
Protein::Protein(){};

/* Copy constructor. */
Protein::Protein(const Protein &other) {
  /* Set model essentials. */
  this->sequence = other.sequence;
  this->dim = other.dim;
  this->bond_values = other.bond_values;
  this->weighted_amino_acids = other.weighted_amino_acids;
  this->max_weights = other.max_weights;

  /* Create new AminoAcid objects for all amino acids. */
  AminoAcid *new_aa;
  this->amino_acids.clear();
  for (AminoAcid *other_aa : other.amino_acids) {
    new_aa = new AminoAcid(*other_aa);
    this->amino_acids.push_back(new_aa);
  }

  /* Copy placement of other protein and map to new AminoAcids. */
  this->space.clear();
  for (auto &item : other.space) {
    this->space[item.first] = this->amino_acids[item.second->get_index()];
  }

  /* Copy state of protein's values. */
  this->cur_len = other.cur_len;
  this->last_move = other.last_move;
  this->last_pos = other.last_pos;
  this->score = other.score;
  this->aminos_placed = other.aminos_placed;
  this->solutions_checked = other.solutions_checked;
};

/* Construct a new Protein. */
Protein::Protein(std::string sequence, int dim, std::string model,
                 std::map<std::string, int> bond_values, bool bond_symmetry) {
  this->sequence = sequence;
  space = {};
  cur_len = 0;
  this->dim = dim;
  last_move = 0;
  last_pos.assign(dim, 0);
  score = 0;
  solutions_checked = 0;
  aminos_placed = 0;

  /* Deduct what model to use, or apply custom one. */
  if (model == "HP") {
    /* Store bond_values according to the HP-model. */
    std::map<std::string, int> model_bonds = {{"HH", -1}};
    this->bond_values = model_bonds;
    this->weighted_amino_acids = "H";
  } else if (model == "HPXN") {
    /* Store bond_values according to the HPXN-model. */
    std::map<std::string, int> model_bonds = {
        {"HH", -4}, {"PP", -1}, {"PN", -1}, {"NN", 1}};
    this->bond_values = model_bonds;
    this->weighted_amino_acids = "HPN";
  } else {
    /* Store the given bond_values weights. */
    std::string weighted_amino_acids = "";
    std::string reversed_bond;

    /* Add amino acid to list of weighted if not added yet. */
    for (const auto &element : bond_values) {
      /* Search all unique weighted amino acids. */
      for (char const &amino_acid : element.first) {
        if (weighted_amino_acids.find(amino_acid) == std::string::npos)
          weighted_amino_acids += amino_acid;
      }

      /* For every bond, add symmetry value if not in map already. */
      if (bond_symmetry) {
        reversed_bond = std::string(1, element.first[1]) + element.first[0];

        /* Add symmetry value of the bond. */
        if (bond_values.count(reversed_bond) == 0) {
          bond_values[reversed_bond] = element.second;
        }
      }
    }

    this->bond_values = bond_values;
    this->weighted_amino_acids = weighted_amino_acids;
  }

  /* Create mapping from all weighted amino acids to their maximum achievable
   * scores. This is used to create a vector of maximum achievable scores per
   * index.
   */
  std::map<char, int> max_amino_weights;

  for (const auto &c : this->weighted_amino_acids) {
    max_amino_weights[c] = 0;
  }

  /* Store the highest achievable weight per amino acid. */
  for (const auto &element : this->bond_values) {
    for (const auto &c : element.first) {
      if (element.second < max_amino_weights[c] ||
          (max_amino_weights[c] == 0 && element.second > max_amino_weights[c]))
        max_amino_weights[c] = element.second;
    }
  }

  /* Create AminoAcid objects for all amino acids. */
  for (std::string::size_type i = 0; i < sequence.size(); i++) {
    AminoAcid *new_aa = new AminoAcid(sequence[i], (int)i, 0, 0);
    amino_acids.push_back(new_aa);
    this->max_weights.push_back(max_amino_weights[sequence[i]]);
  }

  /* Place the first amino acid at the origin if there is one. */
  if (sequence.size() != 0) {
    space[last_pos] = amino_acids[0];
    cur_len++;
    aminos_placed++;
  }
}

/* Overload assignment operator for assignments. */
Protein &Protein::operator=(const Protein &other) {
  /* Set model essentials. */
  this->sequence = other.sequence;
  this->dim = other.dim;
  this->bond_values = other.bond_values;
  this->weighted_amino_acids = other.weighted_amino_acids;
  this->max_weights = other.max_weights;

  /* Create new AminoAcid objects for all amino acids. */
  AminoAcid *new_aa;
  this->amino_acids.clear();
  for (AminoAcid *other_aa : other.amino_acids) {
    new_aa = new AminoAcid(*other_aa);
    this->amino_acids.push_back(new_aa);
  }

  /* Copy placement of other protein and map to new AminoAcids. */
  this->space.clear();
  for (auto &item : other.space) {
    this->space[item.first] = this->amino_acids[item.second->get_index()];
  }

  /* Copy state of protein's values. */
  this->cur_len = other.cur_len;
  this->last_move = other.last_move;
  this->last_pos = other.last_pos;
  this->score = other.score;
  this->aminos_placed = other.aminos_placed;
  this->solutions_checked = other.solutions_checked;

  return *this;
}

/* Returns the Protein's sequence. */
std::string Protein::get_sequence() { return sequence; }

/* Returns the Protein's set maximum dimension. */
int Protein::get_dim() { return dim; }

/* Returns the Protein's set of bond links. */
std::map<std::string, int> Protein::get_bond_values() { return bond_values; }

/* Returns the Protein's current length. */
int Protein::get_cur_len() { return cur_len; }

/* Returns the last performed move. */
int Protein::get_last_move() { return last_move; }

/* Returns the last position an amino acid was placed. */
std::vector<int> Protein::get_last_pos() { return last_pos; }

/* Returns the AminoAcid at the given position, or NULL if there is
 * none.
 */
AminoAcid *Protein::get_amino(std::vector<int> position) {
  if (space.count(position))
    return space.at(position);
  else
    return NULL;
}

/* Returns the Protein's current score. */
int Protein::get_score() { return score; }

/* Returns the number of checked solutions. */
std::uint64_t Protein::get_solutions_checked() { return solutions_checked; }

/* Returns the number of amino acids placed. */
std::uint64_t Protein::get_aminos_placed() { return aminos_placed; }

/* Returns if the amino acid at the given index is weighted. */
bool Protein::is_weighted(int index) {
  return weighted_amino_acids.find(sequence[index]) != std::string::npos;
}

/* Returns the weight created between two amino acids. */
int Protein::get_weight(std::string aminos) {
  std::map<std::string, int>::iterator it = bond_values.find(aminos);

  if (it == bond_values.end()) {
    return 0;
  } else {
    return it->second;
  }
}

/* Returns the vector with maximum achievable weights per amino acid. */
std::vector<int> Protein::get_max_weights() { return max_weights; }

/* Reset all variables of a protein as if it was just initialized. */
void Protein::reset() {
  space.clear();
  cur_len = 1;
  last_pos.assign(dim, 0);
  last_move = 0;
  score = 0;
  solutions_checked = 0;
  aminos_placed = 0;

  space[last_pos] = amino_acids[0];
}

/* Reset only the conformation variables of a protein. */
void Protein::reset_conformation() {
  space.clear();
  cur_len = 1;
  last_pos.assign(dim, 0);
  last_move = 0;
  score = 0;

  space[last_pos] = amino_acids[0];
}

/* Returns true if a move is valid, returns false otherwise. */
bool Protein::is_valid(int move) {
  std::vector<int> check_pos = last_pos;
  check_pos[abs(move) - 1] += move / abs(move);

  /* Check if the placement causes overlap. */
  if (space.count(check_pos) == 0)
    return true;
  else
    return false;
}

/* Place the next amino acid and update the conformation accordingly. */
void Protein::place_amino(int move, bool track) {
  /* Check for illegal move. */
  if (move == 0)
    throw std::runtime_error("Protein folded onto itself..");

  space[last_pos]->set_next_move(move);
  last_pos[abs(move) - 1] += move / abs(move);

  /* Check for illegal folds. */
  if (space.count(last_pos) > 0)
    throw std::runtime_error("Protein folded onto itself..");

  /* Place amino acid on new (valid) position. */
  space[last_pos] = amino_acids[cur_len];
  space[last_pos]->set_prev_move(move);
  last_move = move;

  /* Change score according to placement of the new amino. */
  if (is_weighted(cur_len))
    _change_score(move, true);

  cur_len++;

  /* Update number of found solutions and amino acids placed. */
  if (track) {
    aminos_placed++;

    if (cur_len == (int)sequence.size()) {
      solutions_checked++;
    }
  }
}

/* Remove last placed amino acid and change score accordingly. */
void Protein::remove_amino() {
  if (cur_len == 1)
    throw std::runtime_error("Cannot remove the last amino acid at origin..");

  cur_len--;

  if (is_weighted(cur_len))
    _change_score(last_move, false);

  /* Remove the last amino. */
  space.erase(last_pos);
  last_pos[abs(last_move) - 1] -= last_move / abs(last_move);
  space[last_pos]->set_next_move(0);
  last_move = space[last_pos]->get_prev_move();
}

/* Hash and return the fold of the current conformation. */
std::vector<int> Protein::hash_fold() {
  std::vector<int> fold_hash;
  std::vector<int> cur_pos(dim, 0);
  AminoAcid *cur_amino;
  int next_move = 0;

  if (space.count(cur_pos) > 0) {
    cur_amino = space.at(cur_pos);
    next_move = cur_amino->get_next_move();

    while (next_move != 0) {
      cur_pos[abs(next_move) - 1] += next_move / abs(next_move);
      fold_hash.push_back(next_move);
      cur_amino = space.at(cur_pos);
      next_move = cur_amino->get_next_move();
    }
  }

  return fold_hash;
}

/* Set the conformation to the given hash. */
void Protein::set_hash(std::vector<int> fold_hash, bool track) {
  reset_conformation();

  for (auto &move : fold_hash) {
    place_amino(move, track);
  }
}

/* Change score according to the already performed addition or removal of the
 * given move. This function is preferably only called when the altered amino
 * acid is weighted.
 *      :placed is true if the move placed an amino, false otherwise.
 */
void Protein::_change_score(int move, bool placed) {
  std::vector<int> moves;

  for (int i = -dim; i <= dim; i++) {
    if (i != 0 and i != -move)
      moves.push_back(i);
  }

  /* Initialize string with amino character for bond weight checking. */
  std::string aminos = std::string() + space[last_pos]->get_type();
  std::vector<int> cur_pos;

  for (auto &move : moves) {
    cur_pos = last_pos;
    cur_pos[abs(move) - 1] += move / abs(move);

    /* Update score if placing the amino creates a bond. */
    if (space.count(cur_pos) > 0) {
      /* get_weight() returns 0 if no bond is made. */
      if (placed) {
        score += get_weight(aminos + space[cur_pos]->get_type());
      } else {
        score -= get_weight(aminos + space[cur_pos]->get_type());
      }
    }
  }
}

/* Return a vector with int pairs that represent the aminos making a bond. */
std::vector<std::pair<int, int>>
Protein::_append_bond_pairs(std::vector<std::pair<int, int>> pairs,
                            std::vector<int> pos, std::vector<int> moves) {
  AminoAcid *cur_amino = space[pos];
  char cur_type = cur_amino->get_type();
  std::vector<int> other_pos;
  std::string bond;

  /* Check if amino acid at pos cannot create bonds. */
  if (weighted_amino_acids.find(cur_type) != std::string::npos) {
    /* For each surrounding non-connected space, check for bonds. */
    for (int m : moves) {
      other_pos = pos;
      other_pos[abs(m) - 1] += m / abs(m);

      /* Check if there is an amino on the other_pos location. */
      if (space.count(other_pos) != 0) {
        bond = {cur_type, space[other_pos]->get_type()};

        /* Add pair if it creates a bond. */
        if (bond_values[bond] < 0) {
          pairs.push_back(std::make_pair(cur_amino->get_index(),
                                         space[other_pos]->get_index()));
        }
      }
    }
  }

  return pairs;
}

/* Get the pairs of amino acids indexes forming bonds. */
std::vector<std::pair<int, int>> Protein::get_bonds() {
  std::vector<int> pos(dim, 0);
  std::vector<std::pair<int, int>> pairs;

  /* Create set of all possible moves. */
  std::vector<int> moves(dim * 2 + 1);
  std::iota(moves.begin(), moves.end(), -dim);
  moves.erase(moves.begin() + dim);

  /* Make list with possible bond creating moves from origin.  */
  std::vector<int> cur_moves = moves;
  cur_moves.erase(std::remove(cur_moves.begin(), cur_moves.end(),
                              space[pos]->get_next_move()),
                  cur_moves.end());

  /* Check if the origin has formed any bonds. */
  pairs = this->_append_bond_pairs(pairs, pos, cur_moves);

  /* Check if any of the non-origin amino acids has formed a bond. */
  for (int m : this->hash_fold()) {
    /* Update position. */
    pos[abs(m) - 1] += m / abs(m);

    /* Update move set with options from new position. */
    cur_moves = moves;
    cur_moves.erase(std::remove(cur_moves.begin(), cur_moves.end(),
                                space[pos]->get_prev_move()),
                    cur_moves.end());

    if (space[pos]->get_next_move() != 0)
      cur_moves.erase(std::remove(cur_moves.begin(), cur_moves.end(),
                                  space[pos]->get_next_move()),
                      cur_moves.end());

    /* Append pairs with bonds from new position. */
    pairs = this->_append_bond_pairs(pairs, pos, cur_moves);
  }

  return pairs;
}

/* Overload << operator for printing Proteins. */
std::ostream &operator<<(std::ostream &os, Protein &protein) {
  std::cout << "<Protein s=" << protein.get_score()
            << " l=" << protein.get_cur_len() << ", [ ";
  for (int i : protein.hash_fold()) {
    std::cout << i << " ";
  }

  std::cout << "]>" << std::flush;
  return os;
}
