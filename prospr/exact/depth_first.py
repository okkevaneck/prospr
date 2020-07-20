#!/usr/bin/env python3
"""
File:       depth_first.py
Author:     Okke van Eck

Description:    This file defines the depth-first search algorithm for folding a
                Protein object.
"""

import numpy as np


def depth_first(protein):
    """
    Depth-first algorithm for folding a protein.
    :param Protein  protein:    Protein object to process.
    :return int:                Best found score.
    """
    max_length = len(protein.get_sequence())

    # First two aminos are placed to prevent symmetry. These are not placed on
    # the stack since it may not be backtracked.
    if max_length >= 1:
        protein.place_amino(0)
    if max_length >= 2:
        protein.place_amino(2)
    if max_length < 3:
        return 0

    # Setup the stack as if the protein is one long upwards going chain. The
    # remaining moves are all the positive directions of all dimensions to
    # prevent symmetry over all axis.
    dim = protein.get_dim()
    stack_size = max_length - 2
    move = 2

    stack = np.zeros((stack_size, 2 * dim - 1), dtype=np.int64)
    stack_pointer = stack_size - 1

    for i in range(max_length - 2):
        protein.place_amino(move)
        idx = 0

        for j in range(-dim, dim):
            if j != 0 and j != move:
                stack[i][idx] = j
                idx += 1

    # Init variables for run.
    moves = np.array([m for m in range(-dim, dim) if m != 0], dtype=np.int64)
    best_score = 1
    best_hash = None

    while stack:
        # Check score if bottom of tree is reached and save best protein.
        # Otherwise continue with next amino and possible moves.
        if protein.get_cur_len() == max_length:
            score = protein.get_score()

            if score < best_score:
                best_score = score
                best_hash = protein.hash_fold()

            remaining_moves = np.zeros((2 * dim - 1), dtype=np.int64)
        else:
            remaining_moves = moves[moves != -move]
            remaining_moves.reshape((2 * dim - 1))

        # Loop till an amino has been placed or no new combinations are left.
        placed_amino = False

        while not placed_amino:
            # Try all remaining moves for the current amino and place if valid.
            while not placed_amino and remaining_moves:
                # Pop move from remaining moves.
                move = remaining_moves[0]
                remaining_moves = remaining_moves[remaining_moves != move]
                remaining_moves.reshape((2 * dim - 1))

                # Place amino, update length and save the move on the stack
                # with the remaining possible moves, if valid.
                if protein.is_valid(move):
                    protein.place_amino(move)
                    stack[stack_pointer] = remaining_moves
                    stack_pointer += 1
                    placed_amino = True

            # Backtrack if the current amino has no possible moves left.
            # while not placed_amino and not remaining_moves and stack:
            while placed_amino == False and remaining_moves.size == 0 and stack:
                remaining_moves = stack[-1]
                stack_pointer -= 1
                protein.remove_amino()

            # Check if all possible combinations have been checked.
            if not remaining_moves:
                break

    protein.set_hash(best_hash)

    return best_score, best_hash
