#!/usr/bin/env python3
"""
File:           test_protein.py
Description:    This file contains the pytest tests for the Protein core code.
"""

from prospr import Protein
import pytest


@pytest.fixture()
def protein_2d():
    return Protein("HPPHPPHH", 2)


@pytest.fixture()
def protein_3d():
    return Protein("HPPHPPHH", 3)


class TestProtein:
    def test_protein_2d_generation(self, protein_2d):
        """Test if a 2D protein is generated correctly."""
        assert protein_2d.sequence == "HPPHPPHH"
        assert protein_2d.dim == 2
        assert protein_2d.cur_len == 0
        assert protein_2d.last_move == 0
        assert protein_2d.last_pos == [0, 0]
        assert protein_2d.score == 0
        assert protein_2d.changes == 0

    def test_protein_3d_generation(self, protein_3d):
        """Test if a 3D protein is generated correctly."""
        assert protein_3d.sequence == "HPPHPPHH"
        assert protein_3d.dim == 3
        assert protein_3d.cur_len == 0
        assert protein_3d.last_move == 0
        assert protein_3d.last_pos == [0, 0, 0]
        assert protein_3d.score == 0
        assert protein_3d.changes == 0

    def test_protein_2d_moves(self, protein_2d):
        """Test if a 2D protein can move in all directions."""
        assert protein_2d.cur_len == 0
        moves = [0, 1, 2, -1, -1, -2]

        for i, m in enumerate(moves):
            protein_2d.place_amino(m)
            assert protein_2d.hash_fold() == moves[1 : i + 1]
            assert protein_2d.cur_len == len(moves[: i + 1])
            assert protein_2d.last_move == m

    def test_protein_3d_moves(self, protein_3d):
        """Test if a 3D protein can move in all directions."""
        assert protein_3d.cur_len == 0
        moves = [0, 1, 2, -1, 3, -2, -1, -3]

        for i, m in enumerate(moves):
            protein_3d.place_amino(m)
            assert protein_3d.hash_fold() == moves[1 : i + 1]
            assert protein_3d.cur_len == len(moves[: i + 1])
            assert protein_3d.last_move == m

    def test_protein_2d_undo_move(self, protein_2d):
        """Test if a 2D protein can remove an amino."""
        moves = [0, 1, 2, -1, -1, -2]

        for m in moves:
            protein_2d.place_amino(m)

        for m in moves[:0:-1]:
            protein_2d.remove_amino(m)

    def test_protein_3d_undo_move(self, protein_3d):
        """Test if a 3D protein can remove an amino."""
        moves = [0, 1, 2, -1, 3, -2, -1, -3]

        for m in moves:
            protein_3d.place_amino(m)

        for m in moves[:0:-1]:
            protein_3d.remove_amino(m)
