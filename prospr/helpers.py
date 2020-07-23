#!/usr/bin/env python3
"""
File:       helpers.py
Author:     Okke van Eck

Description:    This file contains helper functions for a Protein instance.
"""

import numpy as np


def get_scoring_aminos(protein):
    """
    Collect all placed aminos from a protein that may score points.
    """
    score_pos = {}
    cur_pos = [0 for _ in range(protein.get_dim())]
    idx, next_dir = protein.get_amino(cur_pos)

    # Store origin if it may score points.
    # if amino == "H":
    if protein.is_hydro(idx):
        score_pos[tuple(cur_pos)] = np.array([0, next_dir], dtype=np.int64)

    while next_dir != 0:
        # Compute position of next amino and get its amino and fold.
        cur_pos[abs(next_dir) - 1] += next_dir // abs(next_dir)
        idx, fold = protein.get_amino(cur_pos)

        # Store previous directions for checking existing connections.
        prev_dir = -next_dir
        next_dir = fold

        # Save amino if it may score points.
        if protein.is_hydro(idx):
            score_pos[tuple(cur_pos)] = np.array([prev_dir, next_dir],
                                                 dtype=np.int64)

    return score_pos


def get_scoring_pairs(protein):
    """
    Fetch positions of scoring amino in pairs for the currently folded amino
    acid chain.
    """
    # Get dictionary with the amino's that can possibly score points.
    score_aminos = get_scoring_aminos(protein)

    # Sort positions from bottom-left to upper-rigth.
    moves = np.array([m for m in range(1, protein.get_dim() + 1)])
    pairs = np.empty((1, 2, protein.get_dim()), dtype=np.int64)

    # Check if a score can be made with the amino above or to the right.
    for pos, [prev_dir, next_dir] in score_aminos.items():
        other_pos = np.array(pos, dtype=np.int64)

        # Check every positive direction, started in the lower quadrant.
        for move in moves:
            if prev_dir != move and next_dir != move:
                other_pos[move - 1] += 1

                if tuple(other_pos) in score_aminos:
                    new_pair = np.array([[pos, tuple(other_pos)]],
                                        dtype=np.int64)
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
    aminos = protein.get_sequence()[:len(moves) + 1]
    dim = protein.get_dim()

    # Setup storage of the amino positions.
    cur_pos = np.array([0 for _ in range(dim)], dtype=np.int64)
    positions = np.array([*cur_pos, aminos[0]])

    # Save the coordinates of the aminos in order.
    for amino, move in zip(aminos[1:], moves):
        cur_pos[abs(move) - 1] += move // abs(move)
        positions = np.vstack((positions, np.array([*cur_pos, amino])))

    return positions
