#!/usr/bin/env python3
"""
File:           helpers.py
Description:    This file contains helper functions for a Protein instance.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

import numpy as np


def get_scoring_aminos(protein):
    """
    Collect all placed aminos from a protein that may score points.
    """
    score_pos = {}
    cur_pos = [0 for _ in range(protein.dim)]
    amino_acid = protein.get_amino(cur_pos)
    idx = amino_acid.index
    next_dir = amino_acid.next_move
    max_weights = protein.max_weights

    # Store origin if it may score points.
    if max_weights[idx] < 0:
        score_pos[tuple(cur_pos)] = np.array([0, next_dir], dtype=np.int64)

    while next_dir != 0:
        # Compute position of next amino and get its amino and fold.
        cur_pos[abs(next_dir) - 1] += next_dir // abs(next_dir)
        amino_acid = protein.get_amino(cur_pos)
        idx = amino_acid.index
        fold = amino_acid.next_move

        # Store previous directions for checking existing connections.
        prev_dir = -next_dir
        next_dir = fold

        # Save amino if it may score points.
        if max_weights[idx] < 0:
            score_pos[tuple(cur_pos)] = np.array(
                [prev_dir, next_dir], dtype=np.int64
            )

    return score_pos


def get_scoring_pairs(protein):
    """
    Fetch positions of scoring amino in pairs for the currently folded amino
    acid chain.
    """
    # Get dictionary with the amino's that can possibly score points.
    score_aminos = get_scoring_aminos(protein)

    # Sort positions from bottom-left to upper-right.
    moves = np.array([m for m in range(1, protein.dim + 1)])
    pairs = np.empty((1, 2, protein.dim), dtype=np.int64)

    # Check if a score can be made with the amino above or to the right.
    for pos, [prev_dir, next_dir] in score_aminos.items():
        other_pos = np.array(pos, dtype=np.int64)

        # Check every positive direction, started in the lower quadrant.
        for move in moves:
            if prev_dir != move and next_dir != move:
                other_pos[move - 1] += 1

                if tuple(other_pos) in score_aminos:
                    new_pair = np.array(
                        [[pos, tuple(other_pos)]], dtype=np.int64
                    )
                    pairs = np.vstack((pairs, new_pair))

                other_pos[move - 1] -= 1

    # Return all pairs without the first row of zeros.
    return pairs[1:]


def get_ordered_positions(protein):
    """
    Fetch the positions of the currently folded amino chain in order of
    placement.
    """
    # Fetch done moves and the aminos used so far.
    moves = protein.hash_fold()
    aminos = protein.sequence[: len(moves) + 1]

    # Setup storage of the amino positions.
    cur_pos = np.array([0 for _ in range(protein.dim)], dtype=np.int64)
    positions = np.array([*cur_pos, aminos[0]])

    # Save the coordinates of the aminos in order.
    for amino, move in zip(aminos[1:], moves):
        cur_pos[abs(move) - 1] += move // abs(move)
        positions = np.vstack((positions, np.array([*cur_pos, amino])))

    return positions
