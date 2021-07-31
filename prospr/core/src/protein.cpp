/* File:            protein.cpp
 * Description:     Source file for a protein object within the HP-model.
 */

#include "protein.hpp"
#include <stdlib.h>
#include <algorithm>


/* Construct a new Protein. */
Protein::Protein(std::string sequence, int dim, std::string model,
        std::map<std::string, int> bond_values, bool bond_symmetry) {
    this->sequence = sequence;
    space = {};
    cur_len = 1;
    this->dim = dim;
    last_move = 0;
    last_pos.assign(dim, 0);
    score = 0;
    changes = 0;

    /* Deduct what model to use, or apply custom one. */
    if (model == "HP") {
        std::map<std::string, int> model_bonds = {{"HH", -4}};
        this->bond_values = model_bonds;
        this->weighted_amino_acids = "H";
    } else if (model == "HPXN") {
        std::map<std::string, int> model_bonds = {{"HH", -4}, {"PP", -1},
                                                  {"PN", -1}, {"NN", 1}};
        this->bond_values = model_bonds;
        this->weighted_amino_acids = "HPN";
    } else if (bond_symmetry) {
        std::string weighted_amino_acids = "";
        std::string reversed_bond;

        /* For every bond, add symmetry value if not in map already.
         * Also add amino acid to list of weighted if not added yet.
         */
        for (std::pair<std::string, int> element : bond_values) {
            reversed_bond = std::string(1, element.first[1]) + element.first[0];

            if (bond_values.count(reversed_bond) == 0) {
                bond_values[reversed_bond] = element.second;
            }

            for (char const &amino_acid: element.first) {
                if (!weighted_amino_acids.contains(amino_acid))
                    weighted_amino_acids += amino_acid;
            }
        }

        this->bond_values = bond_values;
    } else {
        std::string weighted_amino_acids = "";

        /* Search all unique weighted amino acids. */
        for (char const &amino_acid: element.first) {
            if (!weighted_amino_acids.contains(amino_acid))
                weighted_amino_acids += amino_acid;
        }

        this->bond_values = bond_values;
        this->weighted_amino_acids = weighted_amino_acids;
    }

    /* Create AminoAcid objects for all amino acids. */
    for (std::string::size_type i = 0; i < sequence.size(); i++) {
        AminoAcid* new_aa = new AminoAcid(sequence[i], (int)i, 0, 0);
        amino_acids.push_back(new_aa);
    }

    /* Place the first amino acid at the origin. */
    space[last_pos] = amino_acids[0];
}

/* Returns the Protein's sequence. */
std::string Protein::get_sequence() {
    return sequence;
}

/* Returns the Protein's set maximum dimension. */
int Protein::get_dim() {
    return dim;
}

/* Returns the Protein's set of bond links. */
std::map<std::string, int> Protein::get_bond_values() {
    return bond_values;
}

/* Returns the Protein's current length. */
int Protein::get_cur_len() {
    return cur_len;
}

/* Returns the last performed move. */
int Protein::get_last_move() {
    return last_move;
}

/* Returns the last position an amino acid was placed. */
std::vector<int> Protein::get_last_pos() {
    return last_pos;
}

/* Returns the AminoAcid at the given position, or NULL if there is
 * none.
 */
AminoAcid* Protein::get_amino(std::vector<int> position) {
     if (space.count(position))
        return space.at(position);
     else
        return NULL;
}

/* Returns the Protein's current score. */
int Protein::get_score() {
    return score;
}

/* Returns the number of performed changes. */
int Protein::get_changes() {
    return changes;
}

/* Returns if the amino acid at the given index is weighted. */
bool Protein::is_weighted(int index) {
    return weighted_amino_acids.find(sequence[index]) != std::string::npos;
}

/* Reset all variables of a protein as if it was just initialized. */
void Protein::reset() {
    space.clear();
    cur_len = 1;
    last_pos.assign(dim, 0);
    last_move = 0;
    score = 0;
    changes = 0;

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
    /* Check for illegal moves. */
    if (move == 0)
        throw std::runtime_error("Protein folded onto itself..");

    if (track)
        changes++;

    space[last_pos]->set_next_move(move);
    last_pos[abs(move) - 1] += move / abs(move);

    /* Check for illegal folds. */
    if (space.count(last_pos) > 0)
        throw std::runtime_error("Protein folded onto itself..");

    /* Change score according to placement of the new amino. */
    if (is_weighted(cur_len))
        _change_score(move);

    space[last_pos] = amino_acids[cur_len];
    space[last_pos]->set_prev_move(move);
    last_move = move;
    cur_len++;
}

/* Remove last placed amino acid and change score accordingly. */
void Protein::remove_amino() {
    if (cur_len == 1)
        throw std::runtime_error("Cannot remove the last amino acid at origin..");

    cur_len--;

    if (is_weighted(cur_len))
        _change_score(last_move);

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
    AminoAcid* cur_amino;
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

    for (auto &move: fold_hash) {
        place_amino(move, track);
    }
}

/* Change score according to the already performed addition or removal of the
 * given move.
 */
void Protein::_change_score(int move) {
    std::vector<int> moves;

    for (int i = -dim; i <= dim; i++) {
        if (i != 0 and i != -move)
            moves.push_back(i);
    }

    std::vector<int> cur_pos;

    for (auto &move: moves) {
        cur_pos = last_pos;
        cur_pos[abs(move) - 1] += move / abs(move);

        if (space.count(cur_pos) > 0 && is_weighted(space[cur_pos]->get_index())) // TODO: Change is_hydro to new function get_weight between two indexes.
            score += weight;
    }
}
