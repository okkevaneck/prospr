/* File:       Protein.cpp
 * Author:     Okke van Eck
 *
 * Description:    Source file for a Protein object within the HP-model.
 */

#include "Protein.h"
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
    if (move != 0 &&
            find(h_idxs.begin(), h_idxs.end(), cur_len) != h_idxs.end())
        change_score(move, -1);

    space[last_pos] = std::vector<int>{cur_len, 0};
    last_move = move;
    cur_len++;
}

void Protein::remove_amino() {
    /* Remove last placed amino acid and set direction for new last placed amino
     * acid to 0.
     */
     cur_len--;

     if (last_move != 0 &&
            find(h_idxs.begin(), h_idxs.end(), cur_len) != h_idxs.end())
            change_score(last_move, 1);

    /* Remove the last placed amino acid and change direction of the amino
     * before.
     */
    space.erase(last_pos);
    last_pos[abs(last_move) - 1] -= last_move / abs(last_move);
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

        if (space.count(cur_pos) > 0 &&
                find(h_idxs.begin(), h_idxs.end(), cur_len) != h_idxs.end())
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
