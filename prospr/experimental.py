#!/usr/bin/env python3
"""
File:           experimental.py
Description:    This file contains experimental functions which may not
                be documented or production ready.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

import warnings


def depth_first_symmetry(protein):
    """A depth-first brand-and-bound search function for finding a minimum energy
    conformation.
    Isomorphism pruning is applied based on symmetry between conformations.
    :param Protein      protein:    Protein object to fold.

    TODO:
    - Implement this function in C++
    - Prune using branch-and-bound method
    - Investigate/fix move evaluation (s. below)
    """

    if len(protein.sequence) < 4:
        # Cannot form any bonds (conformation does not matter)
        protein.set_hash([-1 for _ in protein.sequence[:-1]])
        return

    if len(protein.sequence) > 5:
        warnings.warn(
            "Experimental depth_first_symmetry(...) is written in pure Python"
            " and may struggle with large problem instances."
        )

    dimensions = protein.dim

    def recurse(protein, partial_hash, dimensions_moved):
        protein.set_hash(partial_hash, track=False)
        if len(protein.sequence) - 1 == len(partial_hash):
            return partial_hash, protein.score
        best_hash = None
        best_score = 1
        for move in range(-dimensions, dimensions + 1):
            if move == 0:
                continue  # Not a valid dimension
            # Prune axis symmetry
            if move not in dimensions_moved and move > 0:
                continue
            next_dimensions_moved = dimensions_moved.copy()
            next_dimensions_moved.add(abs(move))
            # FIXME: protein.is_valid(move) does not seem to work correctly
            # Just try to place it instead
            try:
                protein.set_hash(partial_hash, track=False)
                protein.place_amino(move, track=True)
            except RuntimeError:
                continue
            # Recurse
            check_hash = partial_hash + [move]
            local_best_hash, score = recurse(
                protein, check_hash, next_dimensions_moved
            )
            # Update solution
            if score < best_score:
                best_hash = local_best_hash
                best_score = score
        return best_hash, best_score

    # Prune radial symmetry of x-axis
    # The first move is fixed to -1 (Fixes x-axial symmetry)
    # However, [-1, -2], [-1,-3], etc. are isomorphic states (x-radial symmetry)
    best_hash1, score1 = recurse(protein, [-1, -1], {1})
    best_hash2, score2 = recurse(protein, [-1, -2], {1, 2})
    protein.set_hash(
        best_hash1 if score1 <= score2 else best_hash2, track=False
    )
