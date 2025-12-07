#!/usr/bin/env python3
"""
File:           helpers.py
Description:    This file contains helper functions for a Protein instance.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

from io import StringIO
from pathlib import Path
import numpy as np


def _protein_to_coordinates(protein):
    """
    Map 2D/3D protein fold to grid coordinates
    """
    coordinates = [[0, 0, 0]]
    for move in protein.hash_fold():
        coordinates.append(coordinates[-1].copy())
        coordinates[-1][abs(move) - 1] += 1 if move > 0 else -1
    offset = [0, 0, 0]
    # Avoid negative coordinates
    for c in coordinates:
        for i in range(3):
            if c[i] < 0:
                offset[i] = max(offset[i], abs(c[i]))
    for c in coordinates:
        c[0] += offset[0]
        c[1] += offset[1]
        c[2] += offset[2]
    return coordinates


def export_protein(protein, path):
    """
    Save conformation of a protein in Protein Data Bank (PDB) file format
    for processing or visualization with external software such as Mol*.
    :param Protein  protein:        Protein object to save the hash of.
    :param os.PathLike | str  path: The path of the output file.
    """
    if protein.dim not in [2, 3]:
        raise ValueError("Only 2D or 3D proteins can be saved as PDB files.")
    path = Path(path)
    if not path.suffix.lower() == ".pdb":
        raise ValueError('Suffix ".pdb" required for output PDB files.')
    coordinates = _protein_to_coordinates(protein)
    buf = StringIO()
    # Header
    buf.write("HEADER    HP-protein folding structure\n")
    buf.write(f"TITLE     Sequence: {protein.sequence}\n")
    buf.write(
        "REMARK    "
        + "Generated using prospr (https://github.com/okkevaneck/prospr)\n"
    )
    # Amino acids
    for i, c in enumerate(coordinates):
        x, y, z = [v * 3.8 for v in c]  # "Safe" distance is 3.8 Angstrom
        # Use some known H- and P-type amino acid
        amino_acid = "ALA" if protein.sequence[i] == "H" else "SER"
        buf.write(f"ATOM  {i+1:5d}  CA  {amino_acid:>3} A{i+1:4d}    ")
        buf.write(f"{x:8.3f}{y:8.3f}{z:8.3f}  1.00  0.00           C\n")
    # Chain
    buf.write("CONECT    1    2\n")
    for i in range(2, len(coordinates)):
        buf.write(f"CONECT {i:4d} {i-1:4d} {i+1:4d}\n")
    buf.write("END\n")
    path.write_text(buf.getvalue())


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
