#!/usr/bin/env python3
"""
File:           test_protein.py
Description:    This file contains the pytest tests for the Protein core code.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2022). See the LICENSE file for the
                specifics.
"""

from prospr import Protein
import pytest


@pytest.fixture()
def protein_2d():
    return Protein("HPPHPPHH", dim=2, model="HP")


@pytest.fixture()
def protein_3d():
    return Protein("HPPHPPHH", dim=3, model="HP")


@pytest.mark.order(order=1)
class TestProtein:
    def test_protein_2d_generation(self, protein_2d):
        """Test if a 2D protein is generated correctly."""
        assert protein_2d.sequence == "HPPHPPHH"
        assert protein_2d.dim == 2
        assert protein_2d.bond_values == {"HH": -1}
        assert protein_2d.cur_len == 1
        assert protein_2d.last_move == 0
        assert protein_2d.last_pos == [0, 0]
        assert protein_2d.score == 0
        assert protein_2d.solutions_checked == 0
        assert protein_2d.aminos_placed == 1

    def test_protein_3d_generation(self, protein_3d):
        """Test if a 3D protein is generated correctly."""
        assert protein_3d.sequence == "HPPHPPHH"
        assert protein_3d.dim == 3
        assert protein_3d.bond_values == {"HH": -1}
        assert protein_3d.cur_len == 1
        assert protein_3d.last_move == 0
        assert protein_3d.last_pos == [0, 0, 0]
        assert protein_3d.score == 0
        assert protein_3d.solutions_checked == 0
        assert protein_3d.aminos_placed == 1

    def test_protein_2d_place_moves(self, protein_2d):
        """Test if a 2D protein can move in all directions."""
        assert protein_2d.cur_len == 1
        moves = [1, 2, -1, -1, -2]
        scores = [0, 0, -1, -1, -1]
        track_placed = [1, 0, 1, 0, 1]

        for i, m in enumerate(moves):
            protein_2d.place_amino(m, track=bool(track_placed[i]))
            assert protein_2d.hash_fold() == moves[: i + 1]
            assert protein_2d.cur_len == len(moves[: i + 1]) + 1
            assert protein_2d.last_move == m
            assert protein_2d.score == scores[i]
            assert protein_2d.aminos_placed == 1 + sum(track_placed[: i + 1])

    def test_protein_3d_place_moves(self, protein_3d):
        """Test if a 3D protein can move in all directions."""
        assert protein_3d.cur_len == 1
        moves = [1, 2, -1, 3, -2, -1, -3]
        scores = [0, 0, -1, -1, -1, -1, -2]
        track_placed = [1, 0, 1, 0, 1, 0, 1]

        for i, m in enumerate(moves):
            protein_3d.place_amino(m, track=bool(track_placed[i]))
            assert protein_3d.hash_fold() == moves[: i + 1]
            assert protein_3d.cur_len == len(moves[: i + 1]) + 1
            assert protein_3d.last_move == m
            assert protein_3d.score == scores[i]
            assert protein_3d.aminos_placed == 1 + sum(track_placed[: i + 1])

    def test_protein_2d_undo_moves(self, protein_2d):
        """Test if a 2D protein can remove amino acids in all directions."""
        moves = [1, 2, -1, -1, -2]

        for m in moves:
            protein_2d.place_amino(m)

        for _ in range(len(moves)):
            protein_2d.remove_amino()

    def test_protein_3d_undo_moves(self, protein_3d):
        """Test if a 3D protein can remove amino acids in all directions."""
        moves = [1, 2, -1, 3, -2, -1, -3]

        for m in moves:
            protein_3d.place_amino(m)

        for _ in range(len(moves)):
            protein_3d.remove_amino()

    def test_protein_2d_get_bond_indexes(self, protein_2d):
        """Test if a 2D protein correctly computes indexes of formed bonds."""
        moves = [1, 2, -1, -1, -2]

        for m in moves:
            protein_2d.place_amino(m)

        assert protein_2d.get_bonds() == [(0, 3), (3, 0)]
