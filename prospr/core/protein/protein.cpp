/* File:            protein.cpp
 * Description:     Source file for a protein object within the HP-model.
 */

#include "protein.hpp"
#include <stdlib.h>
#include <algorithm>


Protein::Protein(std::string sequence, int dim) {
    this->sequence = sequence;
    space = {};
    cur_len = 0;
    this->dim = dim;
    last_move = 0;
    last_pos.assign(dim, 0);
    score = 0;
    changes = 0;

    /* Store all indices of the H-amino acid characters in hIdxs. */
    size_t pos = sequence.find("H", 0);

    while (pos != std::string::npos) {
        h_idxs.push_back(pos);
        pos = sequence.find("H", pos + 1);
    }
}

std::string Protein::get_sequence() {
    return sequence;
}

int Protein::get_dim() {
    return dim;
}

int Protein::get_cur_len() {
    return cur_len;
}

std::vector<int> Protein::get_last_pos() {
    return last_pos;
}

std::vector<int> Protein::get_amino(std::vector<int> position) {
    /* Return the amino acid and next direction at the given position or an
     * empty std::vector if there is no amino acid at the given position.
     */
     if (space.count(position))
        return space.at(position);
     else
        return {};
}

int Protein::get_score() {
    return score;
}

int Protein::get_changes() {
    return changes;
}

std::vector<int> Protein::get_h_idxs() {
    return h_idxs;
}

bool Protein::is_hydro(int index) {
    return find(h_idxs.begin(), h_idxs.end(), index) != h_idxs.end();
}

void Protein::reset() {
    /* Reset all variables of a protein as it was just initialized. */
    space.clear();
    cur_len = 0;
    last_pos.assign(dim, 0);
    last_move = 0;
    score = 0;
    changes = 0;
}

void Protein::reset_conformation() {
    /* Reset only the conformation variables of a protein. */
    space.clear();
    cur_len = 0;
    last_pos.assign(dim, 0);
    last_move = 0;
    score = 0;
}

bool Protein::is_valid(int move) {
    /* Returns True if a move does not cause overlap, returns False otherwise.
     */
    std::vector<int> check_pos = last_pos;
    check_pos[abs(move) - 1] += move / abs(move);

    if (space.count(check_pos) == 0)
        return true;
    else
        return false;
}

void Protein::place_amino(int move, bool track) {
    /* Place amino acid and update score accordingly. */
    if (track)
        changes++;

    if (move != 0) {
        space[last_pos][1] = move;
        last_pos[abs(move) - 1] += move / abs(move);
    }

    if (space.count(last_pos) > 0)
        throw std::runtime_error("Protein folded onto itself..");

    /* Change score according to placement of the new amino. */
    if (move != 0 && is_hydro(cur_len))
        change_score(move, -1);

    space[last_pos] = std::vector<int>{cur_len, 0};
    last_move = move;
    cur_len++;
}

void Protein::remove_amino(int move) {
    /* Change score according to removal of the last amino. */
    cur_len--;

    if (move != 0 && is_hydro(cur_len))
        change_score(move, 1);

    /* Remove the last amino. */
    space.erase(last_pos);
    last_pos[abs(move) - 1] -= move / abs(move);
    space[last_pos][1] = 0;
}

void Protein::change_score(int move, int value) {
    /* Change score according to the addition or removal of the given move. */
    std::vector<int> moves;

    for (int i = -dim; i <= dim; i++) {
        if (i != 0 and i != -move)
            moves.push_back(i);
    }

    std::vector<int> cur_pos;

    for (auto &move: moves) {
        cur_pos = last_pos;
        cur_pos[abs(move) - 1] += move / abs(move);

        if (space.count(cur_pos) > 0 && is_hydro(space[cur_pos][0]))
            score += value;
    }
}

std::vector<int> Protein::hash_fold() {
    /* Hash and return the fold of the current conformation. */
    std::vector<int> fold_hash;
    std::vector<int> cur_pos(dim, 0);
    std::vector<int> item;

    if (space.count(cur_pos) > 0) {
        item = space.at(cur_pos);

        while (item[1] != 0) {
            cur_pos[abs(item[1]) - 1] += item[1] / abs(item[1]);
            fold_hash.push_back(item[1]);
            item = space.at(cur_pos);
        }
    }

    return fold_hash;
}

void Protein::set_hash(std::vector<int> fold_hash, bool track) {
    /* Set the conformation to the given hash. */
    reset_conformation();
    place_amino(0, track);

    for (auto &move: fold_hash) {
        place_amino(move, track);
    }
}


/* Pybind11 module definition. */
#include <pybind11/pybind11.h>
namespace py = pybind11;

PYBIND11_MODULE(protein, m) {
    m.doc() = "Definition of a Protein instance.";
    py::class_<Protein>(m, "Protein")
        .def(py::init<const std::string, int &>(), "Protein constructor",
                py::arg("sequence"), py::arg("dim")=2)
        .def_property_readonly("sequence", &Protein::get_sequence)
        .def_property_readonly("h_idxs", &Protein::get_h_idxs)
        .def_property_readonly("cur_len", &Protein::get_cur_len)
        .def_property_readonly("dim", &Protein::get_dim)
        .def_property_readonly("last_pos", &Protein::get_last_pos)
        .def_property_readonly("score", &Protein::get_score)
        .def_property_readonly("changes", &Protein::get_changes)

        .def("get_amino", &Protein::get_amino)
        .def("is_hydro", &Protein::is_hydro)
        .def("reset", &Protein::reset)
        .def("reset_conformation", &Protein::reset_conformation)
        .def("is_valid", &Protein::is_valid)
        .def("place_amino", &Protein::place_amino) // TODO: Check default args, maybe import extra pybind headers? Execution causes segfault.
        .def("remove_amino", &Protein::remove_amino)
        .def("change_score", &Protein::change_score)
        .def("hash_fold", &Protein::hash_fold)
        .def("set_hash", &Protein::set_hash)
    ;
}

/* List of private properties with no getters or setters:
 *      space, last_move
 */